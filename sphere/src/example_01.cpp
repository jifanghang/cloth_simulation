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


#define PI 3.14159265 // Should be used from mathlib

using namespace std;

//===================================================
// Class to hold the scene
// Should be in separate header file with it's 
// implementation in separate cpp file.
//===================================================

class Light {
public:
  SmVector3 pos;
  SmVector3 col;
}; 

class Scene {
public:
  SmVector3 ka;
  SmVector3 kd;
  SmVector3 ks;
  double    sp;

  int numPL = 0;
  Light pL[5];

  int numDL = 0;
  Light dL[5];
};

class Buffer {
protected:
  
  int        w,h;
  SmVector3 *mem;

public:

  Buffer() {
    w = h = 0;
    mem = NULL;
  }


  void setSize(int newW, int newH) {
    //cerr << "Buffer set to " << newW << " " << newH << "\n";
    if ((newW == w) && (newH == h)) return;
    //cerr << "Resize\n";

    if (mem != NULL) { delete mem; }
    
    if ((newW!=0) && (newH!=0)) {
      w = newW;
      h = newH;
      mem = new SmVector3[w*h];
    }else{
      mem = NULL;
    }
  }


  ~Buffer() {
    setSize(0,0);
  }

  SmVector3 get(int i, int j) {
    if ((w==0) && (h==0)) return 0.0;
    if ((i<0) || (i>=w) ||
	(j<0) || (j>=h)) {
      cerr << "Range error!\n";
      cerr << i << " " << j << " " << w << " " << h << "\n";
      abort();
    }
    return mem[i+j*w];
  }

  void set(int i, int j, SmVector3 val) {
    if ((w==0) && (h==0)) return;
    if ((i<0) || (i>=w) ||
	(j<0) || (j>=h)) {
      cerr << "Range error!\n";
      cerr << i << " " << j << " " << w << " " << h << "\n";
      abort();
    }
    mem[i+j*w] = val;
  }

  void clear() {
    if (w!=0 && h!=0) {
      int i,j;
      SmVector3 black(0,0,0);
      for (i=0;i<w;i++)
	for (j=0;j<w;j++)
	  set(i,j,black);
    }
  }

};


//===================================================
// Global Variables
//===================================================
GLfloat translation[3] = {0.0f, 0.0f, 0.0f};
bool auto_strech = false;
int Width_global = 600;
int Height_global = 600;

Scene scene;
Buffer buffer;

inline float sqr(float x) { return x*x; }




//===================================================
// Simple init function
//===================================================
void initializeRendering()
{
  glfwInit();
}


//===================================================
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//===================================================
void setPixel(float x, float y, GLfloat r, GLfloat g, GLfloat b) {

  buffer.set(int(x),int(y),SmVector3(r,g,b));

  glColor3f(r, g, b);
  glVertex2f(x+0.5, y+0.5);  // The 0.5 is to target pixel centers
  // Note: Need to check for gap bug on inst machines.
}

//===================================================
// Keyboard inputs
//===================================================
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  switch (key) {
            
  case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
  case GLFW_KEY_Q: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
  case GLFW_KEY_LEFT :
    if (action) translation[0] -= 0.01f * Width_global; break;
  case GLFW_KEY_RIGHT:
    if (action) translation[0] += 0.01f * Width_global; break;
  case GLFW_KEY_UP   :
    if (action) translation[1] += 0.01f * Height_global; break;
  case GLFW_KEY_DOWN :
    if (action) translation[1] -= 0.01f * Height_global; break;
  case GLFW_KEY_F:
    if (action) auto_strech = !auto_strech; break;
  case GLFW_KEY_SPACE: break;
            
  default: break;
  }
    
}


//===================================================
// Draw a filled circle.
//===================================================
void drawCircle(float centerX, float centerY, float radius) {
  // Draw inner circle
  glBegin(GL_POINTS);

  // We could eliminate wasted work by only looping over the pixels
  // inside the sphere's radius.  But the example is more clear this
  // way.  In general drawing an object by looping over the whole
  // screen is wasteful.

  int minI = max(0,(int)floor(centerX-radius));
  int maxI = min(Width_global-1,(int)ceil(centerX+radius));

  int minJ = max(0,(int)floor(centerY-radius));
  int maxJ = min(Height_global-1,(int)ceil(centerY+radius));

  for (int i = 0; i < Width_global; i++) {
    for (int j = 0; j < Height_global; j++) {

      // Location of the center of pixel relative to center of sphere
      float x = (i+0.5-centerX);
      float y = (j+0.5-centerY);

      float dist = sqrt(sqr(x) + sqr(y));

      if (dist <= radius) {

	// This is the front-facing Z coordinate
	float z = sqrt(radius*radius-dist*dist);

	SmVector3 surface(x/radius,y/radius,z/radius);
	SmVector3 normal = surface; normalize(normal); // Assume centered at origin

	SmVector3 eye(0.0,0.0,1.0); normalize(eye);  

	SmVector3 color(0.0, 0.0, 0.0);

	int l;

	for (l=0; l < scene.numPL; l++) {
	  SmVector3 light = scene.pL[l].pos - surface;
	  normalize(light);
	  color += scene.kd * scene.pL[l].col * max( 0.0, dot(light,normal) );

	  SmVector3 refl = 2* dot(light,normal) * normal - light;
	  color += scene.ks * scene.pL[l].col * pow( max ( 0.0, dot(eye,refl) ) , scene.sp );
	}


	for (l=0; l < scene.numDL; l++) {
	  SmVector3 light = -scene.dL[l].pos;
	  normalize(light);
	  color += scene.kd * scene.dL[l].col * max( 0.0, dot(light,normal) );

	  SmVector3 refl = 2* dot(light,normal) * normal - light;
	  color += scene.ks * scene.dL[l].col * pow( max ( 0.0, dot(eye,refl) ) , scene.sp );
	}

	color += scene.ka;

	setPixel(i, j, color[0], color[1], color[2]);

	// This is amusing, but it assumes negative color values are treated reasonably.
	// setPixel(i,j, x/radius, y/radius, z/radius );
                
	// Just for fun, an example of making the boundary pixels yellow.
	// if (dist > (radius-1.0)) {
	//     setPixel(i, j, 1.0, 1.0, 0.0);
	// }

      }
    }
  }

  glEnd();
}

//===================================================
// function that does the actual drawing of stuff
//===================================================
void display( GLFWwindow* window )
{
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f ); //clear background screen to black
    
  glClear(GL_COLOR_BUFFER_BIT);                // clear the color buffer (sets everything to black)
    
  glMatrixMode(GL_MODELVIEW);                  // indicate we are specifying camera transformations
  glLoadIdentity();                            // make sure transformation is "zero'd"
    
  buffer.setSize(Width_global,Height_global);
  buffer.clear();

  //----------------------- code to draw objects --------------------------
  glPushMatrix();
  glTranslatef (translation[0], translation[1], translation[2]);
  drawCircle(Width_global / 2.0 , Height_global / 2.0 , min(Width_global, Height_global) * 0.8 / 2.0);
  glPopMatrix();
    
  glfwSwapBuffers(window);
    
}

//===================================================
// function that is called when window is resized
//===================================================
void size_callback(GLFWwindow* window, int width, int height)
{
  // Get the pixel coordinate of the window
  // it returns the size, in pixels, of the framebuffer of the specified window
  glfwGetFramebufferSize(window, &Width_global, &Height_global);
    
  glViewport(0, 0, Width_global, Height_global);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, Width_global, 0, Height_global, 1, -1);
    
  display(window);
}


//===================================================
// Parse arguments
//===================================================

void parse_arguments(int argc, char *argv[]) {

  int i;
  
  //for (i = 0 ; i < argc ; i ++ ) {
  //  cout << "#" << i << " " << argv[i] << "\n";
  //}

  for (i = 1 ; i < argc ; i ++ ) {
    // Start at 1 to skip program name

    if (strcmp(argv[i],"-ka") == 0 ) {

      if ( (i+3) >= argc ) {
	cout << "You need three arguments for -ka.\n";
	exit(-1);
      }

      scene.ka[0] = atof(argv[++i]);
      scene.ka[1] = atof(argv[++i]);
      scene.ka[2] = atof(argv[++i]);

     } else if (strcmp(argv[i],"-kd") == 0 ) {

      if ( (i+3) >= argc ) {
	cout << "You need three arguments for -kd.\n";
	exit(-1);
      }

      scene.kd[0] = atof(argv[++i]);
      scene.kd[1] = atof(argv[++i]);
      scene.kd[2] = atof(argv[++i]);

     } else if (strcmp(argv[i],"-ks") == 0 ) {

      if ( (i+3) >= argc ) {
	cout << "You need three arguments for -ks.\n";
	exit(-1);
      }

      scene.ks[0] = atof(argv[++i]);
      scene.ks[1] = atof(argv[++i]);
      scene.ks[2] = atof(argv[++i]);

    } else if (strcmp(argv[i],"-sp") == 0 ) {

      if ( (i+1) >= argc ) {
	cout << "You need one argument for -sp.\n";
	exit(-1);
      }

      scene.sp = atof(argv[++i]);
    } else if (strcmp(argv[i],"-pl") == 0 ) {

      if ( (i+6) >= argc ) {
	cout << "You need six arguments for -pl.\n";
	exit(-1);
      }

      if ( scene.numPL >= 5 ) {
	cout << "Only five PL allowed!\n";
	exit(-1);
      }
      scene.pL[scene.numPL].pos[0] = atof(argv[++i]);
      scene.pL[scene.numPL].pos[1] = atof(argv[++i]);
      scene.pL[scene.numPL].pos[2] = atof(argv[++i]);
      scene.pL[scene.numPL].col[0] = atof(argv[++i]);
      scene.pL[scene.numPL].col[1] = atof(argv[++i]);
      scene.pL[scene.numPL].col[2] = atof(argv[++i]);
      scene.numPL++;


    } else if (strcmp(argv[i],"-dl") == 0 ) {

      if ( (i+6) >= argc ) {
	cout << "You need six arguments for -dl.\n";
	exit(-1);
      }

      if ( scene.numDL >= 5 ) {
	cout << "Only five DL allowed!\n";
	exit(-1);
      }
      scene.numDL++;
      scene.dL[scene.numDL].pos[0] = atof(argv[++i]);
      scene.dL[scene.numDL].pos[1] = atof(argv[++i]);
      scene.dL[scene.numDL].pos[2] = atof(argv[++i]);
      scene.dL[scene.numDL].col[0] = atof(argv[++i]);
      scene.dL[scene.numDL].col[1] = atof(argv[++i]);
      scene.dL[scene.numDL].col[2] = atof(argv[++i]);
      scene.numDL++;
    } else {
      cout << "I don't know what \"" << argv[i] << "\" means!\n";
      exit(-1);
    }
  }
  
  cout << "ka = " << scene.ka << "\n";
  cout << "kd = " << scene.kd << "\n";
  cout << "ks = " << scene.ks << "\n";
  cout << "ks = " << scene.ks << "\n";
  cout << "sp = " << scene.sp << "\n";

  for (i=0;i<scene.numPL;i++) {
    cout << "pl " << i << " " << scene.pL[i].pos << " " << scene.pL[i].col << "\n";
  }

  for (i=0;i<scene.numDL;i++) {
    cout << "dl " << i << " " << scene.dL[i].pos << " " << scene.dL[i].col << "\n";
  }



}

// ===================================================
// the usual stuff, nothing exciting here
// ===================================================
int main(int argc, char *argv[]) {

  parse_arguments(argc, argv);

  //This initializes glfw
  initializeRendering();

    
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
  glOrtho(0, Width_global, 0, Height_global, 1, -1);
    
  glfwSetWindowTitle(window, "CS184");
  glfwSetWindowSizeCallback(window, size_callback);
  glfwSetKeyCallback(window, key_callback);
    
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








