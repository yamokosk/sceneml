#ifndef _XODE_SCENE_BUILDER_H_FILE_
#define _XODE_SCENE_BUILDER_H_FILE_

#include <xode/scene.h>

#include <string>

namespace xode {

// Abstract Builder
class SceneBuilder
{
protected:
	std::auto_ptr<Scene> scene;
public:
	SceneBuilder() {}
	virtual ~SceneBuilder() {}
	std::auto_ptr<Scene> GetScene() { return scene; }
 
	void createNewScene() { scene.reset(new Scene); }
 
	// General build methods
	virtual void buildSpaces()=0;
	virtual void buildBodies()=0;
	virtual void buildGeoms()=0;
};
 
// Director
class SceneDirector
{
	SceneBuilder* sceneBuilder;
public:
	SceneDirector() : sceneBuilder(NULL) {}
	~SceneDirector() {}
 
	void SetSceneBuilder(SceneBuilder* b) { sceneBuilder = b; }
	std::auto_ptr<Scene> GetScene() {return sceneBuilder->GetScene();}
	void ConstructScene()
	{
		sceneBuilder->createNewScene();
		sceneBuilder->buildSpaces();
		sceneBuilder->buildBodies();
		sceneBuilder->buildGeoms();
	}
};

};


#endif
