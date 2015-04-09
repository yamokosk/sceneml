# SceneML 1.5 Notes #

## Change Log ##
  * Hopefully some performance improvements were made to rendering in Matlab.
  * Slight change to format of data passed back by sceneGetAllGeoms().
  * New Matlab function: sceneGetGeomHandle. Allows you to get a hold of the graphics handle for the object to manipulate its appearance in the scene.. for instance if its in collision! Here's a quick example:
```
if ( sceneCollisionState() )
    c = sceneGetContactData(); % Get the skinny on all objects in collision
     
    for n = 1:length(c)
        h = sceneGetGeomHandle( c(n).geom1 );
        set(h, 'FaceColor', [1, 0, 0]); % Set geom1 face color to red
        h = sceneGetGeomHandle( c(n).geom2 );
        set(h, 'FaceColor', [1, 0, 0]); % Set geom2 face color to red  
    end
end
```


## Porting notes ##
  * Ha! Finally some porting notes!
  * There is a slight change to the Matlab function 'sceneGetAllGeoms()'. It used to return a cell array of structures. However it now returns just a simple array of structures. Here are some examples of what this kind of change means:
```
% Version 1.4 way of doing things
g = sceneGetAllGeoms();
for n = 1:length(g)
    g{n}.alpha
end

% Version 1.5 way of doing things
g = sceneGetAllGeoms();
for n = 1:length(g)
    g(n).alpha
end
```
  * Also different is that sceneRender will **not** color objects red when they are in collision. Instead that is now being left up to the user's discretion. Along with the new function added, coloring objects should now be pretty straight forward.
## Annotated roadmap ##