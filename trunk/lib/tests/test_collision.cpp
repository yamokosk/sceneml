#include <xode/xode.h>

#include <string>
#include <memory>
#include <iostream>


using namespace xode;

int main(int argc, char* argv[])
{	
	if (argc == 2) {
		SceneDirector sceneDirector;
		
		XMLSceneBuilder xmlSceneBuilder(argv[1]);
		sceneDirector.SetSceneBuilder( &xmlSceneBuilder );
		sceneDirector.ConstructScene();
		
		std::auto_ptr<Scene> scene = sceneDirector.GetScene();
		scene.get()->update();	
		
		scene.get()->reportVars();
		
		float values[7] = {0, 100, 200, 300, 400, 500, 600};
		for (int n = 0; n < 7; ++n) {
			xode::ValueList_t vars;
			vars.push_back( NameValue("boxtrans", values[n], 0, 0) );
			scene.get()->setMutableVars(vars);
			scene.get()->update();
			if (scene.get()->inCollision()) {
				std::cout << "In collision at " << values[n] << std::endl;
				ContactGeoms_t contactData = scene.get()->getContactData();
				for (int m=0; m < contactData.size(); ++m) {
					std::cout << "     geom1: " << scene.get()->getGeomByID(contactData[m].g1) << std::endl
							<< "     geom2: " << scene.get()->getGeomByID(contactData[m].g2) << std::endl;
				}
				std::cout << "still going!" << std::endl;
			} else {
				std::cout << "Not in collision!" << std::endl;
			}
		}		
		return EXIT_SUCCESS;
	} else {
		std::cout << "Usage: test_build_scene filename.scene" << std::endl;
	}	
}
