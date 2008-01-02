function drawCoordinateSystem(h, T, str)
origin = T(1:3,4);
v = T * [100,0,0,1]'; xpt = v(1:3,1); 
xaxis = [origin'; xpt'];

v = T * [0,100,0,1]'; ypt = v(1:3,1);
yaxis = [origin'; ypt'];

v = T * [0,0,100,1]'; zpt = v(1:3,1);
zaxis = [origin'; zpt'];

figure(h);
line(xaxis(:,1), xaxis(:,2), xaxis(:,3), 'Color', 'r');
line(yaxis(:,1), yaxis(:,2), yaxis(:,3), 'Color', 'g');
line(zaxis(:,1), zaxis(:,2), zaxis(:,3), 'Color', 'b');
text(origin(1), origin(2), origin(3), str);
