/*
 * Query.h
 *
 *  Created on: Jul 25, 2008
 *      Author: yamokosk
 */

#ifndef SCENEQUERY_H_
#define SCENEQUERY_H_

namespace TinySG {

// Forward declarations
class ObjectManager;
class Query;

class QueryResult
{
public:
	QueryResult(Query* q) : query_(q) {};
	virtual ~QueryResult() {};

	Query* getCreator() {return query_;}

protected:
	Query* query_;
};


class Query
{
public:
	Query(const std::string& type) : type_(type) {};
	virtual ~Query() {};
	virtual QueryResult* execute(const ObjectManager* mgr, const PropertyCollection* params = 0) = 0;
	virtual const std::string& getType() {return type_;}
private:
	std::string type_;
};

}

#endif /* SCENEQUERY_H_ */
