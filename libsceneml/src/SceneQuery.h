/*
 * SceneQuery.h
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#ifndef SCENEQUERY_H_
#define SCENEQUERY_H_

#include <Subject.h>

namespace sml {

// Forward declarations
class SceneManager;
class QueryResult;

class SceneQuery : public Subject
{
public:

	enum QueryStatus
	{
		QUERY_STARTED=100,
		QUERY_COMPLETE
	};

	SceneQuery();
	virtual ~SceneQuery();

	virtual SceneQuery* clone() const = 0;
	virtual void execute(const SceneManager* mgr) = 0;
	virtual std::string getType() const = 0;

	virtual QueryResult* getResult() = 0;
	virtual void deleteResult( QueryResult* result ) = 0;
};

class QueryResult
{
public:
	QueryResult(SceneQuery* creator);
	virtual ~QueryResult();

	SceneQuery* getCreator();
	virtual std::string getTypeName() = 0;

protected:
	SceneQuery* creator_;
};

}

#endif /* SCENEQUERY_H_ */
