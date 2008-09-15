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

#ifndef _MEX_STRUCT_DEF_H_
#define _MEX_STRUCT_DEF_H_

/*
 * Body field definition
 */
#define BODY_DEF	FIELD(NAME, "name")					\
					FIELD(PROXBODY, "proxBody")			\
					FIELD(DISTBODIES, "distBodies")		\
					FIELD(GEOMS, "geoms")				\
					FIELD(TPROXBODY, "T_prox_body")		\
					FIELD(TWORLDBODY, "T_world_body")

enum body_indices {
#define	FIELD(ID, name) BODY_##ID,
	BODY_DEF
#undef FIELD
    NUM_BODY_FIELDS /* The number of existing commands */
};

const char *fnames_body[] = {
#define	FIELD(ID, name) name ,
	BODY_DEF
#undef FIELD
	NULL
};

/*
 * Geom field definition
 */
#define GEOM_DEF	FIELD(NAME, "name")					\
					FIELD(TYPE, "type")					\
					FIELD(BODY, "body")					\
					FIELD(TBODYGEOM, "T_body_geom")		\
					FIELD(TWORLDGEOM, "T_world_geom")	\
					FIELD(GHANDLE, "ghandle")			\
					FIELD(PARAMS, "params")				\
					FIELD(FV, "fv")						\
					FIELD(COLOR, "color")				\
					FIELD(ALPHA, "alpha")				\
					FIELD(COLLISIONFLAG, "collisioncheck")

enum geom_indices {
#define	FIELD(ID, name) GEOM_##ID,
	GEOM_DEF
#undef FIELD
    NUM_GEOM_FIELDS
};

const char *fnames_geom[] = {
#define	FIELD(ID, name) name ,
	GEOM_DEF
#undef FIELD
	NULL
};

/*
 * Box field definition
 */
#define BOX_DEF		FIELD(LENGTH, "length")				\
					FIELD(WIDTH, "width")				\
					FIELD(HEIGHT, "height")			

enum box_indices {
#define	FIELD(ID, name) GEOM_BOX_##ID,
	BOX_DEF
#undef FIELD
    NUM_GEOM_BOX_FIELDS
};

const char *fnames_geom_box[] = {
#define	FIELD(ID, name) name ,
	BOX_DEF
#undef FIELD
	NULL
};

/*
 * Sphere field definition
 */			
#define SPHERE_DEF	FIELD(RADIUS, "radius")

enum sphere_indices {
#define	FIELD(ID, name) GEOM_SPHERE_##ID,
	SPHERE_DEF
#undef FIELD
    NUM_GEOM_SPHERE_FIELDS
};

const char *fnames_geom_sphere[] = {
#define	FIELD(ID, name) name ,
	SPHERE_DEF
#undef FIELD
	NULL
};

/*
 * Capsule field definition
 */	
#define CAPSULE_DEF	FIELD(RADIUS, "radius")				\
					FIELD(LENGTH, "length")			

enum capsule_indices {
#define	FIELD(ID, name) GEOM_CAPSULE_##ID,
	CAPSULE_DEF
#undef FIELD
    NUM_GEOM_CAPSULE_FIELDS
};

const char *fnames_geom_capsule[] = {
#define	FIELD(ID, name) name ,
	CAPSULE_DEF
#undef FIELD
	NULL
};

/*
 * Cylinder field definition
 */	
#define CYLINDER_DEF	FIELD(RADIUS, "radius")			\
						FIELD(LENGTH, "length")						

enum cylinder_indices {
#define	FIELD(ID, name) GEOM_CYLINDER_##ID,
	CYLINDER_DEF
#undef FIELD
    NUM_GEOM_CYLINDER_FIELDS
};

const char *fnames_geom_cylinder[] = {
#define	FIELD(ID, name) name ,
	CYLINDER_DEF
#undef FIELD
	NULL
};

/*
 * Trimesh field definition
 */	
#define TRIMESH_DEF	FIELD(FILENAME, "filename")

enum trimesh_indices {
#define	FIELD(ID, name) GEOM_TRIMESH_##ID,
	TRIMESH_DEF
#undef FIELD
    NUM_GEOM_TRIMESH_FIELDS
};

const char *fnames_geom_trimesh[] = {
#define	FIELD(ID, name) name ,
	TRIMESH_DEF
#undef FIELD
	NULL
};

/*
 * Plane field definition
 */	
#define PLANE_DEF	FIELD(NORMALX, "normal_x")			\
					FIELD(NORMALY, "normal_y")			\
					FIELD(NORMALZ, "normal_z")			\
					FIELD(D, "d")

enum plane_indices {
#define	FIELD(ID, name) GEOM_PLANE_##ID,
	PLANE_DEF
#undef FIELD
    NUM_GEOM_PLANE_FIELDS
};

const char *fnames_geom_plane[] = {
#define	FIELD(ID, name) name ,
	PLANE_DEF
#undef FIELD
	NULL
};

/*
 * Contact field definition
 */					
#define CONTACT_DEF	FIELD(POSITION, "position")			\
					FIELD(NORMAL, "normal")				\
					FIELD(DEPTH, "depth")				\
					FIELD(GEOM1, "geom1")				\
					FIELD(GEOM2, "geom2")

enum contact_indices {
#define	FIELD(ID, name) CONTACT_##ID,
	CONTACT_DEF
#undef FIELD
    NUM_CONTACT_FIELDS
};

const char *fnames_contact[] = {
#define	FIELD(ID, name) name ,
	CONTACT_DEF
#undef FIELD
	NULL
};

/*
 * Face/vertex field definition
 */	
#define FV_DEF		FIELD(FACES, "faces")				\
					FIELD(VERTICES, "vertices")

enum fv_indices {
#define	FIELD(ID, name) FV_##ID,
	FV_DEF
#undef FIELD
    NUM_FV_FIELDS
};

const char *fnames_fv[] = {
#define	FIELD(ID, name) name ,
	FV_DEF
#undef FIELD
	NULL
};
					
#endif
