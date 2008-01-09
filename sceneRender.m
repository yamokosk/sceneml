function fig = sceneRender(varargin)

fig = gcf;
if ( ~strcmp('xode', get(fig, 'UserData')) )
    close(fig);
    fig = createWindow(varargin);
end

patches = findobj(gca,'Type','patch');
if (isempty(patches))
    % if the patches don't already exist, create them
    geomData = sceneGetAllGeoms();
    for n=1:length(geomData)
       [fv,color] = createGeom(geomData{n});
       h = patch(fv, 'FaceColor', color);
       set(h, 'FaceLighting', 'flat');
       set(h, 'EdgeColor', 'none');
       set(h, 'UserData', geomData{n}.name);
        
%        drawCoordinateSystem(fig, geomData{n}.T_world_geom, geomData{n}.name);
    end
else
    % otherwise just update vertex data
    names = get(patches, 'UserData');
    for n=1:length(patches)
        geomData = sceneGetGeom(names{n});
        [fv,color] = createGeom(geomData);
        set(patches(n), 'Vertices', fv.vertices);
        set(patches(n), 'FaceColor', color);
        
%        drawCoordinateSystem(fig, geomData.T_world_geom, geomData.name);
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


function [fv, c] = createGeom(geom)
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
        %T = geom.T_world_geom;
        %nv = size(geom.fv.vertices, 1);
        %vtemp = [geom.fv.vertices'; ones(1, nv)];
        %vtemp = T * vtemp;
        %fv.vertices = vtemp(1:3,:)';
        fv.vertices = geom.fv.vertices;
        fv.faces = geom.fv.faces;        
    otherwise
        warning([geom.type ' is an unrecognized geometry type']);
end
c = geom.color/255;

end % End of drawGeom()


function fig = createWindow(varargin)
fig = figure('Position', [50, 50, 1024, 768], ...
             'Renderer', 'zbuffer', ...
             'UserData', 'xode');
nopts = size(varargin{1},2);
for n = 1:2:nopts
    set(fig, varargin{1}(n), varargin{1}(n+1));
end


% Scene axes
set(gca, 'DataAspectRatio',   [1, 1, 1], ...
         'CameraPosition',    [1000,1000,500], ...
         'CameraTarget',      [0,0,200], ...
         'CameraUpVector',    [0,0,1], ...
         'Visible',           'off');
         %'XLimMode',          'manual', ...
         %'YLimMode',          'manual', ...
         %'ZLimMode',          'manual', ...
         %'XLim',              [-400 400], ...
         %'YLim',              [-400 400], ...
         %'ZLim',              [-400 400]);
light('Position',[0 0 2000],'Style','infinite');
end % End createWindow()