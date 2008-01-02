#include "MeshImport.h"

#include <stdlib.h> 
#include <string> 
#include <iostream> 
#include <fstream>
#include <stdexcept>

int importBinarySTL(POLYHEDRON* mesh);
int importAsciiSTL(POLYHEDRON* mesh);

int importSTL(POLYHEDRON* mesh)
{
	// Open the file 
	std::fstream file;
	file.open (mesh->filename.c_str(), std::ios_base::in | std::ios_base::binary);

	if( !file.good() ){
		file.close();
		throw std::runtime_error("There is an problem opening " + mesh->filename + ".");
	}

	// An ASCII STL file begins with the line: solid name
	char header[80];
	file.read(header, sizeof(header));
	char *tok = strtok(header, " ");
	file.close();
	
	if ( strcmp(tok, "solid") == 0 )
		return importAsciiSTL( mesh );
	else
		return importBinarySTL( mesh );
}

int importBinarySTL(POLYHEDRON* mesh)
{
	// Open the file 
	std::fstream file;
	
	file.open (mesh->filename.c_str(), std::ios_base::in | std::ios_base::binary);

	if (file.is_open())
	{
		char attributeByteCount[2];
		file.seekg(80, std::ios::beg); // Skip first 80 bytes

		long numFacets = 0;
		file.read((char *)&numFacets, sizeof(unsigned));

		// Fill feature arrays
		mesh->index_count = numFacets;
		mesh->vertex_count = numFacets*3;
		long nVertCoords = (mesh->vertex_count)*3;
		long nIndexCoords = numFacets*3;
		
		mesh->vertices = (float*)malloc( sizeof(float)*nVertCoords );
		mesh->indices = (unsigned int*)malloc( sizeof(unsigned int)*nIndexCoords );
		
		if ( (mesh->vertices == NULL) || (mesh->indices == NULL) ) {
			throw std::runtime_error("Ran out of system memory while trying to allocate space for mesh.");
		}
		
		mesh->vertex_stride = sizeof(float) * 3;
		mesh->index_stride = sizeof(unsigned int) * 3;
		
		for (long i = 0; i < numFacets; ++i) {
			float normal[3];

			file.read((char *)&normal[0], sizeof(float));
			file.read((char *)&normal[1], sizeof(float));
			file.read((char *)&normal[2], sizeof(float));

			for (long n=0; n < 9; ++n)
				file.read((char *)&mesh->vertices[i*9 + n], sizeof(float));
			
			for (int n=0; n < 3; ++n)
				mesh->indices[i*3 + n] = i*3 + n;
			
			file.read(attributeByteCount, sizeof(attributeByteCount));
		} // END FOR LOOP

	} else {
		throw std::runtime_error("Strange, could not open " + mesh->filename + " for reading.");
	}

	file.close();
	return 0;
}

int importAsciiSTL(POLYHEDRON* mesh)
{
	std::fstream file;
	file.open (mesh->filename.c_str(), std::ios_base::in);

	if (file.is_open())
	{
		//collect number of features of the polyhedron
		char buf[256];
		while( !file.eof() ) {
			file.getline(buf,sizeof(buf));
			char *tok = strtok(buf, " ");
			if (tok != NULL) {
				if (strcmp(tok, "facet") == 0) {
					//std::cout << "Found a facet" << std::endl;
					mesh->index_count++;
				} 
				
				if (strcmp(tok, "vertex") == 0) {
					//std::cout << "Found a vertex" << std::endl;
					mesh->vertex_count++;
				}
			}
		}
		file.clear();				// forget we hit the end of file
		file.seekg(0, std::ios::beg);	// move to the start of the file
		
		// Fill feature arrays
		mesh->vertices = (float*)malloc( sizeof(float)*(mesh->vertex_count * 3) );
		mesh->indices = (unsigned int*)malloc( sizeof(unsigned int)*(mesh->index_count * 3) );
		
		if ( (mesh->vertices == NULL) || (mesh->indices == NULL) ) {
			throw std::runtime_error("Ran out of system memory while trying to allocate space for mesh.");
		}
		
		mesh->vertex_stride = sizeof(float) * 3;
		mesh->index_stride = sizeof(unsigned int) * 3;

		double v[3]; char tmp[80];
		int vp = 0, ip = 0;

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
					for (int n=0; n < 3; ++n) {
						mesh->vertices[vp] = (float)v[n];
						vp++;
					}
					if (vp > (mesh->vertex_count*3)) throw std::runtime_error("Encountered more vertices than I counted!");
				}
			
				if (strcmp(tok, "facet") == 0) {
					for (int n=0; n < 3; ++n) {
						mesh->indices[ip] = ip;
						ip++;
					}
				}
			}
		} // END IF STATEMENT
	
	} else {
		throw std::runtime_error("Strange, could not open " + mesh->filename + " for reading.");
	}

	file.close();
	return 0;
}
