#include <xode/xode.h>

#include <string>
#include <memory>
#include <iostream>


using namespace xode;

int main(int argc, char* argv[])
{	
	SceneDirector sceneDirector;
	XMLSceneBuilder *xmlSceneBuilder = NULL;
	
	if (argc == 2) {
		xmlSceneBuilder = new XMLSceneBuilder(argv[1]);
	} else {
		xmlSceneBuilder = new XMLSceneBuilder("twopa10.scene");
	}	
	
	sceneDirector.SetSceneBuilder( xmlSceneBuilder );
	sceneDirector.ConstructScene();
		
	std::auto_ptr<Scene> scene = sceneDirector.GetScene();
	scene.get()->update();	
	
	scene.get()->setMutableValue("q_src_1", M_PI/5);
	scene.get()->update();

	delete xmlSceneBuilder;
	return EXIT_SUCCESS;	
}
