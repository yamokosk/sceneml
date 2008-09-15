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
/*
 * Serializer.cpp
 *
 *  Created on: Sep 15, 2008
 *      Author: yamokosk
 */

#include "Serializer.h"

namespace TinySG
{

using namespace log4cxx;

LoggerPtr Serializer::logger(Logger::getLogger("Serializer"));

Serializer::Serializer() :
	root_(new TiXmlElement("TinySG"))
{

}

Serializer::~Serializer()
{
	if (root_) delete root_;
}

void Serializer::save(const std::string& filename)
{
	LOG4CXX_INFO(logger, "Saving file: " << filename);

	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "" );
	doc.LinkEndChild( decl );
	doc.LinkEndChild( root_ );
	doc.SaveFile( filename );

	LOG4CXX_INFO(logger, "Saved file: " << filename);
}

void Serializer::load(const std::string& filename)
{
	LOG4CXX_INFO(logger, "Loading file: " << filename);

	TiXmlDocument doc(pFilename);
	if (!doc.LoadFile()) return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);


	LOG4CXX_INFO(logger, "Loaded file: " << filename);
}

bool Serializer::serialize(Serializable* obj)
{
	serializeObject(obj, root_);
}

int	Serializer::deserialize(ObjectFactory*, CPtrList& objList)
{
	// block: name
	{
		pElem=hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!pElem) return;
		m_name=pElem->Value();

		// save this for later
		hRoot=TiXmlHandle(pElem);
	}

	// block: string table
	{
		m_messages.clear(); // trash existing table

		pElem=hRoot.FirstChild( "Messages" ).FirstChild().Element();
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			const char *pKey=pElem->Value();
			const char *pText=pElem->GetText();
			if (pKey && pText)
			{
				m_messages[pKey]=pText;
			}
		}
	}

	// block: windows
	{
		m_windows.clear(); // trash existing list

		TiXmlElement* pWindowNode=hRoot.FirstChild( "Windows" ).FirstChild().Element();
		for( pWindowNode; pWindowNode; pWindowNode=pWindowNode->NextSiblingElement())
		{
			WindowSettings w;
			const char *pName=pWindowNode->Attribute("name");
			if (pName) w.name=pName;

			pWindowNode->QueryIntAttribute("x", &w.x); // If this fails, original value is left as-is
			pWindowNode->QueryIntAttribute("y", &w.y);
			pWindowNode->QueryIntAttribute("w", &w.w);
			pWindowNode->QueryIntAttribute("hh", &w.h);

			m_windows.push_back(w);
		}
	}

	// block: connection
	{
		pElem=hRoot.FirstChild("Connection").Element();
		if (pElem)
		{
			m_connection.ip=pElem->Attribute("ip");
			pElem->QueryDoubleAttribute("timeout",&m_connection.timeout);
		}
	}

}

void Serializer::serializeObject(Serializable* obj, TiXmlElement* parent)
{
	LOG4CXX_INFO(logger, "Serializing " << obj->getClassName());

	TiXmlElement* child = new TiXmlElement( obj->getClassName() );

	if ( parent != NULL) {
		LOG4CXX_INFO(logger, "Linking " << obj->getClassName() << " to parent.");
		parent->LinkEndChild( child );
	}

	// Get attributes for this obj
	PropertyCollection pc;
	obj->getAttributes(pc);
	LOG4CXX_INFO(logger, "Got " << pc.size() << " " << obj->getClassName() << " attributes.");

	// Loop over attributes
	for (unsigned int iter = 0; iter < pc.size(); ++iter)
	{
		PropertyPair pair = pc.getPair(iter);
		LOG4CXX_INFO(logger, "Setting " << obj->getClassName() << " attribute " << pair.getPropertyName() << " to " << pair.getPropertyValue() << ".");
		child->SetAttribute(pair.getPropertyName(), pair.getPropertyValue());
	}

	// Get object's properties to serialize
	Serializable::ComplexProperties cmplx = obj->getProperties();

	// Loop over these complex properties
	LOG4CXX_INFO(logger, "Setting " << cmplx.size() << " complex properties.");
	for (unsigned int n=0; n < cmplx.size(); ++n)
	{
		serializeObject(cmplx[n], child);
	}
}

/*int Serializer::deserializeObject(ObjectFactory*, CPtrList& objList,MSXML::IXMLDOMNode* parent=NULL)
{

}*/

}
