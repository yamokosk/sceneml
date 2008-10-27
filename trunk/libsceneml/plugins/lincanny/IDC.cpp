#include "IDC.h"

double IDENITY[] = {1.0,0,0,0,0,1.0,0,0,0,0,1.0,0,0,0,0,1.0};
Vertex ZERO_VECTOR;
double closestFeaturesInit(Polyhedron* poly1, Polyhedron* poly2, double* poly1_trans, double* poly2_trans, Feature* &f1, Feature* &f2, Vertex &pos1, Vertex &pos2, int debug){

	if(poly1->vertex_count < 1 || poly2->vertex_count < 1){
		return -0.0;
	}

	Vertex* v1 = (poly1->vertices[0]);
	Vertex* v2 = (poly2->vertices[0]);

	f1 = static_cast<Feature*>(v1);
	f2 = static_cast<Feature*>(v2);

	return closestFeatures(poly1, poly2, poly1_trans, poly2_trans, f1, f2, pos1, pos2, debug, 400);
}

double closestFeatures(Polyhedron* poly1, Polyhedron* poly2, double* poly1_trans, double* poly2_trans,Feature* &f1, Feature* &f2, Vertex &pos1, Vertex &pos2, int debug, int depth){

	if(poly1->vertex_count < 1 || poly2->vertex_count < 1){
		return -0.0;
	}

	Vertex* v1 = (poly1->vertices[0]);
	Vertex* v2 = (poly2->vertices[0]);
	Edge *edge1, *edge2;
	Polygon *face1, *face2;

	f1 = static_cast<Feature*>(f1);
	f2 = static_cast<Feature*>(f2);

	int z = 0;
	int dp = 0;
	int stop = depth;

	Vertex edge_point;
	Vertex edge_point1, edge_point2;

	bool search, search_r1, search_r2;
	search = search_r1 = search_r2 = true;

	while(search)
	{

		switch(f1->id){
			case Feature::Vertex:
				v1 = static_cast<Vertex*>(f1);
				break;
			case Feature::Edge:
				edge1 = static_cast<Edge*>(f1);
				break;
			case Feature::Face:
				face1 = static_cast<Polygon*>(f1);
				break;
		}
		while(search_r2){
			dp ++;
		//	printf("2, %i:\n", debug);
			switch(f2->id){
				case Feature::Vertex:
					//printf("vertex %i - ",v2->index);
					v2 = static_cast<Vertex*>(f2);
					switch(f1->id){
						case Feature::Vertex:
						//	printf("vertex %i\n",v1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(vertex_vertex_test(v1, v2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}

							break;
						case Feature::Edge:
							//printf("edge %i\n", edge1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							closestPointEdge(v2, edge1, poly2_trans, poly1_trans, edge_point);
							if(vertex_vertex_test(&edge_point, v2, IDENITY, poly2_trans, f2)){
								search_r2 = false;
							}

							break;

						case Feature::Face:
							//printf("face %i\n",face1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}

							Vertex face_point;

							closestPointFace(v2, face1, poly2_trans, poly1_trans, face_point);
							if(vertex_vertex_test(&face_point, v2, IDENITY, poly2_trans, f2)){
								search_r2 = false;
							}


							break;
					}
					break;
				case Feature::Edge:
					edge2 = static_cast<Edge*>(f2);
					//printf("edge %i - ",edge2->index);
					switch(f1->id){
						case Feature::Vertex:
							//printf("vertex %i\n",v1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(vertex_edge_test(v1, edge2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}

							break;
						case Feature::Edge:
							//printf("edge %i\n",edge1->index);
							if(z++ == stop){
								search = search_r2 = search_r1 = false;
								break;
							}
							if(edge_edge_test(edge1, edge2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}
							break;

						case Feature::Face:
							//printf("face %i\n",face1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(face_edge_test(face1,edge2,poly1_trans,poly2_trans,f2)){
								search_r2 = false;
							}

							break;
					}
					break;
				case Feature::Face:
					face2 = static_cast<Polygon*>(f2);
					//printf("face %i - ",face2->index);
					switch(f1->id){
						case Feature::Vertex:
							//printf("vertex %i\n",v1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(vertex_face_test(v1, face2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}
							break;
						case Feature::Edge:
							//printf("edge %i\n",edge1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(edge_face_test(edge1, face2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}
							break;
						case Feature::Face:
							//printf("face %i\n",face1->index);
							if(z++ == stop){
									search = search_r2 = search_r1 = false;
								break;
							}
							if(face_face_test(face1, face2, poly1_trans, poly2_trans, f2)){
								search_r2 = false;
							}
							break;
					}

					break;
			}
		}
		switch(f2->id){
			case Feature::Vertex:
				v2 = static_cast<Vertex*>(f2);
				break;
			case Feature::Edge:
				edge2 = static_cast<Edge*>(f2);
				break;
			case Feature::Face:
				face2 = static_cast<Polygon*>(f2);
				break;
		}

		while(search_r1){
				dp ++;
				//printf("1, %i:", debug);
				switch(f1->id){
					case Feature::Vertex:
						v1 = static_cast<Vertex*>(f1);
						//printf("vertex %i - ",v1->index);
						switch(f2->id){
							case Feature::Vertex:
								//printf("vertex\n", v2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(vertex_vertex_test(v2, v1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}

								break;
							case Feature::Edge:
								//printf("edge %i\n",edge2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								closestPointEdge(v1, edge2, poly1_trans, poly2_trans,edge_point);
							if(vertex_vertex_test(&edge_point, v1, IDENITY, poly1_trans, f1)){
									search_r1 = false;
								}

								break;
							case Feature::Face:
								//printf("face %i\n", face2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								Vertex face_point;
								closestPointFace(v1, face2, poly1_trans, poly2_trans, face_point);
								//printf("closest point/face:\n");
								//printf("%f\n%f\n%f\n",face_point.x,face_point.y,face_point.z);
								if(vertex_vertex_test(&face_point, v1, IDENITY, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
						}
						break;
					case Feature::Edge:
						edge1 = static_cast<Edge*>(f1);
						//printf("edge %i- ",edge1->index);
						switch(f2->id){
							case Feature::Vertex:
								//printf("vertex\n");
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(vertex_edge_test(v2, edge1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
							case Feature::Edge:
								//printf("edge %i\n", edge2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(edge_edge_test(edge2, edge1, poly2_trans,poly1_trans, f1)){
									search_r1 = false;
								}
								break;
							case Feature::Face:
								//printf("face %i\n", face2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(face_edge_test(face2, edge1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
						}
						break;
					case Feature::Face:
						face1 = static_cast<Polygon*>(f1);
					//	printf("face %i- ", face1->index);
						switch(f2->id){
							case Feature::Vertex:
								//printf("vertex %i\n", v2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(vertex_face_test(v2, face1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
							case Feature::Edge:
								//printf("edge %i\n",edge2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(edge_face_test(edge2, face1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
							case Feature::Face:
								//printf("face %i\n", face2->index);
								if(z++ == stop){
									search = search_r2 = search_r1 = false;
									break;
								}
								if(face_face_test(face2, face1, poly2_trans, poly1_trans, f1)){
									search_r1 = false;
								}
								break;
						}
						break;
			}
		}
		if(confirmClosest(f1, f2, poly1_trans, poly2_trans)){
			//printf("closest confirmed!!\n");
			search = false;
		}
		else{
			search_r1 = search_r2 = true;
		}
	}

	double d;

	switch(f1->id){

		case Feature::Vertex:
			v1->transform(poly1_trans, pos1);
			switch(f2->id){
				case Feature::Vertex:
					(static_cast<Vertex*>(f2))->transform(poly2_trans, pos2);
					break;
				case Feature::Edge:
					d = closestPointEdge(v1, static_cast<Edge*>(f2), poly1_trans, poly2_trans, pos2);
					break;
				case Feature::Face:
					d = closestPointFace(v1, static_cast<Polygon*>(f2), poly1_trans, poly2_trans, pos2);
					break;
			}
			break;
		case Feature::Edge:
			switch(f2->id){
				case Feature::Vertex:
					closestPointEdge(static_cast<Vertex*>(f2), edge1, poly2_trans, poly1_trans, pos1);
					(static_cast<Vertex*>(f2))->transform(poly2_trans, pos2);
					break;
				case Feature::Edge:
					closestEdgeEdge(edge1, edge2, poly1_trans, poly2_trans, pos1, pos2);
					break;
				case Feature::Face:
					//hack so zero is not returned - needs fixin'
					((*edge1->head + *edge1->tail)/2.0).transform(poly1_trans, pos1);
					face2->d.transform(poly2_trans, pos2);
					break;


			}
			break;
		case Feature::Face:
			switch(f2->id){
				case Feature::Vertex:
					d = closestPointFace(static_cast<Vertex*>(f2), face1, poly2_trans, poly1_trans, pos1);
					(static_cast<Vertex*>(f2))->transform(poly2_trans, pos2);
					break;
				case Feature::Edge:
					//hack so zero is not returned - needs fixin'
					((*edge2->head + *edge2->tail)/2.0).transform(poly2_trans, pos1);
					face1->d.transform(poly1_trans, pos1);
					break;
				case Feature::Face:
					//hack so zero is not returned - needs fixin'
					face1->d.transform(poly1_trans, pos1);
					face2->d.transform(poly2_trans, pos2);
					break;
			}
			break;
	}

	if(dp > stop-1){
		///printf("OVERFLOW!!!\n");
		//while(true);
	}
	//if((pos1 - pos2).mag() < 0.0001){
	//	printf("f1 = %i, f2 = %i\n", f1->id, f2->id);
	//	printf("ZERO\n");
	//}

	return (pos1 - pos2).mag();
}

bool closestFeatures(Feature* f1, Feature* f2, unsigned int depth)
{
	unsigned int z = 0;
	uint8_t category = (f1->id<<3 || f2->id);
	bool done = false;

	while ( (z++ < depth) || !done )
	{
		switch (category)
		{
		case VertexVertexTest:
			Vertex* v1 = static_cast<Vertex*>(f1);
			Vertex* v2 = static_cast<Vertex*>(f2);
			done = vertex_vertex_test(v1, v2, poly1_trans, poly2_trans, f2);
			break;

		case EdgeEdgeTest:
			Edge* e1 = static_cast<Edge*>(f1);
			Edge* e2 = static_cast<Edge*>(f2);
			done = edge_edge_test(e1, e2, poly1_trans, poly2_trans, f2);
			break;

		case EdgeVertexTest:
			closestPointEdge(v2, edge1, poly2_trans, poly1_trans, edge_point);
			done = vertex_vertex_test(&edge_point, v2, IDENITY, poly2_trans, f2);
			break;

		case VertexEdgeTest:
			done = vertex_edge_test(v1, edge2, poly1_trans, poly2_trans, f2);
			break;

		case FaceFaceTest:
			done = face_face_test(face1, face2, poly1_trans, poly2_trans, f2);
			break;

		case FaceVertexTest:
			Vertex face_point;
			closestPointFace(v2, face1, poly2_trans, poly1_trans, face_point);
			done = vertex_vertex_test(&face_point, v2, IDENITY, poly2_trans, f2);
			break;

		case VertexFaceTest:
			done = vertex_face_test(v1, face2, poly1_trans, poly2_trans, f2);
			break;

		case FaceEdgeTest:
			done = face_edge_test(face1,edge2,poly1_trans,poly2_trans,f2) );
			break;

		case EdgeFaceTest:
			done = edge_face_test(edge1, face2, poly1_trans, poly2_trans, f2) );
			break;

		default:
			// Report error!
			break;
		}
	}

	return done;
}

bool confirmClosest(Feature* f1, Feature* f2, double* f1_trans, double* f2_trans){
	Vertex* v1, *v2;
	Edge* edge1, *edge2;
	Polygon* face1,* face2;
	Feature* f_nil;
	switch(f1->id){
		case Feature::Vertex:
			v1 = static_cast<Vertex*>(f1);
			break;
		case Feature::Edge:
			edge1 = static_cast<Edge*>(f1);
			break;
		case Feature::Face:
			face1 = static_cast<Polygon*>(f1);
			break;
	}

	Vertex face_point, edge_point1, edge_point2;
	switch(f2->id){
		case Feature::Vertex:
			v2 = static_cast<Vertex*>(f2);
			switch(f1->id){
				case Feature::Vertex:
					return vertex_vertex_test(v1, v2, f1_trans, f2_trans, f_nil);
				case Feature::Edge:
					closestPointEdge(v2, edge1, f2_trans, f1_trans, edge_point1);
					return vertex_vertex_test(&edge_point1, v2, IDENITY, f2_trans, f_nil);
				case Feature::Face:
					closestPointFace(v2, face1, f2_trans, f1_trans, face_point);
					return vertex_vertex_test(&face_point, v2, IDENITY, f2_trans, f2);
			}
			break;
		case Feature::Edge:
			edge2 = static_cast<Edge*>(f2);
			switch(f1->id){
				case Feature::Vertex:
					return vertex_edge_test(v1, edge2, f1_trans, f2_trans, f_nil);
				case Feature::Edge:
					return edge_edge_test(edge1, edge2, f1_trans, f2_trans, f_nil);
				case Feature::Face:
					return face_edge_test(face1, edge2, f1_trans, f2_trans,f_nil);
			}
			break;
		case Feature::Face:
			face2 = static_cast<Polygon*>(f2);
			switch(f1->id){
				case Feature::Vertex:
					return vertex_face_test(v1, face2, f1_trans, f2_trans, f_nil);
				case Feature::Edge:
					return edge_face_test(edge1, face2, f1_trans, f2_trans, f_nil);
				case Feature::Face:
					return face_face_test(face1, face2, f1_trans, f2_trans, f_nil);

			}
			break;
	}
	return true;
}

bool vertex_vertex_test(Vertex* p, Vertex* v, double*p_trans, double* v_trans, Feature* &f){
	Vertex p_global;
	Vertex v_global;
	p->transform(p_trans, p_global);
	v->transform(v_trans, v_global);
	for(int i=0;i<v->adjacency_size;i++){
		Edge* e = v->adjacency[i];
		VECTOR3D n;
		VECTOR3D head_global, tail_global;
		e->head->transform(v_trans, head_global);
		e->tail->transform(v_trans, tail_global);
		if(e->head->index == v->index){
			n = head_global - tail_global;
		}
		else{
			n = tail_global - head_global;
		}
		if(!pointInFrontOfPlane(v_global, n, p_global)){
			f = static_cast<Feature*>(e);
			return false;
		}
	}
	return true;
}


bool vertex_edge_test(Vertex* p, Edge* e, double* p_trans, double* e_trans, Feature* &f)
{
	Vertex p_global;
	Vertex head_global;
	Vertex tail_global;

	p->transform(p_trans, p_global);
	e->head->transform(e_trans, head_global);
	e->tail->transform(e_trans, tail_global);

	VECTOR3D n = tail_global - head_global;
	if(!pointInFrontOfPlane(head_global, n, p_global)){
		f = static_cast<Feature*>(e->head);
		return false;
	}

	n = head_global - tail_global;
	if(!pointInFrontOfPlane(tail_global, n, p_global)){
		f = static_cast<Feature*>(e->tail);
		return false;
	}
	//printf("8");
	n = tail_global - head_global;
	VECTOR3D face_n;
	e->left_face->n.rotate(e_trans, face_n);
	VECTOR3D cross = n.cross(face_n);
	if(!pointInFrontOfPlane(head_global, cross, p_global)){
		f = static_cast<Feature*>(e->left_face);
		return false;
	}

	n = head_global - tail_global;
//	printf("b");
//	printf("edge index = %i",e->index);
	e->right_face->n.rotate(e_trans, face_n);
//	printf("a");
	cross = n.cross(face_n);
	if(!pointInFrontOfPlane(tail_global, cross, p_global)){
		f = static_cast<Feature*>(e->right_face);
		return false;
	}
//	printf("true\n");
	f = static_cast<Feature*>(e);
	return true;
}

bool vertex_face_test(Vertex* p, Polygon* face, double* p_trans, double* face_trans, Feature* &f)
{
	Vertex p_global;
	p->transform(p_trans, p_global);
	VECTOR3D n;
	//printf("5");
	face->n.rotate(face_trans, n);

	for(int i=0;i<face->vertex_count;i++){
		Vertex head_global;
		Vertex tail_global;
		face->edges[i]->head->transform(face_trans, head_global);
		face->edges[i]->tail->transform(face_trans, tail_global);

		VECTOR3D cross;
		int hid = face->getVertexIndex(face->edges[i]->head) + 1;
		int tid = face->getVertexIndex(face->edges[i]->tail) + 1;
		if((tid)%face->vertex_count == (hid +1)%face->vertex_count){
			cross = n.cross(tail_global - head_global);
			if(!pointInFrontOfPlane(tail_global, cross, p_global)){
				f = static_cast<Feature*>(face->edges[i]);
				return false;
			}
		}
		else{
			cross = n.cross(head_global - tail_global);
			if(!pointInFrontOfPlane(head_global, cross, p_global)){
				f = static_cast<Feature*>(face->edges[i]);
				return false;
			}
		}
	}

	VECTOR3D pos;
	face->d.transform(face_trans, pos);
	if(!pointInFrontOfPlane(pos,n,p_global)){
		//printf("BEHIND_PLANE: %i\n",face->index);
		Edge* e = face->edges[face->idc_edge];
		face->idc_edge++;
		face->idc_edge %= face->vertex_count;
		if(face->getEdgeSide(e) == LEFT_FACE){
			f = static_cast<Feature*>(e->right_face);
		//	printf("R:face id = %i\n",e->right_face->index);
		}
		else{
			f = static_cast<Feature*>(e->left_face);
			//printf("L:face id = %i\n",e->left_face->index);
		}
		return false;
	}

	return true;

}

bool edge_edge_test(Edge* e1, Edge* e2, double* e1_trans, double* e2_trans, Feature* &f)
{
	Vertex p1, p2;
	closestEdgeEdge(e1, e2, e1_trans, e2_trans, p1, p2);
//	printf("closest edge/edge:\n");
//	printf("%f, %f, %f\n",p1.x,p1.y,p1.z);
//	printf("%f, %f, %f\n",p2.x,p2.y,p2.z);
	return vertex_edge_test(&p1, e2, IDENITY, e2_trans, f);
}

bool face_edge_test(Polygon* face, Edge* edge, double* face_trans, double* edge_trans, Feature* &f){
	if(parallelEdgeFace(edge, face, edge_trans, face_trans)){
		//printf("PARALLEL: face-edge!!! \n");

		Vertex p1;
		int f_id;
		if(edgeVisible(edge, &face->d, edge_trans, face_trans, f_id)){
			return true;
		}
		double head_dist = closestPointFace(edge->head, face, edge_trans, face_trans, p1);
		double tail_dist = closestPointFace(edge->tail, face, edge_trans, face_trans, p1);

		if(head_dist < tail_dist){
			f = static_cast<Feature*>(edge->head);
			return false;
		}
		else{
			f = static_cast<Feature*>(edge->tail);
			return false;
		}
		/*
		int face_id;
		if(edgeVisible(edge, &ZERO_VECTOR, edge_trans, face_trans, face_id)){
			return true;
		}
		*/
		/*	if(vertex_face_test(&p1, face, edge_trans, face_trans,f)){
			if(vertex_face_test(edge->head, face, edge_trans,face_trans, f)){
				printf("1\n");
				f = static_cast<Feature*>(edge->head);
				return false;
			}
			if(vertex_face_test(edge->tail, face, edge_trans, face_trans, f)){
				printf("2\n");
				f = static_cast<Feature*>(edge->tail);
				return false;
			}


			else{
				if(face_id == LEFT_FACE){
					printf("3\n");
					f = static_cast<Feature*>(edge->left_face);
				}
				else{
					printf("4\n");
					f = static_cast<Feature*>(edge->right_face);
				}
				return false;
			}

		}
		else{
			int face_id;
			edgeVisible(edge, &ZERO_VECTOR, edge_trans, face_trans, face_id);
			if(face_id == LEFT_FACE){
				printf("3b\n");
				f = static_cast<Feature*>(edge->left_face);
			}
			else{
				printf("4b\n");
				f = static_cast<Feature*>(edge->right_face);
			}
			return false;
		}
		*/
	}
	Vertex pos;
	Vertex head_global, tail_global, n_global;
	face->d.transform(face_trans, pos);
	edge->head->transform(edge_trans, head_global);
	edge->tail->transform(edge_trans, tail_global);
	face->n.rotate(face_trans, n_global);

	bool head, tail;
	double head_dist, tail_dist;
	head = tail = false;

	if(vertex_face_test(edge->head, face, edge_trans, face_trans, f)){
		head = true;
		head_dist = closestPointFace(edge->head,face, edge_trans, face_trans, pos);
	}

	if(vertex_face_test(edge->tail, face, edge_trans, face_trans, f)){
		tail = true;
		tail_dist = closestPointFace(edge->tail,face, edge_trans, face_trans, pos);
	}

	if(head && tail){
		if(head_dist < tail_dist){
			f = static_cast<Feature*>(edge->head);
			return false;
		}
		else{
			f = static_cast<Feature*>(edge->tail);
			return false;
		}
	}
	else{
		if(head){
			if((head_global-tail_global).dot(n_global)< 0.00 ){
				f = static_cast<Feature*>(edge->head);
				return false;
			}
			else{
				f = static_cast<Feature*>(edge);
				return true;
			}

		}
		else if(tail){
			if((tail_global-head_global).dot(n_global)< 0.00 ){
				f = static_cast<Feature*>(edge->tail);
				return false;
			}
			else{
				f = static_cast<Feature*>(edge);
				return true;
			}
		}
	}
	f = static_cast<Feature*>(edge);
	return true;
}


bool edge_face_test(Edge* edge, Polygon* face, double* edge_trans, double* face_trans, Feature* &f){
	if(!face_edge_test(face,edge,face_trans, edge_trans,f)){
		f = static_cast<Feature*>(face);
		return true;
	}

	if(parallelEdgeFace(edge, face, edge_trans, face_trans)){
		/****
		FIX THIS!!!!!
		****/
		/*
		printf("PARALLEL: edge-face\n");
		Vertex p1;
		closestPointFace(edge->head, face, edge_trans, face_trans, p1);
		if(vertex_face_test(&p1, face, edge_trans, face_trans,f)){
			//f = static_cast<Feature*>(edge);
			return true;
		}
		//f = static_cast<Feature*>(edge->head);
		return false;
	*///	printf("PARALLEL: edge-face\n");
		return true;
	}

	Vertex p1, p2;
	int closest_index = 0;
	double closest = closestEdgeEdge(edge, face->edges[0], edge_trans, face_trans, p1, p2);

	for(int i=1;i<face->vertex_count;i++){
		double d = closestEdgeEdge(edge, face->edges[i], edge_trans, face_trans, p1, p2);
		if(d < closest){
			closest = d;
			closest_index = i;
		}
	}
	f = static_cast<Feature*>(face->edges[closest_index]);
	return false;
}

bool face_face_test(Polygon* face1, Polygon* face2, double* face1_trans, double* face2_trans, Feature* &f){
	if( parallelFaceFace(face1, face2, face1_trans, face2_trans) )
	{
		return overlapTest(face1, face2,face1_trans,face2_trans, f);
	}

	Vertex p_closest;
	double min_dist = closestPointFace(face1->vertices[0], face2, face1_trans, face2_trans, p_closest);

	Vertex p_temp;
	for(int i=1;i<face1->vertex_count;i++){
		double temp_dist  = closestPointFace(face1->vertices[0], face2, face1_trans, face2_trans, p_temp);
		if(temp_dist < min_dist){
			min_dist = temp_dist;
			p_closest = p_temp;
		}
	}
	return vertex_face_test(&p_closest, face2, IDENITY, face2_trans, f);
}

double closestPointEdge(Vertex* p, Edge* e, double* p_trans, double* e_trans, Vertex &v)
{
	Vertex p_global, head_global, tail_global;
	p->transform(p_trans, p_global);
	e->head->transform(e_trans, head_global);
	e->tail->transform(e_trans, tail_global);
	VECTOR3D ev = tail_global - head_global;
	ev.normalize();
	VECTOR3D pv = p_global-head_global;
	double d = pv.dot(ev);

	v = head_global + ev*d;

	VECTOR3D a = v - head_global;
	VECTOR3D b = tail_global - head_global;

	if(a.dot(b) < 0.0){
		v = head_global;
	}
	else{
		double e_mag = (tail_global - head_global).magSquared();
		double v_mag = (v - head_global).magSquared();
		if(v_mag > e_mag){
			v = tail_global;
		}
	}
	return (p_global - v).mag();
}

double closestPointFace(Vertex* p, Polygon* f, double* p_trans, double* f_trans, Vertex &v){
	Vertex p_global, d_global;
	p->transform(p_trans, p_global);
	f->d.transform(f_trans, d_global);
	VECTOR3D pv = p_global - d_global;
	VECTOR3D nv;
//	printf("1");
	f->n.rotate(f_trans, nv);
	double d = pv.dot(nv);
	v = p_global - nv*d;
	return (p_global - v).mag();


	/*
	Vertex p_global;
	p->transform(p_trans, p_global);
	VECTOR3D n;
	face->n.rotate(face_trans, n);

	for(int i=0;i<face->vertex_count;i++){
		Vertex head_global;
		Vertex tail_global;
		face->edges[i]->head->transform(face_trans, head_global);
		face->edges[i]->tail->transform(face_trans, tail_global);

		VECTOR3D cross;
		int hid = face->getVertexIndex(face->edges[i]->head) + 1;
		int tid = face->getVertexIndex(face->edges[i]->tail) + 1;
		if((tid)%face->vertex_count == (hid +1)%face->vertex_count){
			cross = n.cross(tail_global - head_global);
			if(!pointInFrontOfPlane(tail_global, cross, p_global)){
				f = static_cast<Feature*>(face->edges[i]);
				return false;
			}
		}
		else{
			cross = n.cross(head_global - tail_global);
			if(!pointInFrontOfPlane(tail_global, cross, p_global)){
				f = static_cast<Feature*>(face->edges[i]);
				return false;
			}
		}
	}
	*/
}


double closestEdgeEdge(Edge* e1, Edge* e2, double* e1_trans, double* e2_trans, Vertex &v1, Vertex &v2)
{
	VECTOR3D e1_head_global, e1_tail_global;
	VECTOR3D e2_head_global, e2_tail_global;

	e1->head->transform(e1_trans, e1_head_global);
	e1->tail->transform(e1_trans, e1_tail_global);
	VECTOR3D u = e1_tail_global - e1_head_global;

	e2->head->transform(e2_trans, e2_head_global);
	e2->tail->transform(e2_trans, e2_tail_global);
	VECTOR3D v = e2_tail_global - e2_head_global;

	VECTOR3D w = e1_head_global - e2_head_global;

	double a = u.magSquared();
	double b = u.dot(v);
	double c = v.magSquared();
	double d = u.dot(w);
	double e = v.dot(w);
	double denom = a*c - b*b;

	double sc, sN, sD = denom;
	double tc, tN, tD = denom;

	if(denom < EPS){
		sN = 0.0;
		sD = 1.0;
		tN = e;
		tD = c;
	}
	else{
		sN = (b*e - c*d);
		tN = (a*e - b*d);
		if(sN < 0.0){
			sN = 0.0;
            tN = e;
            tD = c;
		}
		else if(sN > sD){
            sN = sD;
            tN = e + b;
            tD = c;
        }
	}
	if(tN < 0.0) {
        tN = 0.0;
		if(-d < 0.0){
            sN = 0.0;
		}
		else if(-d > a){
            sN = sD;
		}
        else{
            sN = -d;
            sD = a;
        }
    }
	else if (tN > tD) {
       tN = tD;
	   if((-d + b) < 0.0){
           sN = 0;
	   }
	   else if((-d + b) > a){
           sN = sD;
	   }
       else{
           sN = (-d + b);
           sD = a;
       }
    }
	sc = (abs(sN) < EPS ? 0.0 : sN / sD);
    tc = (abs(tN) < EPS ? 0.0 : tN / tD);


	v1 = e1_head_global + u*sc;
	v2 = e2_head_global + v*tc;

	return (v2-v1).mag();
}

bool pointInFrontOfPlane(VECTOR3D pos, VECTOR3D n, Vertex p){
	//printf("d = %f\n", (n).dot(p-pos));
	return ((n).dot(p-pos) > -.001);
}

bool edgeVisible(Edge* e, Vertex* p, double* e_trans, double* p_trans, int& face_id){
	Vertex disp, orign, left_global, right_global, p_global, e_global;
	e->left_face->n.rotate(e_trans, left_global);
	e->right_face->n.rotate(e_trans, right_global);
	p->transform(p_trans, p_global);
	orign.transform(e_trans, e_global);
	disp = p_global - e_global;
	double d_left = disp.dot(left_global);
	double d_right = disp.dot(right_global);

	if(d_left < d_right){
		face_id = LEFT_FACE;
	}
	else{
		face_id = RIGHT_FACE;
	}

	if(d_left > -EPS && d_right > -EPS){
		return true;
	}
	else{
		return false;
	}
}

bool parallelEdgeFace(Edge* e, Polygon* f , double* e_trans, double* f_trans){
	VECTOR3D head_global, tail_global, n_global;
	e->head->transform(e_trans, head_global);
	e->tail->transform(e_trans, tail_global);
	f->n.rotate(f_trans, n_global);
	double d = (tail_global - head_global).dot(n_global);
	return (d >-EPS && d<EPS);
}

bool parallelFaceFace(Polygon* face1, Polygon* face2, double* face1_trans, double* face2_trans){
	VECTOR3D n1_global, n2_global;
	face1->d.rotate(face1_trans, n1_global);
	face2->d.rotate(face2_trans, n2_global);
	double d = n1_global.dot(n2_global);
	if((d > 1.0-EPS && d <1.0+EPS) || (d > -1.0-EPS && d <-1.0+EPS)){
		return true;
	}
	return false;
}

bool overlapTest(Polygon* f1, Polygon* f2, double* f1_trans, double* f2_trans, Feature* &f){
	for(int i=0;i<f2->vertex_count;i++){
		if(vertex_face_test(f2->vertices[i], f1, f2_trans, f1_trans, f)){
			return true;
		}
	}
	for(int i=0;i<f1->vertex_count;i++){
		if(vertex_face_test(f1->vertices[i], f2, f1_trans, f2_trans, f)){
			f = static_cast<Feature*>(f2);
			return true;
		}
	}
	return false;
}
