#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

//include header file for glfw library so that we can use OpenGL
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "smVector.hpp"

#define WIDTH 20
#define HEIGHT 20
#ifdef _WIN32
static DWORD lastTime;
#else
static struct timeval lastTime;
#endif

#define PI 3.14159265 // Should be used from mathlib

using namespace std;

//****************************************************
// Global Variables
//****************************************************
SmVector3 sheet_pos[WIDTH][HEIGHT];
SmVector3 sheet_v[WIDTH][HEIGHT];
SmVector3 adjust[WIDTH][HEIGHT];
SmVector3 sheet_pos_tmp[WIDTH][HEIGHT];
bool sheet_locked[WIDTH][HEIGHT];
int adjCount[WIDTH][HEIGHT];
int isAdjust[WIDTH][HEIGHT];
double spring_I[WIDTH - 1][HEIGHT];
double spring_J[WIDTH][HEIGHT - 1];


SmVector3 sheet_g(0, 0, 9.8);

GLfloat translation[3] = {0.0f, 0.0f, 0.0f};
bool auto_strech = false;
int Width_global = 400;
int Height_global = 400;
int Z_buffer_bit_depth = 128;
int isStart = 0;
int isReset = 0;
int rotate = 1;
int rotate_x = 0;
int rotate_y = 0;
int rotate_z = 0;
int wire = 0;
int releaseLock = 0;
double phase = 0;
inline float sqr(float x) { return x*x; }


//****************************************************
// Simple init function
//****************************************************
void initializeRendering()
{
    glfwInit();
}
int tm = 0;
void EnforceConstraints(double dt)
{
  tm++;
  int numOfAdjust;
  
  const int max_iter = 5;
  for(int i=0;i < max_iter;i++)
  {
    for(int w = 0; w < WIDTH; w++)
    {
      for(int h = 0; h < HEIGHT; h++)
      {
        adjCount[w][h] = 0;
        adjust[w][h] = SmVector3(0,0,0);
        
      }
    }
    double dist = 0;
    SmVector3 diff(0,0,0);
    for(int w = 0; w < WIDTH; w++)
    {
      for(int h = 0; h < HEIGHT; h++)
      {
        if(w!=WIDTH -1)
        {
          
          diff = sheet_pos[w + 1][h] - sheet_pos[w][h];
          dist = mag(diff) - spring_I[w][h];
          if(dist !=0)
          {
            adjust[w][h] += 0.4*dist*(diff)/(mag(diff));
            adjust[w+1][h] -= 0.4*dist*(diff)/(mag(diff));
            adjCount[w][h]++;
            adjCount[w+1][h]++;
            isAdjust[w][h] = 1;
            isAdjust[w+1][h] = 1;


          }
          
        }

        if(h!=HEIGHT -1)
        {
          diff = sheet_pos[w][h + 1] - sheet_pos[w][h];
          dist = mag(diff) - spring_J[w][h];
          if(dist != 0)
          {

            adjust[w][h] += 0.4*dist*(diff)/(mag(diff));
            adjust[w][h+1] -= 0.4*dist*(diff)/(mag(diff));
            adjCount[w][h]++;
            adjCount[w][h+1]++;
            isAdjust[w][h] = 1;
            isAdjust[w][h+1] = 1;
            // if(tm==2 && w == 5 && h == 8)
            //   cout << "reach"  <<endl;
          }
          
        }
      
      }
    }

    for(int w = 0; w < WIDTH; w++)
    {
      for(int h = 0; h < HEIGHT; h++)
      {
        if(adjCount[w][h]!=0)
        {
          if(!sheet_locked[w][h])
          {
          
            sheet_pos[w][h] += (adjust[w][h] / adjCount[w][h]);
          }
        }
      }
    }
  }

  for(int w = 0; w < WIDTH; w++)
  {
    for(int h = 0; h < HEIGHT; h++)
    {
      //cout << sheet_pos[w][h] - sheet_pos_tmp[w][h] <<endl;
      if(!sheet_locked[w][h] && isAdjust[w][h])
      {
        //if(sheet_pos[w][h] - sheet_pos_tmp[w][h]!= SmVector3(0,0,0))
       
        sheet_v[w][h] = (sheet_pos[w][h] - sheet_pos_tmp[w][h])/dt;
       
      }
      
    }
  }
}
SmVector3 intersectSphere(SmVector3 e, SmVector3 d, double t0, double t1, SmVector3 c, double r, bool &hit)  {
  // If r misses sphere, return false.
  //
  // If r hits, then 
  // set hr.t to the t value of the hit
  // set hr.p to the location of the hit (point in space)
  // set hr.n to the normal of the sphere at hr.p
  // return true

  // <<>>
  double ea = dot(d,d);
  double eb = 2*(dot(e,d) - dot(d,c));
  double ec = dot(e,e) + dot(c, c) - 2*(dot(e, c)) - r*r;
  double delta = eb*eb - 4*ea*ec;
  double res = 0;
  if(delta < 0)
  {
    hit = false;
    return SmVector3(0,0,0);
  }
  else
  {
    res = (-eb - sqrt(delta))/(2*ea);
    if(res >= t0 && res <= t1)
    {
      hit = true;
      return e + (res)*d;
    }
    else
    {
      res = (-eb + sqrt(delta))/(2*ea);
      if(res >= t0 && res <= t1)
      {
        hit = true;
        return e + (res)*d;
      }
      else
      {
        hit = false;
        return SmVector3(0,0,0);
      }
    }
  }

}


void EnforceSphereConstraints(double r, SmVector3 c)
{
  double u = 1;
  double v = 1;
  for(int w = 0; w < WIDTH; w++)
  {
    for(int h = 0; h < HEIGHT; h++)
    {
      if(!sheet_locked[w][h])
      {
        bool hit = false;
        SmVector3 e = sheet_pos_tmp[w][h];
        SmVector3 d = sheet_pos[w][h] - sheet_pos_tmp[w][h]; 
        SmVector3 p = intersectSphere(e,d,0,1,c,r,hit);
        if(hit)
        {
          SmVector3 n = p - c;
          normalize(n);
          SmVector3 r = sheet_pos[w][h] - p;
          sheet_pos[w][h] = u*(r - dot(n,r) * n) - v*dot(n,r) * n + p;
          sheet_v[w][h] = u*(sheet_v[w][h] - dot(n, sheet_v[w][h])*n) - v*dot(n, sheet_v[w][h])*n;
        }
       
      }
      
    }
  }
}
void integrate(double dt)
{
  for(int i=0; i < WIDTH; i++)
  {
    for(int j=0; j < HEIGHT; j++)
    {
      sheet_pos_tmp[i][j] = sheet_pos[i][j];
      //isAdjust[w][h] = 0;
      if(sheet_locked[i][j])
      {

        //continue;
        sheet_v[i][j] = SmVector3(0,0,0);
      }
      else
      {
        sheet_v[i][j] += dt*sheet_g;
        sheet_pos[i][j] += dt*sheet_v[i][j];
        
        //cout << sheet_pos[i][j] <<endl;
      }
    }
  }

}

void init()
{
  for(int i=0; i < WIDTH; i++)
  {
    for(int j=0; j < HEIGHT; j++)
    {
      if(i == 0)
      //if(i == 0 || j == 0)
      //if(i == 0 || j == 0 || i == WIDTH-1)
      //if(i == 0 || j == 0 || i == WIDTH-1 || j ==HEIGHT-1)
      //if(false)
      {
        sheet_locked[i][j] = true;
      }
      else
      {
        sheet_locked[i][j] = false;
      }
      sheet_pos[i][j][0] = (i/20.0)*2;
      sheet_pos[i][j][1] = (j/20.0)*2;
      //sheet_pos[i][j][2] = sin(2*PI*(i+ phase)/20.0)*0.5;
      sheet_pos[i][j][2] = 0;
      sheet_v[i][j] = SmVector3(0,0,0);
      //cout << sheet_pos[i][j] <<endl;
    }
  }
  //cout << intersect(SmVector3(1,1,1), SmVector3(-1,-1,-1), 0, 100, SmVector3(0,0,0), 1.0) <<endl;
  for(int i=0; i < WIDTH; i++)
  {
    for(int j=0; j < HEIGHT; j++)
    {
      if(i!= WIDTH -1)
      {

        spring_I[i][j] = mag(sheet_pos[i+1][j] - sheet_pos[i][j]);
        
      }
      if(j!=HEIGHT-1)
      {
         spring_J[i][j] = mag(sheet_pos[i][j+1] - sheet_pos[i][j]);

      }
    }
  }

}

GLdouble* smToArray(SmVector3 &p)
{
  GLdouble *arr = new GLdouble[3];
  arr[0] = p[0];
  arr[1] = p[1];
  arr[2] = p[2];
  return arr;
}
//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************
void setPixel(float x, float y, GLfloat r, GLfloat g, GLfloat b) {
    glColor3f(r, g, b);
    glVertex2f(x+0.5, y+0.5);  // The 0.5 is to target pixel centers
    // Note: Need to check for gap bug on inst machines.
}

//****************************************************
// Keyboard inputs. Add things to match the spec! 
//****************************************************
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key) {
            
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        case GLFW_KEY_Q: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
        case GLFW_KEY_LEFT :
            if (action && mods == GLFW_MOD_SHIFT) translation[0] -= 0.001f * Width_global; break;
        case GLFW_KEY_RIGHT:
            if (action && mods == GLFW_MOD_SHIFT) translation[0] += 0.001f * Width_global; break;
        case GLFW_KEY_UP   :
            if (action && mods == GLFW_MOD_SHIFT) translation[1] += 0.001f * Height_global; break;
        case GLFW_KEY_DOWN :
            if (action && mods == GLFW_MOD_SHIFT) translation[1] -= 0.001f * Height_global; break;
        case GLFW_KEY_R :
            if (action) rotate_x = (rotate_x+2)%360; break;
        case GLFW_KEY_T :
            if (action) rotate_y = (rotate_y+2)%360; break;
        case GLFW_KEY_X :
            if (action) releaseLock = ~releaseLock; break;
        case GLFW_KEY_Y :
            if (action) rotate_z = (rotate_z+2)%360; break;
        case GLFW_KEY_SPACE :
            if (action) isStart = !isStart; break;
        case GLFW_KEY_P :
            if (action) isReset = 1; break;
        case GLFW_KEY_W:
            if (action) wire = !wire; break;
        case GLFW_KEY_F:
            if (action && mods == GLFW_MOD_SHIFT) auto_strech = !auto_strech; break;
            
        default: break;
    }
    
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
  
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        cout << "press" <<endl;
}

//****************************************************
// Draw a cube. You don't need this for your final assignment, but it's
// here so you don't look at a blank screen.
// Taken from https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html
//****************************************************
void drawCube() {
	 glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f( 1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f, -1.0f);
      glVertex3f(-1.0f, 1.0f,  1.0f);
      glVertex3f( 1.0f, 1.0f,  1.0f);
 
      // Bottom face (y = -1.0f)
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glVertex3f( 1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f( 1.0f, -1.0f, -1.0f);
 
      // Front face  (z = 1.0f)
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( 1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f,  1.0f, 1.0f);
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glVertex3f( 1.0f, -1.0f, 1.0f);
 
      // Back face (z = -1.0f)
      glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      glVertex3f( 1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f( 1.0f,  1.0f, -1.0f);
 
      // Left face (x = -1.0f)
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.0f,  1.0f,  1.0f);
      glVertex3f(-1.0f,  1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f, -1.0f);
      glVertex3f(-1.0f, -1.0f,  1.0f);
 
      // Right face (x = 1.0f)
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glVertex3f(1.0f,  1.0f, -1.0f);
      glVertex3f(1.0f,  1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f,  1.0f);
      glVertex3f(1.0f, -1.0f, -1.0f);
   glEnd();  // End of drawing color-cube
}

void drawSheet()
{
  glBegin(GL_QUADS);  
  for(int i=0; i < WIDTH - 1; i++)
  {
    for(int j=0; j < HEIGHT - 1; j++)
    {
      glColor3f(1.0, 1.0, 0.0);
      SmVector3 normal = -cross(sheet_pos[i+1][j]-sheet_pos[i][j],
      sheet_pos[i][j+1]-sheet_pos[i][j]);
      normalize(normal);
      glNormal3d(normal[0],normal[1],normal[2]);


      glVertex3d(sheet_pos[i][j][0],  sheet_pos[i][j][1],  sheet_pos[i][j][2]);
      glVertex3d(sheet_pos[i][j+1][0],  sheet_pos[i][j+1][1],  sheet_pos[i][j+1][2]);
      glVertex3d(sheet_pos[i+1][j+1][0],  sheet_pos[i+1][j+1][1],  sheet_pos[i+1][j+1][2]);
      glVertex3d(sheet_pos[i+1][j][0],  sheet_pos[i+1][j][1],  sheet_pos[i+1][j][2]);


    }
  }
  glEnd();

}

int hstep = 45;
int vstep = 90;
void drawSphere(double r, SmVector3 c)
{
  static int tx = 0;
  tx++;
  glBegin(GL_QUADS); 
  int hspace = 180 / hstep;
  int vspace = 360 / vstep;
  SmVector3 p0,p1,p2,p3, normal;
  glColor3f(0.5, 0.5, 0.0);
  for(int i = 0;i <= 180; i+=hspace)
  {
    for(int j = 0;j <= 360; j+=vspace)
    {
      if(i == 0 || i == 180)
      {
        p0 = SmVector3(r*sin(i*PI/180.0)*sin(j*PI/180.0), r*sin(i*PI/180.0)*cos(j*PI/180.0), r*cos(i*PI/180.0));
        p1 = SmVector3(r*sin((i+hspace)*PI/180.0)*sin((j+vspace)*PI/180.0), r*sin((i+hspace)*PI/180.0)*cos((j+vspace)*PI/180.0), r*cos((i+hspace)*PI/180.0));
        p2 = SmVector3(r*sin((i+hspace)*PI/180.0)*sin(j*PI/180.0), r*sin((i+hspace)*PI/180.0)*cos(j*PI/180.0), r*cos((i+hspace)*PI/180.0));
        p0 += c;
        p1 += c;
        p2 += c;

        normal = cross(p1 - p0, p2 - p0);
        normalize(normal);
        glNormal3d(normal[0],normal[1],normal[2]);
        glVertex3d(p0[0], p0[1], p0[2]);
        glVertex3d(p1[0], p1[1], p1[2]);
        glVertex3d(p2[0], p2[1], p2[2]);
        continue;
      }
      p0 = SmVector3(r*sin(i*PI/180.0)*sin(j*PI/180.0), r*sin(i*PI/180.0)*cos(j*PI/180.0), r*cos(i*PI/180.0));
      p1 = SmVector3(r*sin(i*PI/180.0)*sin((j+vspace)*PI/180.0), r*sin(i*PI/180.0)*cos((j+vspace)*PI/180.0), r*cos(i*PI/180.0));
      p2 = SmVector3(r*sin((i+hspace)*PI/180.0)*sin((j+vspace)*PI/180.0), r*sin((i+hspace)*PI/180.0)*cos((j+vspace)*PI/180.0), r*cos((i+hspace)*PI/180.0));
      p3 = SmVector3(r*sin((i+hspace)*PI/180.0)*sin(j*PI/180.0), r*sin((i+hspace)*PI/180.0)*cos(j*PI/180.0), r*cos((i+hspace)*PI/180.0));
      p0 += c;
      p1 += c;
      p2 += c;
      p3 += c;
      normal = cross(p1 - p0, p3 - p0);

      normalize(normal);
      glNormal3d(normal[0],normal[1],normal[2]);
      glVertex3d(p0[0], p0[1], p0[2]);
      glVertex3d(p1[0], p1[1], p1[2]);
      glVertex3d(p2[0], p2[1], p2[2]);
      glVertex3d(p3[0], p3[1], p3[2]);
    }
  }
  glEnd();
}
//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void display( GLFWwindow* window )
{
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black)
    glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
    glLoadIdentity();                            // make sure transformation is "zero'd"
    glShadeModel (GL_SMOOTH);
    phase += 0.006;
    //cout << phase <<endl;
   // init();
    if(releaseLock)
    {
      for(int i=0; i < WIDTH; i++)
      {
        for(int j=0; j < HEIGHT; j++)
        {
          sheet_locked[i][j] = false;
        }
      }
      releaseLock = 0;
    }
    if(isStart)
    {
      integrate(0.005);
      EnforceConstraints(0.005);
      EnforceSphereConstraints(0.3, SmVector3(1.0,1.0,0.8));
    }

    if(isReset)
    {
      init();
      isStart = 0;
      isReset = 0;
    }
    
    
    
    //EnforceConstraints(0.00005);
    //----------------------- code to draw objects --------------------------
    if(wire)
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glPushMatrix();
    glTranslatef (-2.5f, 2.0f, 0.0f);
    glScalef(2.0f, 2.0f, 1.0f);
    glTranslatef (translation[0], translation[1], translation[2]);
    //glRotatef(45*rotate, 1, 1, 0); //rotates the cube below
   glRotatef(80, 1, 0, 0);
    glRotatef(-45, 0, 0, 1);
    glRotatef(rotate_x, 1, 0, 0);
    glRotatef(rotate_y, 0, 1, 0);
    glRotatef(rotate_z, 0, 0, 1);

    drawSphere(0.3, SmVector3(1.0,1.0,0.8));
    drawSheet(); // REPLACE ME!
    
    //gluSphere();
    //glutSolidSphere( 1.0f, 0, 0 );
    //gluSolidSphere();
    glPopMatrix();
    
    glfwSwapBuffers(window);

    // note: check out glPolygonMode and glShadeModel 
    // for wireframe and shading commands
    
}

//****************************************************
// function that is called when window is resized
//***************************************************
void size_callback(GLFWwindow* window, int width, int height)
{

    // Get the pixel coordinate of the window
    // it returns the size, in pixels, of the framebuffer of the specified window
    glfwGetFramebufferSize(window, &Width_global, &Height_global);


    glViewport(0, 0, Width_global, Height_global);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -3.5  * Width_global/Height_global, 3.5* Width_global/Height_global, -3.5, 3.5, 5, -5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    display(window);
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
    //This initializes glfw
    initializeRendering();
    init();
    GLFWwindow* window = glfwCreateWindow( Width_global, Height_global, "CS184", NULL, NULL );
    if ( !window )
    {
        cerr << "Error on window creating" << endl;
        glfwTerminate();
        return -1;
    }

    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if ( !mode )
    {
        cerr << "Error on getting monitor" << endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent( window );
    
    // Get the pixel coordinate of the window
    // it returns the size, in pixels, of the framebuffer of the specified window
    glfwGetFramebufferSize(window, &Width_global, &Height_global);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-3.5, 3.5, -3.5, 3.5, 5, -5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glEnable(GL_DEPTH_TEST);	// enable z-buffering
    glDepthFunc(GL_LESS);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
    // GLfloat ambient[]={1,1,1,1.0};
    // GLfloat diffuse[]={1.0,1.0,1.0,1.0};
    // GLfloat specular[]={1.0,1.0,1.0,1.0};
    //  GLfloat light0_pos[]={1.0,1.0,1.0};
    // GLint shine = 100;
    // glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    // glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
    // glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse);
    // glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
    // glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,shine);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glfwSetWindowTitle(window, "CS184");
    glfwSetWindowSizeCallback(window, size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    while( !glfwWindowShouldClose( window ) ) // infinite loop to draw object again and again
    {   // because once object is draw then window is terminated
        display( window );
        
        if (auto_strech){
            glfwSetWindowSize(window, mode->width, mode->height);
            glfwSetWindowPos(window, 0, 0);
        }
        
        glfwPollEvents();
        
    }

    return 0;
}
