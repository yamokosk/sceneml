function fv = createCylinder(T, len, r)
% T - Transformation matrix
% len - length of the cylinder
% fv - vertex/face structure
% NOTE - Body coordinate system placed and center of volume
N = 20;
[x y z] = cylinder(r, N); 
fv = surf2patch(x,y,z,z);
NV = size(fv.vertices,1);

a = union(fv.faces(:,1), fv.faces(:,2));
amean = mean(fv.vertices(a,:));
afaces = zeros(length(a),4);

b = union(fv.faces(:,3), fv.faces(:,4));
bmean = mean(fv.vertices(b,:));
bfaces = zeros(length(b),4);

fv.vertices(end+1,:) = amean;
fv.vertices(end+1,:) = bmean;

for n = 2:length(a)
    afaces(n-1,:) = [a(n-1), a(n), NV+1, a(n-1)];
    bfaces(n-1,:) = [b(n-1), b(n), NV+2, b(n-1)];
end

afaces(end,:) = [a(end), a(1), NV+1, a(end)];
bfaces(end,:) = [b(end), b(1), NV+1, b(end)];

fv.faces = [fv.faces; afaces; bfaces];
NV = NV + 2;

% Homogenous vertices - hv
hv = ones(4,NV);
hv(1:3,:) = fv.vertices';

% Scaling to go from unit length to len
S = eye(4); S(3,3) = len;

% Offset the body coordinate system to center of volume
F = eye(4); F(3,4) = -0.5*len;

% Transformed vertices
hv = T*F*S*hv;
fv.vertices = hv(1:3,:)';
