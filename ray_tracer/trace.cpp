#include "trace.H"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <getopt.h>
#ifdef __APPLE__
#define MAX std::numeric_limits<double>::max()
#else
#include <values.h>
#define MAX DBL_MAX
#endif



bool Triangle::intersect(const Ray &r, double t0, double t1, HitRecord &hr) const {
  // If r hits the triangle then 
  // set hr.t to the t value of the hit
  // set hr.p to the location of the hit (point in space)
  // set hr.n to the normal of the triangle
  // set hr.alpha, beta, gamma to the barycentric coordinates of the triangle.
  // return true
  //
  // If it's a miss just return false.

  // <<>>

}

bool TrianglePatch::intersect(const Ray &r, double t0, double t1, HitRecord &hr) const {
  // Same as Triangle::intersect but when hit the normal should
  // be set by barycentric interpolation.

  // <<>>

}


bool Sphere::intersect(const Ray &r, double t0, double t1, HitRecord &hr) const {
  // If r misses sphere, return false.
  //
  // If r hits, then 
  // set hr.t to the t value of the hit
  // set hr.p to the location of the hit (point in space)
  // set hr.n to the normal of the sphere at hr.p
  // return true

  // <<>>

}






Tracer::Tracer(const std::string &fname) {
  std::ifstream in(fname.c_str(), std::ios_base::in);
  std::string line;
  char ch;
  Fill fill;
  bool coloredlights = false;
  while (in) {
    getline(in, line);
    switch (line[0]) {
    case 'b': {
      std::stringstream ss(line);
      ss>>ch>>bcolor[0]>>bcolor[1]>>bcolor[2];
      break;
    }

    case 'v': {
      getline(in, line);
      std::string junk;
      std::stringstream fromss(line);
      fromss>>junk>>eye[0]>>eye[1]>>eye[2];

      getline(in, line);
      std::stringstream atss(line);
      atss>>junk>>at[0]>>at[1]>>at[2];

      getline(in, line);
      std::stringstream upss(line);
      upss>>junk>>up[0]>>up[1]>>up[2];

      getline(in, line);
      std::stringstream angless(line);
      angless>>junk>>angle;

      getline(in, line);
      std::stringstream hitherss(line);
      hitherss>>junk>>hither;

      getline(in, line);
      std::stringstream resolutionss(line);
      resolutionss>>junk>>res[0]>>res[1];
      break;
    }

    case 'p': {
      bool patch = false;
      std::stringstream ssn(line);
      unsigned int nverts;
      if (line[1] == 'p') {
	patch = true;
	ssn>>ch;
      }
      ssn>>ch>>nverts;
      std::vector<SlVector3> vertices;
      std::vector<SlVector3> normals;
      for (unsigned int i=0; i<nverts; i++) {
	getline(in, line);
	std::stringstream ss(line);
	SlVector3 v,n;
	if (patch) ss>>v[0]>>v[1]>>v[2]>>n[0]>>n[1]>>n[2];
	else ss>>v[0]>>v[1]>>v[2];
	vertices.push_back(v);
	normals.push_back(n);
      }
      bool makeTriangles = false;
      if (vertices.size() == 3) {
	if (patch) {
	  surfaces.push_back(std::pair<Surface *, Fill>(new TrianglePatch(vertices[0], vertices[1], vertices[2], 
									  normals [0], normals [1], normals [2]), fill));
	} else {
	  surfaces.push_back(std::pair<Surface *, Fill>(new Triangle(vertices[0], vertices[1], vertices[2]), fill));
	}
      } else if (vertices.size() == 4) {
	SlVector3 n0 = cross(vertices[1] - vertices[0], vertices[2] - vertices[0]);
	SlVector3 n1 = cross(vertices[2] - vertices[1], vertices[3] - vertices[1]);
	SlVector3 n2 = cross(vertices[3] - vertices[2], vertices[0] - vertices[2]);
	SlVector3 n3 = cross(vertices[0] - vertices[3], vertices[1] - vertices[3]);
	if (dot(n0,n1) > 0 && dot(n0,n2) > 0 && dot(n0,n3) > 0) {
	  makeTriangles = true;
	  if (patch) {
	    surfaces.push_back(std::pair<Surface *, Fill>(new TrianglePatch(vertices[0], vertices[1], vertices[2], 
									    normals[0], normals[1], normals[2]), fill));
	    surfaces.push_back(std::pair<Surface *, Fill>(new TrianglePatch(vertices[0], vertices[2], vertices[3], 
									    normals[0], normals[2], normals[3]), fill));
	  } else {
	    surfaces.push_back(std::pair<Surface *, Fill>(new Triangle(vertices[0], vertices[1], vertices[2]), fill));
	    surfaces.push_back(std::pair<Surface *, Fill>(new Triangle(vertices[0], vertices[2], vertices[3]), fill));
	  }
	}
	if (!makeTriangles) {
	  std::cerr << "I didn't make triangles.  Poly not flat or more than quad.\n";
	}
      }
      break;
    }

    case 's' : {
      std::stringstream ss(line);
      SlVector3 c;
      double r;
      ss>>ch>>c[0]>>c[1]>>c[2]>>r;
      surfaces.push_back(std::pair<Surface *, Fill>(new Sphere(c,r), fill));
      break;
    }
	  
    case 'f' : {
      std::stringstream ss(line);
      ss>>ch>>fill.color[0]>>fill.color[1]>>fill.color[2]>>fill.kd>>fill.ks>>fill.shine>>fill.t>>fill.ior;
      break;
    }

    case 'l' : {
      std::stringstream ss(line);
      Light l;
      ss>>ch>>l.p[0]>>l.p[1]>>l.p[2];
      if (!ss.eof()) {
	ss>>l.c[0]>>l.c[1]>>l.c[2];
	coloredlights = true;
      }
      lights.push_back(l);
      break;
    }

    default:
      break;
    }
  }
  if (!coloredlights) for (unsigned int i=0; i<lights.size(); i++) lights[i].c = 1.0/sqrt(lights.size());
  im = new SlVector3[res[0]*res[1]];
  shadowbias = 1e-6;
  samples = 1;
  aperture = 0.0;
}

Tracer::~Tracer() {
  if (im) delete [] im;
  for (unsigned int i=0; i<surfaces.size(); i++) delete surfaces[i].first;
}




SlVector3 Tracer::shade(const HitRecord &hr) const {
  if (color) return hr.f.color;

  // For now always return just the plain color
  // <<>>
  
  return hr.f.color;
}




SlVector3 Tracer::trace(const Ray &r, double t0, double t1) const {
  HitRecord hr;
  SlVector3 color(bcolor);
  
  bool hit = false;
  for (unsigned int k=0; k<surfaces.size(); k++) {
    const std::pair<Surface *, Fill> &s  = surfaces[k];
    if (s.first->intersect(r, t0, t1, hr)) {
      t1 = hr.t;
      hr.f = s.second;
      hr.raydepth = r.depth;
      hr.v = r.e - hr.p;
      normalize(hr.v);
      normalize(hr.n);
      hit = true;
    }
  }

  if (hit) color = shade(hr);
  return color;
}




void Tracer::traceImage() {
  // set up coordinate system
  SlVector3 w = eye - at;
  w /= mag(w);
  SlVector3 u = cross(up,w);
  normalize(u);
  SlVector3 v = cross(w,u);
  normalize(v);

  double d = mag(eye - at);
  double h = tan((M_PI/180.0) * (angle/2.0)) * d;
  double l = -h;
  double r = h;
  double b = h;
  double t = -h;

  SlVector3 *pixel = im;

  for (unsigned int j=0; j<res[1]; j++) {
    for (unsigned int i=0; i<res[0]; i++, pixel++) {
      double x = l + (r-l)*(i+0.5)/res[0];
      double y = b + (t-b)*(j+0.5)/res[1];
      SlVector3 dir = -d * w + x * u + y * v;

      Ray r(eye, dir);
      normalize(r.d);
      (*pixel) = trace(r, hither, MAX);
    }
  }
}


void Tracer::writeImage(const std::string &fname) {
#ifdef __APPLE__
  std::ofstream out(fname, std::ios::out | std::ios::binary);
#else
  std::ofstream out(fname.c_str(), std::ios_base::binary);
#endif
  out<<"P6"<<"\n"<<res[0]<<" "<<res[1]<<"\n"<<255<<"\n";
  SlVector3 *pixel = im;
  char val;
  for (unsigned int i=0; i<res[0]*res[1]; i++, pixel++) {
    val = (unsigned char)(std::min(1.0, std::max(0.0, (*pixel)[0])) * 255.0);
    out.write (&val, sizeof(unsigned char));
    val = (unsigned char)(std::min(1.0, std::max(0.0, (*pixel)[1])) * 255.0);
    out.write (&val, sizeof(unsigned char));
    val = (unsigned char)(std::min(1.0, std::max(0.0, (*pixel)[2])) * 255.0);
    out.write (&val, sizeof(unsigned char));
  }
  out.close();
}


int main(int argc, char *argv[]) {
  int c;
  double aperture = 0.0;
  int samples = 1;
  int maxraydepth = 5;
  bool color = false;
  while ((c = getopt(argc, argv, "a:s:d:c")) != -1) {
    switch(c) {
    case 'a':
      aperture = atof(optarg);
      // <<>>
      std::cerr << "Waring: Feature -a not implemented.\n";
      break;
    case 's':
      samples = atoi(optarg);
      // <<>>
      std::cerr << "Waring: Feature -s not implemented.\n";
      break;
    case 'c':
      color = true;
      break;
    case 'd':
      maxraydepth = atoi(optarg);
      break;
    default:
      abort();
    }
  }

  if (argc-optind != 2) {
    std::cout<<"usage: trace [opts] input.nff output.ppm"<<std::endl;
    for (unsigned int i=0; i<argc; i++) std::cout<<argv[i]<<std::endl;
    exit(0);
  }	

  Tracer tracer(argv[optind++]);
  tracer.aperture = aperture;
  tracer.samples = samples;
  tracer.color = color;
  tracer.maxraydepth = maxraydepth;
  tracer.traceImage();
  tracer.writeImage(argv[optind++]);
};
