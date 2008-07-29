/*
 * verify.cpp
 *
 *  Created on: Jul 28, 2008
 *      Author: yamokosk
 */

#include <SceneML.h>
#include <plugins/Geom.h>
#include <plugins/CollisionQuery.h>

#include <boost/cast.hpp>

using namespace sml;

int main(void)
{
	Root root;
	SceneMgr* sceneMgr = root.createSceneManager();
	//MeshManager* meshMgr = root.createMeshManager();

	// Create ODE object factory
	smlode::ODEObjectFactory objfactory;
	root.addSceneObjectFactory( &objfactory );

	smlode::CollisionQuery query;
	root.addSceneQuery( &query );

	// Create simple scene graph
	Node* n1 = sceneMgr->getRootNode()->createChild("n1");
	Node* n2 = n1->createChild("n2");
	Node* n3 = n2->createChild("n3");
	Node* n4 = n2->createChild("n4");
	Node* n5 = n4->createChild("n5");

	// .. with some variables
	Variable var1;
	var1.setType("translation");
	var1.subscribe(n2);
	var1.setVector( VectorFactory::Vector3( UNIT_X ) );

	Variable var2;
	var2.setType("rotation");
	var2.setSubType("x");
	var2.subscribe(n4);
	var2.setScalar( sml::zero );

	// Create some ODE geoms
	PropertyCollection boxparams;
	boxparams.addPair( RequiredProperty("type", "box") );
	boxparams.addPair( RequiredProperty("length", "3.0") );
	boxparams.addPair( RequiredProperty("width", "1.0") );
	boxparams.addPair( RequiredProperty("height", "2.0") );
	SceneObject* box = sceneMgr->createSceneObject("box", "ODE", &boxparams);
	n2->attachObject( box );

	PropertyCollection sphereparams;
	sphereparams.addPair( RequiredProperty("type", "sphere") );
	sphereparams.addPair( RequiredProperty("radius", "3.0") );
	SceneObject* sphere = sceneMgr->createSceneObject("sphere", "ODE", &sphereparams);
	n4->attachObject( sphere );

	var2.setScalar( sml::pi );

	sceneMgr->_updateSceneGraph();

	std::cout << *n1 << *n2 << *n3 << *n4 << *n5 << std::endl;

	// Check mybox
	smlode::Geom* mybox = boost::polymorphic_downcast<smlode::Geom*>(box);
	const dReal* boxpos = dGeomGetPosition( mybox->_getGeomID() );
	const dReal* boxrot = dGeomGetRotation( mybox->_getGeomID() );

	printf("Box pos: [%g, %g, %g]\n", boxpos[0], boxpos[1], boxpos[2]);
	printf("Box rot:\n");
	printf("\t[%g, %g, %g]\n", boxrot[0], boxrot[1], boxrot[2]);
	printf("\t[%g, %g, %g]\n", boxrot[4], boxrot[5], boxrot[6]);
	printf("\t[%g, %g, %g]\n", boxrot[8], boxrot[9], boxrot[10]);

	// Check mysphere
	smlode::Geom* mysphere = boost::polymorphic_downcast<smlode::Geom*>(sphere);
	const dReal* spherepos = dGeomGetPosition( mysphere->_getGeomID() );
	const dReal* sphererot = dGeomGetRotation( mysphere->_getGeomID() );

	printf("Sphere pos: [%g, %g, %g]\n", spherepos[0], spherepos[1], spherepos[2]);
	printf("Sphere rot:\n");
	printf("\t[%g, %g, %g]\n", sphererot[0], sphererot[1], sphererot[2]);
	printf("\t[%g, %g, %g]\n", sphererot[4], sphererot[5], sphererot[6]);
	printf("\t[%g, %g, %g]\n", sphererot[8], sphererot[9], sphererot[10]);


	// Do collision check

	return 0;
}
