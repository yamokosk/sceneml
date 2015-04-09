# SceneML 1.4 Notes #

## Change Log ##
  * Transparent rendering can be done in two ways.
    * Either through the XML file with a new parameter for geoms. If not specified, default = 1
```
<geometry>
<parameter type="alpha" value="0" /> <!-- any value between 0 (invisible) and 1 (opaque) -->
</geometry>
```
    * Or real-time with the matlab function:
```
sceneSetGeomAlpha('geom_name', alpha); % alpha is between 0 (invisible) and 1 (opaque)
```
  * Turn collision checking off/on for a particular geom can also be done in one of two ways:
    * Either through the XML file with a new parameter for geoms. If not specified, default = 1
```
<geometry>
<parameter type="checkcollision" value="0" /> <!-- 0 for no checking, 1 for checking -->
</geometry>
```
    * Or real-time in Matlab with the following function:
```
sceneSetGeomCFlag('geom_name', 0); % turns collision checking off for this geom
sceneSetGeomCFlag('geom_name', 1); % turns collision checking back on for this geom
```
    * Get the value of a mutable variable in Matlab:
```
[x,y,z] = sceneGetVarValue('variable_name'); % for vector-valued mutable variables
x = sceneGetVarValue('variable_name'); % for scalar mutable variables
```
  * The geometry structure returned in Matlab now reports the new transparent and collision flag:
```
g = sceneGetGeom('src_stand_top')

g =

              name: 'src_stand_top'
              type: 1
              body: 'src_stand'
       T_body_geom: [4x4 double]
      T_world_geom: [4x4 double]
           ghandle: 0
            params: [1x1 struct]
                fv: 0
             color: [190 190 190]
             alpha: 1
    collisioncheck: 0
```

## Porting notes ##

## Annotated roadmap ##