/*************************************************************************
 * SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
 * All rights reserved.
 * Email: yamokosk at gmail dot com
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the License, 
 * or (at your option) any later version. The text of the GNU Lesser General 
 * Public License is included with this library in the file LICENSE.TXT.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for 
 * more details.
 *
 *************************************************************************/

#ifndef _SCENEML_SCENE_BUILDER_H_FILE_
#define _SCENEML_SCENE_BUILDER_H_FILE_

#include <memory>

#include "config.h"
#include "scene.h"

namespace sceneml {

// Abstract Builder
class SceneBuilder
{
protected:
	ScenePtr scene_;
public:
	SceneBuilder() {}
	virtual ~SceneBuilder() {}
	ScenePtr GetScene() { return scene_; }
 
	void createNewScene() { scene_.reset(new Scene); }
 
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
	ScenePtr GetScene() {return sceneBuilder->GetScene();}
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
