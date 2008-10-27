#include "Primitive.h"

/***************
     FEATURE
****************/

FEATURE::FEATURE(){
	this->id = -1;
}

FEATURE::FEATURE(int id){
	this->id = id;
}


/***************
    Vertex
****************/
Vertex::Vertex();
Vertex::Vertex(const Real x, const Real y, const Real z);
Vertex::Vertex(const Real coord[3]);
Vertex::Vertex(const Real* coord);
Vertex::~Vertex();

// Operators
Real Vertex::operator[] (const unsigned int i) const;
Real& Vertex::operator[] (const unsigned int i);
Real* Vertex::ptr ();
//! Pointer accessor for direct copying.
const Real* Vertex::ptr () const;
//! Pointer accessor for direct copying.
Vertex& Vertex::operator= (const Vector3 &rkVector);
//! Assigns the value of the other vector.
Vertex& Vertex::operator= (const Real fScaler);
bool Vertex::operator== (const Vector3 &rkVector) const;
bool Vertex::operator!= (const Vector3 &rkVector) const;
Vertex Vertex::operator+ (const Vector3 &rkVector) const;
Vertex Vertex::operator- (const Vector3 &rkVector) const;
Vertex Vertex::operator * (const Real fScalar) const;
Vertex Vertex::operator * (const Vector3 &rhs) const;
Vertex Vertex::operator/ (const Real fScalar) const;
Vertex Vertex::operator/ (const Vector3 &rhs) const;
const Vector3D& Vertex::operator+ () const;
Vertex Vertex::operator- () const;
Vertex& Vertex::operator+= (const Vector3 &rkVector);
Vertex& Vertex::operator+= (const Real fScalar);
Vertex& Vertex::operator-= (const Vector3 &rkVector);
Vertex& Vertex::operator-= (const Real fScalar);
Vertex& Vertex::operator *= (const Real fScalar);
Vertex& Vertex::operator *= (const Vector3 &rkVector);
Vertex& Vertex::operator/= (const Real fScalar);
Vertex& Vertex::operator/= (const Vector3 &rkVector);

Vertex::Vertex():FEATURE(VERTEX_FEATURE){
	x = y = z = 0.0;
	adjacency_size = 0;
	adjacency_max = 4;
}

Vertex::Vertex(double x, double y, double z):FEATURE(VERTEX_FEATURE){
	this->x = x;
	this->y = y;
	this->z = z;
	adjacency_size = 0;
	adjacency_max = 4;
}

int Vertex::addToAdjacency(EDGE* e){
	if(!adjacency_size){
		adjacency = new EDGE*[adjacency_max];
	}
	else{
		for(int i=0;i<adjacency_size;i++){
			if(adjacency[i]->index == e->index){
				return 1;
			}
		}
		if(adjacency_size == adjacency_max){
			adjacency_max *= 2;
			EDGE** temp = new EDGE*[adjacency_max];
			for(int i=0;i<adjacency_size;i++){
				temp[i] = adjacency[i];
			}
			delete [] adjacency;
			adjacency = temp;
		}
	}
	adjacency[adjacency_size++] = e;
	return 0;
}

double Vertex::dot(Vertex v){
	return x*v.x + y*v.y + z*v.z;
}

Vertex Vertex::cross(Vertex v){
	return Vertex(y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x);
}

double Vertex::magSquared(){
	return (x*x + y*y + z*z);
}

double Vertex::mag(){
	return sqrt(x*x + y*y + z*z);
}

void Vertex::normalize(){
	double m = this->mag();
	if (m == 0.000){
		x = y = z = 0.000;
	}
	else{
		x /= m;
		y /= m;
		z /= m;
	}
}

void Vertex::transform(double* t, double &tx, double &ty, double &tz){
	tx = x*t[0] + y*t[4] + z*t[8] + t[12];
	ty = x*t[1] + y*t[5] + z*t[9] + t[13];
	tz = x*t[2] + y*t[6] + z*t[10] + t[14];
}

void Vertex::transform(double* t, Vertex& v){
	v.x = x*t[0] + y*t[4] + z*t[8] + t[12];
	v.y = x*t[1] + y*t[5] + z*t[9] + t[13];
	v.z = x*t[2] + y*t[6] + z*t[10] + t[14];
}


void Vertex::rotate(double* t, Vertex& v){
	v.x = x*t[0] + y*t[4] + z*t[8];
	v.y = x*t[1] + y*t[5] + z*t[9];
	v.z = x*t[2] + y*t[6] + z*t[10];
}

void Vertex::translate(double* t, double &tx, double &ty, double &tz){
	tx += t[12];
	ty += t[13];
	tz += t[14];
}




/***************
      EDGE
****************/
EDGE::EDGE(Vertex* v1, Vertex* v2):FEATURE(EDGE_FEATURE){
	this->head = v1;
	this->tail = v2;
	left_face = 0;
	right_face =0;
}

EDGE::~EDGE(){}

void EDGE::setCoboundary(FACE* lf, FACE* rf){
	this->left_face = lf;
	this->right_face= rf;
}


/***************
      FACE
****************/
FACE::FACE(Vertex* v1,Vertex* v2,Vertex* v3,Vertex* v4,EDGE* e1,EDGE* e2,EDGE* e3,EDGE* e4):FEATURE(FACE_FEATURE){
	this->vertex_count = 4;
	this->vertices = new Vertex*[this->vertex_count];
	this->edges = new EDGE*[this->vertex_count];
	this->idc_edge = 0;
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;
	vertices[3] = v4;
	edges[0] = e1;
	edges[1] = e2;
	edges[2] = e3;
	edges[3] = e4;

	n = (*v2-*v1).cross(*v4-*v1);
	n.normalize();
	d.x = (v1->x + v2->x + v3->x + v4->x)/4.0;
	d.y = (v1->y + v2->y + v3->y + v4->y)/4.0;
	d.z = (v1->z + v2->z + v3->z + v4->z)/4.0;

	for(int j=0;j<vertex_count;j++){

		int h = getVertexIndex(edges[j]->head)+1;
		int t = getVertexIndex(edges[j]->tail)+1;

		if(edges[j]->left_face){
			edges[j]->right_face = this;
		}
		else{
			edges[j]->left_face = this;
		}
	}
}

FACE::FACE(Vertex* v1,Vertex* v2,Vertex* v3,EDGE* e1,EDGE* e2,EDGE* e3):FEATURE(FACE_FEATURE){
	this->vertex_count = 3;
	this->vertices = new Vertex*[this->vertex_count];
	this->edges = new EDGE*[this->vertex_count];
	this->idc_edge = 0;
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;
	edges[0] = e1;
	edges[1] = e2;
	edges[2] = e3;

	n = (*v2-*v1).cross(*v3-*v1);
	n.normalize();
	d.x = (v1->x + v2->x + v3->x)/3.0;
	d.y = (v1->y + v2->y + v3->y)/3.0;
	d.z = (v1->z + v2->z + v3->z)/3.0;

	for(int j=0;j<vertex_count;j++){
		int h = getVertexIndex(edges[j]->head)+1;
		int t = getVertexIndex(edges[j]->tail)+1;

		if(edges[j]->left_face){
			edges[j]->right_face = this;
		}
		else{
			edges[j]->left_face = this;
		}
	}
}

FACE::~FACE(){}

int FACE::getVertexIndex(Vertex* v){
	for(int i=0;i<this->vertex_count;i++){
		if(v->index == this->vertices[i]->index){
			return i;
		}
	}
	return -1;
}

int FACE::getEdgeSide(EDGE* e){
	if(e->left_face->index == this->index){
		return LEFT_FACE;
	}
	return RIGHT_FACE;

}

/***************
    POLYHEDRON
****************/
POLYHEDRON::POLYHEDRON(){
	this->vertex_count = 0;
	this->edge_count = 0;
	this->face_count = 0;
}

POLYHEDRON::~POLYHEDRON(){
	for(int i=0;i<edge_count;i++){
		delete edges[i];
	}
	for(int i=0;i<face_count;i++){
		delete faces[i];
	}

	delete [] vertices;
	delete [] edges;
	delete [] faces;
}


/***************
    Primitives
****************/
static void createBox()
{
	lx = dim(1)*0.5;
	ly = dim(2)*0.5;
	lz = dim(3)*0.5;

	// Homogeneous vertices
	hv = [-lx,  lx,  lx, -lx, -lx,  lx, lx, -lx; ...
	      -ly, -ly,  ly,  ly, -ly, -ly, ly,  ly; ...
	      -lz, -lz, -lz, -lz,  lz,  lz, lz,  lz; ...
	        1,   1,   1,   1,   1,   1,  1,   1];
	v = T*hv; % Transformed

	// Non-homogeneous vertices
	fv.vertices = v(1:3,:)';

	// Create a new patch object
	fv.faces = [1, 2, 6, 5; ...
	            2, 3, 7, 6; ...
	            3, 4, 8, 7; ...
	            4, 1, 5, 8; ...
	            1, 2, 3, 4; ...
	            5, 6, 7, 8];
}

static void createSphere()
{
  // icosahedron data for an icosahedron of radius 1.0
# define ICX 0.525731112119133606f
# define ICZ 0.850650808352039932f
  static GLfloat idata[12][3] = {
    {-ICX, 0, ICZ},
    {ICX, 0, ICZ},
    {-ICX, 0, -ICZ},
    {ICX, 0, -ICZ},
    {0, ICZ, ICX},
    {0, ICZ, -ICX},
    {0, -ICZ, ICX},
    {0, -ICZ, -ICX},
    {ICZ, ICX, 0},
    {-ICZ, ICX, 0},
    {ICZ, -ICX, 0},
    {-ICZ, -ICX, 0}
  };

  static int index[20][3] = {
    {0, 4, 1},	  {0, 9, 4},
    {9, 5, 4},	  {4, 5, 8},
    {4, 8, 1},	  {8, 10, 1},
    {8, 3, 10},   {5, 3, 8},
    {5, 2, 3},	  {2, 7, 3},
    {7, 10, 3},   {7, 6, 10},
    {7, 11, 6},   {11, 0, 6},
    {0, 1, 6},	  {6, 1, 10},
    {9, 0, 11},   {9, 11, 2},
    {9, 2, 5},	  {7, 2, 11},
  };

  static GLuint listnum = 0;
  if (listnum==0) {
    listnum = glGenLists (1);
    glNewList (listnum,GL_COMPILE);
    glBegin (GL_TRIANGLES);
    for (int i=0; i<20; i++) {
      drawPatch (&idata[index[i][2]][0],&idata[index[i][1]][0],
		 &idata[index[i][0]][0],sphere_quality);
    }
    glEnd();
    glEndList();
  }
  glCallList (listnum);
}

static void createCapsule(float l, float r)
{
  int i,j;
  float tmp,nx,ny,nz,start_nx,start_ny,a,ca,sa;
  // number of sides to the cylinder (divisible by 4):
  const int n = capped_cylinder_quality*4;

  l *= 0.5;
  a = float(M_PI*2.0)/float(n);
  sa = (float) sin(a);
  ca = (float) cos(a);

  // draw cylinder body
  ny=1; nz=0;		  // normal vector = (0,ny,nz)
  glBegin (GL_TRIANGLE_STRIP);
  for (i=0; i<=n; i++) {
    glNormal3d (ny,nz,0);
    glVertex3d (ny*r,nz*r,l);
    glNormal3d (ny,nz,0);
    glVertex3d (ny*r,nz*r,-l);
    // rotate ny,nz
    tmp = ca*ny - sa*nz;
    nz = sa*ny + ca*nz;
    ny = tmp;
  }
  glEnd();

  // draw first cylinder cap
  start_nx = 0;
  start_ny = 1;
  for (j=0; j<(n/4); j++) {
    // get start_n2 = rotated start_n
    float start_nx2 =  ca*start_nx + sa*start_ny;
    float start_ny2 = -sa*start_nx + ca*start_ny;
    // get n=start_n and n2=start_n2
    nx = start_nx; ny = start_ny; nz = 0;
    float nx2 = start_nx2, ny2 = start_ny2, nz2 = 0;
    glBegin (GL_TRIANGLE_STRIP);
    for (i=0; i<=n; i++) {
      glNormal3d (ny2,nz2,nx2);
      glVertex3d (ny2*r,nz2*r,l+nx2*r);
      glNormal3d (ny,nz,nx);
      glVertex3d (ny*r,nz*r,l+nx*r);
      // rotate n,n2
      tmp = ca*ny - sa*nz;
      nz = sa*ny + ca*nz;
      ny = tmp;
      tmp = ca*ny2- sa*nz2;
      nz2 = sa*ny2 + ca*nz2;
      ny2 = tmp;
    }
    glEnd();
    start_nx = start_nx2;
    start_ny = start_ny2;
  }

  // draw second cylinder cap
  start_nx = 0;
  start_ny = 1;
  for (j=0; j<(n/4); j++) {
    // get start_n2 = rotated start_n
    float start_nx2 = ca*start_nx - sa*start_ny;
    float start_ny2 = sa*start_nx + ca*start_ny;
    // get n=start_n and n2=start_n2
    nx = start_nx; ny = start_ny; nz = 0;
    float nx2 = start_nx2, ny2 = start_ny2, nz2 = 0;
    glBegin (GL_TRIANGLE_STRIP);
    for (i=0; i<=n; i++) {
      glNormal3d (ny,nz,nx);
      glVertex3d (ny*r,nz*r,-l+nx*r);
      glNormal3d (ny2,nz2,nx2);
      glVertex3d (ny2*r,nz2*r,-l+nx2*r);
      // rotate n,n2
      tmp = ca*ny - sa*nz;
      nz = sa*ny + ca*nz;
      ny = tmp;
      tmp = ca*ny2- sa*nz2;
      nz2 = sa*ny2 + ca*nz2;
      ny2 = tmp;
    }
    glEnd();
    start_nx = start_nx2;
    start_ny = start_ny2;
  }

  glPopMatrix();
}


// draw a cylinder of length l and radius r, aligned along the z axis
static void createCylinder (float l, float r, float zoffset)
{
  int i;
  float tmp,ny,nz,a,ca,sa;
  const int n = 24;	// number of sides to the cylinder (divisible by 4)

  l *= 0.5;
  a = float(M_PI*2.0)/float(n);
  sa = (float) sin(a);
  ca = (float) cos(a);

  // draw cylinder body
  ny=1; nz=0;		  // normal vector = (0,ny,nz)
  glBegin (GL_TRIANGLE_STRIP);
  for (i=0; i<=n; i++) {
    glNormal3d (ny,nz,0);
    glVertex3d (ny*r,nz*r,l+zoffset);
    glNormal3d (ny,nz,0);
    glVertex3d (ny*r,nz*r,-l+zoffset);
    // rotate ny,nz
    tmp = ca*ny - sa*nz;
    nz = sa*ny + ca*nz;
    ny = tmp;
  }
  glEnd();

  // draw top cap
  glShadeModel (GL_FLAT);
  ny=1; nz=0;		  // normal vector = (0,ny,nz)
  glBegin (GL_TRIANGLE_FAN);
  glNormal3d (0,0,1);
  glVertex3d (0,0,l+zoffset);
  for (i=0; i<=n; i++) {
    if (i==1 || i==n/2+1)
      setColor (color[0]*0.75f,color[1]*0.75f,color[2]*0.75f,color[3]);
    glNormal3d (0,0,1);
    glVertex3d (ny*r,nz*r,l+zoffset);
    if (i==1 || i==n/2+1)
      setColor (color[0],color[1],color[2],color[3]);

    // rotate ny,nz
    tmp = ca*ny - sa*nz;
    nz = sa*ny + ca*nz;
    ny = tmp;
  }
  glEnd();

  // draw bottom cap
  ny=1; nz=0;		  // normal vector = (0,ny,nz)
  glBegin (GL_TRIANGLE_FAN);
  glNormal3d (0,0,-1);
  glVertex3d (0,0,-l+zoffset);
  for (i=0; i<=n; i++) {
    if (i==1 || i==n/2+1)
      setColor (color[0]*0.75f,color[1]*0.75f,color[2]*0.75f,color[3]);
    glNormal3d (0,0,-1);
    glVertex3d (ny*r,nz*r,-l+zoffset);
    if (i==1 || i==n/2+1)
      setColor (color[0],color[1],color[2],color[3]);

    // rotate ny,nz
    tmp = ca*ny + sa*nz;
    nz = -sa*ny + ca*nz;
    ny = tmp;
  }
  glEnd();
}



