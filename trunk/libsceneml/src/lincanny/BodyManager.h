#ifndef BODYMANAGER_H
#define BODYMANAGER_H

#include "RigidBody.h"
#include "IDC.h"

const int MAX_CRITICAL_BODIES = 20;
const int MAX_BODIES = 20;
const double DOUBLE_MAX = 179769313;

class BodyManager{
public:
	BodyManager();
	~BodyManager();
	
	int addBody(BoundingMesh* mesh);
	int addBody(char** file_names, int num_files);
	int addCriticalBody(BoundingMesh* mesh);
	int addCriticalBody(BoundingMesh** meshes, int mesh_count);
	
	bool init();
	bool computeDistances();
	
	void setCriticalTransform(int id, double* trans);
	void setTransform(int id, double* trans);
	void setCriticalTransform(int id, double x, double y, double z, double x_rot, double y_rot, double z_rot);
	void setTransform(int id, double x, double y, double z, double x_rot, double y_rot, double z_rot);
	
	int getCriticalBodyCount();
	int getBodyCount();
	
	double** getDistancesArray();
	FEATURE*** getCriticalFeatureArray();
	FEATURE*** getFeatureArray();
	VERTEX3D** getCriticalPointArray();
	VERTEX3D** getPointArray();

	RigidBody** getCriticalBodyArray();
	RigidBody** getBodyArray();
	
protected:
	int initialized;
	int body_count, critical_count;
	double** distances;
	RigidBody** critical_bodies, **bodies;
	FEATURE*** critical_features, ***features;
	VERTEX3D** critical_points, **points;
	
	FEATURE*** critical_groups;
	FEATURE*** body_groups;
};


#endif
