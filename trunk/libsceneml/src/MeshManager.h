/*
 * MeshManager.h
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#ifndef MESHMANAGER_H_
#define MESHMANAGER_H_

namespace sml {

class MeshManager : public Singleton<MeshManager>
{
public:
	MeshManager();
	virtual ~MeshManager();
};

}

#endif /* MESHMANAGER_H_ */
