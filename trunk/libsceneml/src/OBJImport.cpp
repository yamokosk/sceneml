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
#include <tinysg/Exception.h>

namespace TinySG
{

void importOBJ(const std::string& filename, TriMesh* m, float sx, float sy, float sz)
{
	// Open the file
	std::fstream file;
	file.open(filename, std::ios_base::in);

	if( !file.good() ){
		file.close(); ERR_FILE_NOT_FOUND;
		SML_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "File \"" + filename + "\" not found.");
	}

	//collect number of features of the polyhedron
	unsigned int nv = 0, nf = 0;
	char buf[256];
	while( !file.eof() ) {
		file.getline(buf,sizeof(buf));
		switch(buf[0]){
			case 'v': {
				if(buf[1] == ' ') nv++;
				break;
			}
			case 'f': {
				if(buf[1] == ' ') nf++;
				break;
			}
			default:
				break;
		}
	}
	file.clear();              // forget we hit the end of file
	file.seekg(0, std::ios::beg);   // move to the start of the file

	// Fill feature arrays
	m->init(nv, nf);
	double v[3] = {0.0, 0.0, 0.0};
	while( !file.eof() )
	{
		file.getline(buf,sizeof(buf));
		switch (buf[0])
		{
			case 'v':
			{
				if(buf[1] == ' ')
				{
					sscanf( (buf+2), "%lf %lf %lf", v, v+1, v+2);
					m->addVertex(sx*(float)v[0], sy*(float)v[1], sz*(float)v[2]);
				}
				break;
			}
			case 'f':
			{
				if(buf[1] == ' ')
				{
					unsigned int ids[3] = {0, 0, 0};
					char *tok; unsigned int nverts = 0;
					tok = strtok((buf+2)," ");
					while (tok != NULL)
					{
						if ( nverts > 2 )
						{
							file.close(); ERR_INVALIDPARAMS;
							SML_EXCEPT(Exception::ERR_INVALIDPARAMS, "Quadrilateral face found! Only triangle faces supported.");
						}

						char *tmp = NULL, index[10];
						if ( (tmp = strstr(tok, "/")) != NULL )
						{
							// A slash was found. Just need to extract the first digit
							memset(index, '\0', sizeof(index));
							memcpy(index, tok, (tmp - tok)*sizeof(char));
							ids[nverts] = atoi(index)-1;
							nverts++;
						} else {
							// No slash was found. Just process as if it is a number
							ids[nverts] = atoi(index)-1;
							nverts++;
						}
						tok = strtok(NULL, " ");
					}
					m->addFace(ids[0], ids[1], ids[2]);
				}
				break;
			}
			default:
				break;
		}
	}

	//close file
	file.close();
}

}
