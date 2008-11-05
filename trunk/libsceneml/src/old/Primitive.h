#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <vector>

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
	int getFeatureType() {return type_;}
private:
	const int type_;
};


//foward declarations
class Vertex;
class Edge;
class Face;
class Polyhedron;

class Vertex : public Feature
{
	unsigned int index_;
	ColumnVector coord_;

public:
	inline Vertex() :
		Feature(Feature::Vertex),
		index_(0)
	{
		memset(coord_, 0.0, 3*sizeof(Real));
	}
	inline Vertex(const Real x, const Real y, const Real z) :
		Feature(Feature::Vertex),
		index_(0)
	{
		coord_[0] = x;
		coord_[1] = y;
		coord_[2] = z;
	}
	inline explicit Vertex(const Real coord[3]) :
		Feature(Feature::Vertex),
		index_(0)
	{
		memcpy(coord_, coord, 3*sizeof(Real));
	}
	inline explicit Vertex(const Real* coord) :
		Feature(Feature::Vertex),
		index_(0)
	{
		memcpy(coord_, coord, 3*sizeof(Real));
	}
	~Vertex() {};

	// Operators
	inline Real operator[] (const size_t i) const
	{
		assert(i<3); return *(coord_ + i);
	}
	inline Real& operator[] (const unsigned int i)
	{
		assert(i<3); return *(coord_ + i);
	}
	inline Real x(void) const {return coord[0];}
	inline Real y(void) const {return coord[1];}
	inline Real z(void) const {return coord[2];}
	inline Real& x(void) {return coord[0];}
	inline Real& y(void) {return coord[1];}
	inline Real& z(void) {return coord[2];}
	inline Real* ptr () { return coord_; }
	//! Pointer accessor for direct copying.
	inline const Real* ptr () const { return coord_; }
	//! Pointer accessor for direct copying.
	inline Vertex& operator= (const Vertex &rkVector)
	{
		memcpy(ptr(), rkVector.ptr(), 3*sizeof(Real));
		return *this;
	}
	//! Assigns the value of the other vector.
	inline Vertex& operator= (const Real fScaler)
	{
		memset(ptr(), fScaler, 3*sizeof(Real));
		return *this;
	}
	inline bool operator== (const Vertex &rkVector) const
	{
		return ( x() == rkVector.x() && y() == rkVector.y() && z() == rkVector.z() );
	}
	inline bool operator!= (const Vertex &rkVector) const
	{
		return ( x() != rkVector.x() || y() != rkVector.y() || z() != rkVector.z() );
	}
	// arithmetic operations
	inline Vertex operator+ (const Vertex &rkVector) const
	{
		return Vertex(x() + rkVector.x(), y() + rkVector.y(), z() + rkVector.z());
	}
	inline Vertex operator- (const Vertex &rkVector) const
	{
		return Vertex(x() - rkVector.x(), y() - rkVector.y(), z() - rkVector.z());
	}
	inline Vertex operator * (const Real fScalar) const
	{
		return Vertex(x() * fScalar, y() * fScalar, z() * fScalar);
	}
	inline Vertex operator * (const Vertex &rhs) const
	{
		return Vertex(x() * rkVector.x(), y() * rkVector.y(), z() * rkVector.z());
	}
	inline Vertex operator/ (const Real fScalar) const
	{
		assert( fScalar != 0.0 );
		Real fInv = 1.0 / fScalar;
		return Vertex(x() * fInv, y() * fInv, z() * fInv);
	}
	inline Vertex operator/ (const Vertex &rhs) const
	{
		assert( (rhs.x() != 0.0) || (rhs.y() != 0.0) || (rhs.z() != 0.0) );
		return Vertex(x()/rhs.x(), y()/rhs.y(), z()/rhs.z());
	}
	inline const Vector3D& operator+ () const
	{
		return *this;
	}
	inline Vertex operator- () const
	{
		return Vertex(-x(), -y(), -z());
	}
	// overloaded operators to help Vertex
	inline friend Vertex operator * ( const Real fScalar, const Vertex& rkVector )
	{
		return Vertex(
			fScalar * rkVector.x,
			fScalar * rkVector.y,
			fScalar * rkVector.z);
	}
	inline friend Vertex operator / ( const Real fScalar, const Vertex& rkVector )
	{
		return Vertex(
			fScalar / rkVector.x,
			fScalar / rkVector.y,
			fScalar / rkVector.z);
	}
	inline friend Vertex operator + (const Vertex& lhs, const Real rhs)
	{
		return Vertex(
			lhs.x + rhs,
			lhs.y + rhs,
			lhs.z + rhs);
	}
	inline friend Vertex operator + (const Real lhs, const Vertex& rhs)
	{
		return Vertex(
			lhs + rhs.x,
			lhs + rhs.y,
			lhs + rhs.z);
	}
	inline friend Vertex operator - (const Vertex& lhs, const Real rhs)
	{
		return Vertex(
			lhs.x - rhs,
			lhs.y - rhs,
			lhs.z - rhs);
	}

	inline friend Vertex operator - (const Real lhs, const Vertex& rhs)
	{
		return Vertex(
			lhs - rhs.x,
			lhs - rhs.y,
			lhs - rhs.z);
	}

	// arithmetic updates
	inline Vertex& operator+= (const Vertex &rkVector)
	{
		x() += rkVector.x();
		y() += rkVector.y();
		z() += rkVector.z();
		return *this;
	}
	inline Vertex& operator+= (const Real fScalar)
	{
		x() += fScalar;
		y() += fScalar;
		z() += fScalar;
		return *this;
	}
	inline Vertex& operator-= (const Vertex &rkVector)
	{
		x() -= rkVector.x();
		y() -= rkVector.y();
		z() -= rkVector.z();
		return *this;
	}
	inline Vertex& operator-= (const Real fScalar)
	{
		x() -= fScalar;
		y() -= fScalar;
		z() -= fScalar;
		return *this;
	}
	inline Vertex& operator *= (const Real fScalar)
	{
		x() *= fScalar;
		y() *= fScalar;
		z() *= fScalar;
		return *this;
	}
	inline Vertex& operator *= (const Vertex &rkVector)
	{
		x() *= rkVector.x();
		y() *= rkVector.y();
		z() *= rkVector.z();
		return *this;
	}
	inline Vertex& operator/= (const Real fScalar)
	{
		assert( fScalar != 0.0 );
		Real fInv = 1.0 / fScalar;

		x() *= fInv;
		y() *= fInv;
		z() *= fInv;
		return *this;
	}
	inline Vertex& operator/= (const Vertex &rkVector)
	{
		assert( (rhs.x() != 0.0) || (rhs.y() != 0.0) || (rhs.z() != 0.0) );
		x() /= rkVector.x();
		y() /= rkVector.y();
		z() /= rkVector.z();
		return *this;
	}

	// More math functions
	inline Real dotProduct(const Vertex& vec) const
	{
		return x() * vec.x() + y() * vec.y() + z() * vec.z();
	}
	inline Real length () const
	{
		return Math::Sqrt( x * x + y * y + z * z );
	}
	inline Real squaredLength () const
	{
		return x * x + y * y + z * z;
	}
	inline Real normalise()
	{
		Real fLength = Math::Sqrt( x * x + y * y + z * z );
		// Will also work for zero-sized vectors, but will change nothing
		if ( fLength > 1e-08 )
		{
			Real fInvLength = 1.0 / fLength;
			x *= fInvLength;
			y *= fInvLength;
			z *= fInvLength;
		}
		return fLength;
	}
	inline Vertex crossProduct( const Vertex& rkVector ) const
	{
		return Vertex( y() * rkVector.z() - z() * rkVector.y(),
					   z() * rkVector.x() - x() * rkVector.z(),
					   x() * rkVector.y() - y() * rkVector.x());
	}

	void transform(double* model_matrix, double& tx, double& ty, double& tz);
	void transform(double* t, Vertex& v);
	void rotate(double* model_matrix, double& tx, double& ty, double& tz);
	void rotate(double* model_matrix, Vertex& v);
	void translate(double* model_matrix, double& tx, double& ty, double& tz);

	Vertex closestPoint(const Edge&, Vertex* pt = NULL);
	Vertex closestPoint(const Face&, Vertex* pt = NULL);
};

class Edge
{
public:
	inline Edge(const Vertex& head, const Vertex& tail) :
		head_(head), tail_(tail)
	{}
	inline Vertex head() {return head_;}
	inline Vertex tail() {return tail_;}
	inline Vertex& head() {return head_;}
	inline Vertex& tail() {return tail_;}

	Edge closestDistance(const Edge&);

	inline Real length () const
	{
		return (head_ - tail_).length();
	}
private:
	Vertex head_, tail_;
};

class Face
{
	typedef std::vector<Vertex> VertexList;
	typedef std::pair<Vertex, Vertex> Edge;
	typedef std::multimap <Vertex, Vertex> EdgeMap;
public:
 	Face() :
 		normal_( 0.0, 0.0, 0.0 ),
 		isNormalSet_(false)
 	{
 		// reserve space for 4 vertices to reduce allocation cost
 		vertexList_.reserve(4);
 	}
 	~Face();
 	Face(const Face &cpy)
 	{
 		vertexList_ = cpy.vertexList_;
		normal_ = cpy.normal_;
		isNormalSet_ = cpy.isNormalSet_;
 	}

 	void insertVertex (const Vertex &vdata, size_t vertexIndex)
 	{
 		VertexList::iterator it = vertexList_.begin();

		std::advance(it, vertex);
		vertexList_.insert(it, vdata);
 	}
 	void insertVertex (const Vertex &vdata)
 	{
 		vertexList_.push_back(vdata);
 	}
 	const Vertex& getVertex (size_t vertex) const
 	{
 		return vertexList_[vertex];
 	}
	void setVertex (const Vertex &vdata, size_t vertexIndex)
	{
		vertexList_[ vertex ] = vdata;
	}
	size_t getVertexCount (void) const
	{
		return vertexList_.size();
	}
	const Vertex& getNormal (void)
	{
		updateNormal();

		return normal_;
	}
	void deleteVertex (size_t vertex)
	{
		VertexList::iterator it = vertexList_.begin();
		std::advance(it, vertex);

		vertexList_.erase( it );
	}
	void storeEdges (EdgeMap *edgeMap) const
	{
		size_t vertexCount = getVertexCount();

		for ( size_t i = 0; i < vertexCount; ++i )
		{
			edgeMap->insert( Edge( getVertex( i ), getVertex( ( i + 1 ) % vertexCount ) ) );
		}
	}
	void reset (void)
	{
		// could use swap() to free memory here, but assume most may be reused so avoid realloc
		vertexList_.clear();

		isNormalSet_ = false;
	}
	int getVertexIndex(Vertex* v);
	int getEdgeSide(Edge* e);

	Real closestDistance(const Face*, Edge* pt = NULL);

protected:
	void Face::updateNormal( void )
	{
		if (isNormalSet_)
			return;

		// vertex order is ccw
		const Vector3& a = getVertex( 0 );
		const Vector3& b = getVertex( 1 );
		const Vector3& c = getVertex( 2 );

		// used method: Newell
		normal_.x = 0.5f * ( (a.y - b.y) * (a.z + b.z) +
							   (b.y - c.y) * (b.z + c.z) +
							   (c.y - a.y) * (c.z + a.z));

		normal_.y = 0.5f * ( (a.z - b.z) * (a.x + b.x) +
							   (b.z - c.z) * (b.x + c.x) +
							   (c.z - a.z) * (c.x + a.x));

		normal_.z = 0.5f * ( (a.x - b.x) * (a.y + b.y) +
							   (b.x - c.x) * (b.y + c.y) +
							   (c.x - a.x) * (c.y + a.y));

		normal_.normalise();

		isNormalSet_ = true;
	}

	VertexList vertexList_;
	Vertex normal_;
	bool isNormalSet_;
};

class Polyhedron
{
public:
	typedef std::vector< Face* > FaceList;
protected:
	FaceList faces_;
public:
	Polyhedron();
	~Polyhedron();
	Polyhedron(const Polyhedron& cpy);

	size_t 	getPolygonCount (void) const {return faces_.size();}
	size_t 	getVertexCount (size_t poly) const {return faces_[ poly ]->getVertexCount();}
	const Face& getPolygon(size_t poly) const
	{
		return *faces_[poly];
	}
	const Vector3& getVertex(size_t poly, size_t vertex) const
	{
		return faces_[poly]->getVertex(vertex);
	}
	const Vector3 & getNormal (size_t poly)
	{
		return faces_[ poly ]->getNormal();
	}

protected:
	void insertPolygon (Face *pdata, size_t poly)
	{
		FaceList::iterator it = faces_.begin();
		std::advance(it, poly);

		faces_.insert( it, pdata );
	}
	void insertPolygon (Face *pdata)
	{
		faces_.push_back( pdata );
	}
	void insertVertex (size_t poly, const Vector3 &vdata, size_t vertex)
	{
		faces_[poly]->insertVertex(vdata, vertex);
	}
	void insertVertex (size_t poly, const Vector3 &vdata)
	{
		faces_[poly]->insertVertex(vdata);
	}
	void deletePolygon (size_t poly)
	{
		FaceList::iterator it = faces_.begin();
		std::advance(it, poly);

		freePolygon(*it);
		faces_.erase(it);
	}
	void deleteVertex (size_t poly, size_t vertex)
	{
		faces_[poly]->deleteVertex(vertex);
	}
	void setPolygon (Face *pdata, size_t poly)
	{
		if (pdata != faces_[poly])
		{
			// delete old polygon
			freePolygon(faces_[ poly ]);

			// set new polygon
			faces_[poly] = pdata;
		}
	}
	void setVertex (size_t poly, const Vector3 &vdata, size_t vertex)
	{
		faces_[poly]->setVertex(vdata, vertex);
	}
};

#endif
