/*************************************************************************
 * TinySG, Copyright (C) 2007, 2008  J.D. Yamokoski
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

#include <tinysg/MeshImport.h>

namespace TinySG
{

void importBinarySTL(const std::string& filename, TriMesh* m, float sx, float sy, float sz);
void importAsciiSTL(const std::string& filename, TriMesh* m, float sx, float sy, float sz);

void importSTL(const std::string& filename, TriMesh* m, float sx, float sy, float sz)
{
	// Open the file
	std::fstream file;
	file.open(filename, std::ios_base::in);

	if( !file.good() ){
		file.close(); ERR_FILE_NOT_FOUND;
		SML_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "File \"" + filename + "\" not found.");
	}

	// An ASCII STL file begins with the line: solid name
	char header[80];
	file.read(header, sizeof(header));
	char *tok = strtok(header, " ");
	file.close();

	if ( strcmp(tok, "solid") == 0 )
		return importAsciiSTL(filename, m, sx, sy, sz);
	else
		return importBinarySTL(filename, m, sx, sy, sz);
}

void importBinarySTL(const std::string& filename, TriMesh* m, float sx, float sy, float sz)
{
	// Open the file
	std::fstream file;
	file.open (filename, std::ios_base::in | std::ios_base::binary);

	if (file.is_open())
	{
		char attributeByteCount[2];
		file.seekg(80, std::ios::beg); // Skip first 80 bytes

		long numFacets = 0;
		file.read((char *)&numFacets, sizeof(unsigned));

		// Fill feature arrays
		m->init(numFacets*3, numFacets);
		for (long i = 0; i < numFacets; ++i)
		{
			float normal[3];
			file.read((char *)&normal[0], sizeof(float));
			file.read((char *)&normal[1], sizeof(float));
			file.read((char *)&normal[2], sizeof(float));

			float vx, vy, vz;
			for (long n=0; n < 3; ++n)
			{
				file.read((char *)&vx, sizeof(float));
				file.read((char *)&vy, sizeof(float));
				file.read((char *)&vz, sizeof(float));

				m->addVertex(sx*vx, sy*vy, sz*vz);
			}
			mesh->addFace(i*3+0, i*3+1, i*3+2);

			file.read(attributeByteCount, sizeof(attributeByteCount));
		} // END FOR LOOP

	}

	file.close();
	return 0;
}

int importAsciiSTL(const std::string& filename, TriMesh* m, float sx, float sy, float sz)
{
	std::fstream file;
	file.open (mesh->filename.c_str(), std::ios_base::in);

	if (file.is_open())
	{
		//collect number of features of the polyhedron
		char buf[256];
		unsigned int nv=0, nf=0;
		while( !file.eof() ) {
			file.getline(buf,sizeof(buf));
			char *tok = strtok(buf, " ");
			if (tok != NULL) {
				if (strcmp(tok, "facet") == 0) nf++;
				if (strcmp(tok, "vertex") == 0) nv++;
			}
		}
		file.clear();				// forget we hit the end of file
		file.seekg(0, std::ios::beg);	// move to the start of the file

		// Fill feature arrays
		m->init(nv, nf);

		double v[3]; char tmp[80];
		unsigned int ip = 0;

		while( !file.eof() ) {
			file.getline(buf,sizeof(buf));
			//std::cout << buf << std::endl;

			char *tok = strtok(buf, " ");
			if (tok != NULL) {
				if (strcmp(tok, "vertex") == 0) {
					int len = tok - buf;
					memset(tmp, '\0', sizeof(tmp));
					memcpy(tmp, buf+len+7, sizeof(tmp));
					sscanf( tmp, "%lf %lf %lf", v, v+1, v+2);
					m->addVertex(sx*(float)v[0], sy*(float)v[1], sz*(float)v[2]);
				}

				if (strcmp(tok, "facet") == 0) {
					m->addFace(ip, ip+1 ip+2);
					ip += 3;
				}
			}
		} // END IF STATEMENT

	}

	file.close();
	return 0;
}

};
