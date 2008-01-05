#include <sceneml.h>

#include <string>
#include <memory>
#include <iostream>


using namespace sceneml;

int main(int argc, char* argv[])
{	
	if (argc == 2) {
		SceneDirector sceneDirector;
		
		XMLSceneBuilder xmlSceneBuilder(argv[1]);
		sceneDirector.SetSceneBuilder( &xmlSceneBuilder );
		sceneDirector.ConstructScene();
		
		std::auto_ptr<Scene> scene = sceneDirector.GetScene();
		scene.get()->update();	
		
		std::cout << "Body src3 t-matrix:" << std::endl;
		scene.get()->report("src3");
		std::cout << "\nBody src3 geom t-matrix:" << std::endl;
		scene.get()->reportGeom("src301");
		std::cout << std::endl;
		
		/*xode::ValueList_t values;
		values.push_back( NameValue("src3", M_PI/4) );
		
		std::cout << "Changing variable src3 to " << M_PI/4 << std::endl;
		scene.get()->setMutableVars(values);
		scene.get()->update();
		
		std::cout << "Body src3 t-matrix:" << std::endl;
		scene.get()->report("src3");
		std::cout << "\nBody src3 geom t-matrix:" << std::endl;
		scene.get()->reportGeom("src301");
		std::cout << std::endl;*/
		
		return EXIT_SUCCESS;
	} else {
		std::cout << "Usage: test_build_scene filename.scene" << std::endl;
	}	
}
