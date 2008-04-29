function fig = sceneRender(varargin)
% sceneRender - Renders a scene loaded by SceneML
%
%   fig = sceneRender() renders the scene currently stored in the SceneML
%   library and then returns the figure handle associated with the Matlab
%   window.
%
%   fig = sceneRender('paramName1', paramValue1, ...) passes additional
%   parameters to the Matlab figure window when its created for the first
%   time. Consult Matlab's documentation for more information on the
%   various figure properties.
%
%   Subsequent calls to sceneRender will reuse the same figure window.
%
%   Example:
%       sceneInit('demo/twopa10.scene');
%       h = sceneRender();
%
%   See also sceneInit

% SceneML, Copyright (C) 2007, 2008  J.D. Yamokoski
% All rights reserved.
% Email: yamokosk at gmail dot com
%
% This library is free software; you can redistribute it and/or
% modify it under the terms of the GNU Lesser General Public License as
% published by the Free Software Foundation; either version 2.1 of the License, 
% or (at your option) any later version. The text of the GNU Lesser General 
% Public License is included with this library in the file LICENSE.TXT.
%
% This library is distributed in the hope that it will be useful, but WITHOUT 
% ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
% or FITNESS FOR A PARTICULAR PURPOSE. See the file LICENSE.TXT for 
% more details.

fh = get(0,'Children');     % Get handles to all figure windows
% Now loop through and check to see if a SceneML figure window is open. If
% so, its a good idea to close it since problems can occur if we load a
% different scene description file.
oldFigNumber = -1;
for n = 1:length(fh)
    fig = fh(n);
    figUserData = get(fig, 'UserData');
    if ( ischar(figUserData) )
        if ( strcmp('sceneml', figUserData) )
            oldFigNumber = fig;
            break;
        end
    end
end

if (oldFigNumber < 0)
    fig = createWindow(varargin);
    % Draw the world coordinate system, for reference
    if (ismac)
        drawCoordinateSystem(fig, eye(4), 'world', 10);
    else
        aabb = sceneGetAABB();
        d = [aabb(2)-aabb(1), aabb(4)-aabb(3), aabb(6)-aabb(5)];
        scale = max(d);
        drawCoordinateSystem(fig, eye(4), 'world', scale);
    end
else
    figure(oldFigNumber);
end

patches = findobj(gca,'Type','patch');
if (isempty(patches))
    % if the patches don't already exist, create them
    geomData = sceneGetAllGeoms();
    for n=1:length(geomData)
       [fv,color,alpha] = createGeom(geomData{n});
       h = patch(fv, 'FaceColor', color);
       set(h, 'FaceLighting', 'flat');
       set(h, 'EdgeColor', 'none');
       set(h, 'EdgeAlpha', alpha);
       set(h, 'FaceAlpha', alpha);
       set(h, 'UserData', geomData{n}.name);
    end
else
    % otherwise just update vertex data
    names = get(patches, 'UserData');
    for n=1:length(patches)
        geomData = sceneGetGeom(names{n});
        [fv,color,alpha] = createGeom(geomData);
        set(patches(n), 'Vertices', fv.vertices);
        set(patches(n), 'FaceColor', color);
        set(patches(n), 'EdgeAlpha', alpha);
        set(patches(n), 'FaceAlpha', alpha);
    end
end

% Now check if any geoms are in collision.. if so color them differently
if ( sceneCollisionState() )
    c = sceneGetContactData();
    
    patches = findobj(gca,'Type','patch');
    names = get(patches, 'UserData');
    for n = 1:length(c)
        h = patches( find( strcmp(c(n).geom1, names) ) );
        set(h, 'FaceColor', [1, 0, 0]);
        h = patches( find( strcmp(c(n).geom2, names) ) );
        set(h, 'FaceColor', [1, 0, 0]);     
    end
end

drawnow;

end % End drawScene()


function [fv, c, alpha] = createGeom(geom)
% dSphereClass          0   Sphere
% dBoxClass             1   Box
% dCapsuleClass         2 	Capsule (i.e. cylinder with half-sphere caps at its ends)
% dCylinderClass        3	Regular flag ended Cylinder
% dPlaneClass           4   Infinite plane (non-placeable)
% dRayClass             5   Ray
% dConvexClass          6	
% dGeomTransformClass   7	Geometry transform
% dTrimeshClass         8	Triangle mesh

% Quickly handle if it is a plane
if ( geom.type == 4 )
    fv = createPlane([-2000, 2000, -4000, 4000], [geom.params.normal_x,geom.params.normal_y,geom.params.normal_z,geom.params.d]);
    c = geom.color/255;
    alpha = geom.alpha;
    return;
end

switch (geom.type)
    case 0  % sphere
        fv = createSphere(geom.T_world_geom, geom.params.radius);
    case 1  % box
        fv = createBox(geom.T_world_geom, [geom.params.length, geom.params.width, geom.params.height]);
    case 2  % capsule
        fv = createCapsule(geom.T_world_geom, geom.params.length, geom.params.radius);
    case 3  % cylinder
        fv = createCylinder(geom.T_world_geom, geom.params.length, geom.params.radius);
    case 8 % trimesh
        fv.vertices = geom.fv.vertices;
        fv.faces = geom.fv.faces;        
    otherwise
        warning([geom.type ' is an unrecognized geometry type']);
end
c = geom.color/255;
alpha = geom.alpha;

end % End of drawGeom()


function fig = createWindow(varargin)

fig = figure('Position', [50, 50, 1024, 768], ...
             'Renderer', 'OpenGL', ...
             'UserData', 'sceneml');
nopts = size(varargin{1},2);
for n = 1:2:nopts
    set(fig, varargin{1}(n), varargin{1}(n+1));
end


% Scene axes
if (ismac())
    set(gca, 'DataAspectRatio', [1,1,1], ...
             'Visible', 'off');
	view(130,30);
    light('Position',[0 0 400],'Style','infinite');
else
    aabb = sceneGetAABB();

    cpos = [aabb(2), aabb(4), aabb(6)];
    centroid = [aabb(1), aabb(3), aabb(5)];
    fov = pi/3;

    set(gca, 'DataAspectRatio',   [1, 1, 1], ...
             'CameraPosition',    cpos, ...
             'CameraTarget',      centroid, ...
             'CameraViewAngle',   fov, ...
             'CameraUpVector',    [0,0,1], ...
             'Visible',           'off');
         
    light('Position',[0 0 aabb(6)*20],'Style','infinite');
end



end % End createWindow()