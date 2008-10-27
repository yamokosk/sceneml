#include "OBJImport.h"

int importOBJ(char* filename, POLYHEDRON* mesh){
	
	//open the file 
	FILE* file;
	file = fopen(filename,"r");
	if(!file){
		fprintf(stderr, "there is an problem with the obj file: exiting import routine.\n");
		fclose(file);
		return 1;
	}

	//collect number of features of the polyhedron
	char buf[256];
	while(fscanf(file, "%s", buf) != EOF){
		switch(buf[0]){
			case 'v':
				if(buf[1] == '\0'){  
					mesh->vertex_count++;	
				}
				fgets(buf, sizeof(buf), file);
				break;
			case 'f':
				if(strstr(buf, "/")){
					fprintf(stderr, "there is an problem with the faces in the obj file. please define only vertices: exiting import routine\n");
					fclose(file);
					return 1;
				}
				else{
					mesh->face_count++;
				}				
				break;
			default:
				fgets(buf, sizeof(buf), file);
				break;
		}
	}
	rewind(file);

	//fill feature arrays
	mesh->vertices = new VERTEX3D*[mesh->vertex_count];
	mesh->faces = new FACE*[mesh->face_count];
	mesh->edges = new EDGE*[mesh->face_count+ mesh->vertex_count - 2]; //Euler's formula
	int v = 0;
	int f = 0;
	double x, y, z;

	while(fscanf(file, "%s", buf) != EOF){
		switch(buf[0]){
			case 'v':		
				
				fscanf(file, "%lf %lf %lf", &x, &y, &z); 
				mesh->vertices[v] = new VERTEX3D(x,y,z);
				/*
				mesh->vertices[v].x = x;
				mesh->vertices[v].y = y;
				mesh->vertices[v].z = z;
				*/
				mesh->vertices[v]->index = v++;
	
				//fgets(buf, sizeof(buf), file);
				break;
		
			case 'f':
				int v1, v2, v3, v4;
				v4 = -1;
				if(fscanf(file, "%d %d %d %d ", &v1, &v2, &v3, &v4) > 4){
					fprintf(stderr, "there is an problem with the faces in the obj file. please use quads or triangles only: exiting import routine\n");
					fclose(file);
					return 1;
				}		
				v1--;
				v2--;
				v3--;
				v4--;
				if(v4==-2){
					//triangle
					EDGE* e1 = addEdge(mesh, v1, v2);
					EDGE* e2 = addEdge(mesh, v2, v3);
					EDGE* e3 = addEdge(mesh, v3, v1);
				
					mesh->vertices[v1]->addToAdjacency(e1);
					mesh->vertices[v1]->addToAdjacency(e3);
					
					mesh->vertices[v2]->addToAdjacency(e1);
					mesh->vertices[v2]->addToAdjacency(e2);
					
					mesh->vertices[v3]->addToAdjacency(e2);
					mesh->vertices[v3]->addToAdjacency(e3);
					
					mesh->faces[f] = new FACE(mesh->vertices[v1],mesh->vertices[v2],mesh->vertices[v3], e1, e2, e3);				
				}
				else{
					//quad
					EDGE* e1 = addEdge(mesh, v1, v2);
					EDGE* e2 = addEdge(mesh, v2, v3);
					EDGE* e3 = addEdge(mesh, v3, v4);
					EDGE* e4 = addEdge(mesh, v4, v1);	

					mesh->vertices[v1]->addToAdjacency(e1);
					mesh->vertices[v1]->addToAdjacency(e4);

					mesh->vertices[v2]->addToAdjacency(e1);
					mesh->vertices[v2]->addToAdjacency(e2);

					mesh->vertices[v3]->addToAdjacency(e2);
					mesh->vertices[v3]->addToAdjacency(e3);

					mesh->vertices[v4]->addToAdjacency(e3);
					mesh->vertices[v4]->addToAdjacency(e4);


					mesh->faces[f] = new FACE(mesh->vertices[v1],mesh->vertices[v2],mesh->vertices[v3],mesh->vertices[v4], e1, e2, e3, e4);
				}				
				mesh->faces[f]->index = f++;
				
				break;
			default:
				fgets(buf, sizeof(buf), file);
				break;
		}
	}
	

	//printf("h id = %i\n",mesh->edges[4]->left_face->getVertexIndex(mesh->edges[4]->head));
	//printf("t id = %i\n",mesh->edges[4]->left_face->getVertexIndex(mesh->edges[4]->tail));
	//close file
	fclose(file);
	return 0;
}


EDGE* addEdge(POLYHEDRON* mesh,int head_index,int tail_index){
	for(int i=0;i<mesh->edge_count;i++){
		if((mesh->edges[i]->head->index == head_index) && (mesh->edges[i]->tail->index == tail_index) ){
			return mesh->edges[i];
		}
		else if((mesh->edges[i]->head->index == tail_index) && (mesh->edges[i]->tail->index == head_index)){
			return mesh->edges[i];
		}
	}
	EDGE* e = new EDGE(mesh->vertices[head_index], mesh->vertices[tail_index]);
	mesh->edges[mesh->edge_count] = e;
	mesh->edges[mesh->edge_count]->index = mesh->edge_count++;
	return e;
}