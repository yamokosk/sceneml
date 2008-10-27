#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <vector>

namespace TinySG
{

const int VERTEX_FEATURE = 200;
const int EDGE_FEATURE = 201;
const int FACE_FEATURE = 202;

const int LEFT_FACE = 300;
const int RIGHT_FACE = 301;

const int SEGMENT_INSIDE = 400;
const int SEGMENT_HEAD = 401;
const int SEGMENT_TAIL = 402;


class Feature
{
public:
	enum {
		Vertex = 0,
		Edge,
		Face,
		LeftFace,
		RightFace,
		SegmentInside,
		SegmentHead,
		SegmentTail
	} FeatureTypes;
	Feature(int type) type_(type) {};
private:
	const int type_;
};


//foward declarations
class EDGE;
class FACE;
class POLYHEDRON;
class RigidBody;

class VERTEX3D:public FEATURE{
public:
	int index;
	double x,y,z;
	int adjacency_size;
	int adjacency_max;
	EDGE** adjacency;

	VERTEX3D();
	VERTEX3D(double x, double y, double z);
	~VERTEX3D();
	int addToAdjacency(EDGE* v);
	void transform(double* model_matrix, double& tx, double& ty, double& tz);
	void transform(double* t, VERTEX3D& v);
	void rotate(double* model_matrix, double& tx, double& ty, double& tz);
	void rotate(double* model_matrix, VERTEX3D& v);
	void translate(double* model_matrix, double& tx, double& ty, double& tz);
	double magSquared();
	double mag();
	double dot(VERTEX3D v);
	void normalize();
	VERTEX3D cross(VERTEX3D v);

	VERTEX3D operator-(VERTEX3D v){
		return VERTEX3D(x - v.x, y - v.y, z- v.z );
	}
	VERTEX3D operator+(VERTEX3D v){
		return VERTEX3D(x + v.x, y + v.y, z + v.z );
	}
	VERTEX3D operator+(double s){
		return VERTEX3D(x + s, y + s, z + s);
	}
	VERTEX3D operator+=(VERTEX3D v){
		x+=v.x;
		y+=v.y;
		z+=v.z;
		return *this;
	}
	VERTEX3D operator*(double s){
		return VERTEX3D(x*s, y*s, z*s);
	}
	VERTEX3D operator/(double s){
		return VERTEX3D(x/s, y/s, z/s);
	}
	VERTEX3D operator=(VERTEX3D v){
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
};

class EDGE: public FEATURE{
public:
	int index;
	VERTEX3D* head;
	VERTEX3D* tail;
	FACE* right_face;
	FACE* left_face;

	EDGE(VERTEX3D* v1, VERTEX3D* v2);
	~EDGE();
	void setCoboundary(FACE* lf, FACE* rf);
};

class FACE: public FEATURE{
public:
	int index;
	int vertex_count;

	int idc_edge;
	VERTEX3D n;
	VERTEX3D d;
	EDGE** edges;
	VERTEX3D** vertices;
	POLYHEDRON* mesh;

	FACE(VERTEX3D* v1,VERTEX3D* v2,VERTEX3D* v3,EDGE* e1,EDGE* e2,EDGE* e3);
	FACE(VERTEX3D* v1,VERTEX3D* v2,VERTEX3D* v3,VERTEX3D* v4,EDGE* e1,EDGE* e2,EDGE* e3, EDGE* e4);
	~FACE();
	int getVertexIndex(VERTEX3D* v);
	int getEdgeSide(EDGE* e);
};

class POLYHEDRON{
public:
	int vertex_count, edge_count, face_count;
	VERTEX3D** vertices;
	EDGE** edges;
	FACE** faces;
	POLYHEDRON();
	~POLYHEDRON();
	int addEdge(EDGE* e);
};

} // End namespace
#endif
