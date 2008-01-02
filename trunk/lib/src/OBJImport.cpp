#include "MeshImport.h"

#include <stdlib.h> 
#include <string> 
#include <iostream> 
#include <fstream>
#include <stdexcept>

int importOBJ(POLYHEDRON* mesh)
{
	// Open the file 
	std::fstream file;
	file.open (mesh->filename.c_str(), std::ios_base::in);

	if( !file.good() ){
		file.close();
		throw std::runtime_error("There is an problem loading obj file: exiting import routine.");
	}

	//collect number of features of the polyhedron
	char buf[256];
	while( !file.eof() ) {
		file.getline(buf,sizeof(buf));
		switch(buf[0]){
			case 'v': {
				if(buf[1] == ' '){  
					mesh->vertex_count++;	
				}
				break;
			}
			case 'f': {
				if(buf[1] == ' '){  
					mesh->index_count++;	
				}
				break;
			}
			default:
				break;
		}
	}
	file.clear();              // forget we hit the end of file
	file.seekg(0, std::ios::beg);   // move to the start of the file
	
	// Fill feature arrays
	mesh->vertices = (float*)malloc( sizeof(float)*(mesh->vertex_count * 3) );
	mesh->indices = (unsigned int*)malloc( sizeof(unsigned int)*(mesh->index_count * 3) );
	
	if ( (mesh->vertices == NULL) || (mesh->indices == NULL) ) {
		throw std::runtime_error("Ran out of system memory while trying to allocate space for mesh.");
	}
		
	mesh->vertex_stride = sizeof(float) * 3;
	mesh->index_stride = sizeof(unsigned int) * 3;
	double v[3];
	int vp = 0, ip = 0;
	
	while( !file.eof() ) {
		file.getline(buf,sizeof(buf));
		switch(buf[0]) {
			case 'v': {
				if(buf[1] == ' '){  
					sscanf( (buf+2), "%lf %lf %lf", v, v+1, v+2);
					for (int n=0; n < 3; ++n) {
						mesh->vertices[vp] = (float)v[n];
						vp++;
					}
					if (vp > (mesh->vertex_count*3)) throw std::runtime_error("Encountered more vertices than I counted!");
				}
				break;
			}
			case 'f': {
				if(buf[1] == ' '){  
					char *tok; int nverts = 0;
					tok = strtok((buf+2)," ");
					while (tok != NULL)
					{
						char *tmp = NULL, index[10];
						if ( (tmp = strstr(tok, "/")) != NULL ) {
							// A slash was found. Just need to extract the first digit
							memset(index, '\0', sizeof(index));
							memcpy(index, tok, (tmp - tok)*sizeof(char));
							mesh->indices[ip] = atoi(index)-1;
							ip++; nverts++;
						} else {
							// No slash was found. Just process as if it is a number
							mesh->indices[ip] = atoi(index)-1;
							ip++; nverts++;
						}
						if ( nverts > 3 ) {
							throw std::runtime_error("Quadrilateral face found! Only triangle faces supported.");
						}
						tok = strtok (NULL, " ");
					}
				}
				break;
			}
			default:
				break;
		}
	}
	
	/*std::cout << "Loaded mesh" << std::endl
			<< "   nverts: " << mesh->vertex_count << std::endl
			<< "   nfaces: " << mesh->index_count << std::endl
			<< "Vertices" << std::endl;
	for (int n=0; n < (mesh->vertex_count * 3); n = n + 3)
	{
		std::cout << n/3 << ": ("
				<< *(mesh->vertices + n) << ", "
				<< *(mesh->vertices + n + 1) << ", "
				<< *(mesh->vertices + n + 2) << ")" << std::endl;
	}
	std::cout << "Indices" << std::endl;
	for (int n=0; n < (mesh->index_count * 3); n = n + 3)
	{
		std::cout << n/3 << ": ("
				<< *(mesh->indices + n) << ", "
				<< *(mesh->indices + n + 1) << ", "
				<< *(mesh->indices + n + 2) << ")" << std::endl;
	}*/

	//close file
	file.close();
	return 0;
}
