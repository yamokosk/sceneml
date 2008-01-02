function fv = createPlane(p, params)
% T - Transformation matrix
% p - [xmin, xmax, ymin, ymax]
% params - a,b,c,d of the plane equation
% fv - vertex/face structure
% NOTE - Body coordinate system placed and center of volume
xmin = p(1); xmax = p(2); ymin = p(3); ymax = p(4);
a = params(1);  b = params(2); c = params(3); d = params(4);
z = zeros(1,4);
z(1) = (d - a*xmax - b*ymax)/c;
z(2) = (d - a*xmin - b*ymax)/c;
z(3) = (d - a*xmin - b*ymin)/c;
z(4) = (d - a*xmax - b*ymin)/c;

% Non-homogeneous vertices
fv.vertices = [xmax, xmin, xmin, xmax, xmax, xmin, xmin, xmax; ...
               ymax, ymax, ymin, ymin, ymax, ymax, ymin, ymin; ...
                z(1), z(2), z(3), z(4), z(1)-10, z(2)-10, z(3)-10, z(4)-10]';

% Create a new patch object
fv.faces = [1, 2, 3, 4; 5, 6, 7, 8; 4, 3, 7, 8; 1, 4, 8, 5; 1, 2, 6, 5; 2, 3, 7, 6];
