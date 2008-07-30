/*
 * verify.cpp
 *
 *  Created on: Jul 28, 2008
 *      Author: yamokosk
 */

#include <SceneML.h>
#include <plugins/ODEPlugin.h>

#include <boost/cast.hpp>

using namespace sml;

int main(void)
{
	Root root;

	// Register ODE Framework and its factories with SceneML
	ODEPlugin odePlugin();
	root.registerPlugin( &odePlugin );

	// Create various SceneML Managers
	SceneManager* sceneMgr = root.createSceneManager();
	//MeshManager* meshMgr = root.createMeshManager();

	// Create some ODE spaces
	PropertyCollection spaceparams;
	spaceparams.addPair( RequiredProperty("type", "simple") );
	smlode::Space* space1 = boost::polymorphic_downcast<smlode::Space*>( sceneMgr->createEntity("space1", "ODE_SPACE", &spaceparams) );
	smlode::Space* space2 = boost::polymorphic_downcast<smlode::Space*>( sceneMgr->createEntity("space2", "ODE_SPACE", &spaceparams) );

	// Create some ODE geoms
	PropertyCollection boxparams;
	boxparams.addPair( RequiredProperty("type", "box") );
	boxparams.addPair( RequiredProperty("length", "3.0") );
	boxparams.addPair( RequiredProperty("width", "1.0") );
	boxparams.addPair( RequiredProperty("height", "2.0") );
	smlode::Geom* box = boost::polymorphic_downcast<smlode::Geom*>( sceneMgr->createEntity("box", "ODE_GEOM", &boxparams) );

	PropertyCollection sphereparams;
	sphereparams.addPair( RequiredProperty("type", "sphere") );
	sphereparams.addPair( RequiredProperty("radius", "3.0") );
	smlode::Geom* sphere = boost::polymorphic_downcast<smlode::Geom*>( sceneMgr->createEntity("sphere", "ODE_GEOM", &sphereparams) );

	// Create some variables
	Variable var1;
	var1.setType("translation");
	var1.setVector( VectorFactory::Vector3( UNIT_X ) );

	Variable var2;
	var2.setType("rotation");
	var2.setSubType("x");
	var2.setScalar( sml::zero );

	// Finally lets create simple scene graph
	Node* n1 = sceneMgr->getRootNode()->createChild("n1", VectorFactory::Vector3(0., 0., 2.));
	Node* n2 = n1->createChild("n2");
	Node* n3 = n2->createChild("n3", VectorFactory::Vector3(0.,-1.,0.));
	Node* n4 = n2->createChild("n4");
	Node* n5 = n4->createChild("n5");

	// .. tell some nodes to listen to our variables
	n2->listen( &var1 );
	n4->listen( &var2 );

	// .. attach our geoms to our scene graph
	n2->attachObject( box );
	n4->attachObject( sphere );

	// .. finally add our geoms to the spaces we created
	space1->addGeom( box );
	space2->addGeom( sphere );

	// Update the scene graph
	sceneMgr->update(); // internally calls _updateSceneGraph() and then executes all Queries.. each query executed on its own thread
	bool* inCollision = sceneMgr->getQueryResult("ODE_Collision_Check"); // returns an auto_ptr

	// Change the variables
	var2.setScalar( sml::pi );

	// Update the scene graph again to flow variable influence throughout graph
	sceneMgr->update(); // internally calls _updateSceneGraph() and then executes all Queries.. each query executed on its own thread
	bool* inCollision = sceneMgr->getQueryResult("ODE_Collision_Check");

	// Print some stuff out
	std::cout << *n1 << *n2 << *n3 << *n4 << *n5 << std::endl;

	// Check box data directly from ODE
	const dReal* boxpos = dGeomGetPosition( box->_getGeomID() );
	const dReal* boxrot = dGeomGetRotation( box->_getGeomID() );

	printf("Box pos: [%g, %g, %g]\n", boxpos[0], boxpos[1], boxpos[2]);
	printf("Box rot:\n");
	printf("\t[%g, %g, %g]\n", boxrot[0], boxrot[1], boxrot[2]);
	printf("\t[%g, %g, %g]\n", boxrot[4], boxrot[5], boxrot[6]);
	printf("\t[%g, %g, %g]\n", boxrot[8], boxrot[9], boxrot[10]);

	// Check mysphere
	const dReal* spherepos = dGeomGetPosition( sphere->_getGeomID() );
	const dReal* sphererot = dGeomGetRotation( sphere->_getGeomID() );

	printf("Sphere pos: [%g, %g, %g]\n", spherepos[0], spherepos[1], spherepos[2]);
	printf("Sphere rot:\n");
	printf("\t[%g, %g, %g]\n", sphererot[0], sphererot[1], sphererot[2]);
	printf("\t[%g, %g, %g]\n", sphererot[4], sphererot[5], sphererot[6]);
	printf("\t[%g, %g, %g]\n", sphererot[8], sphererot[9], sphererot[10]);


	// Do collision check

	return 0;
}
