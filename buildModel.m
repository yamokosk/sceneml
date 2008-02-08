function [localMarkers, globalMarkers] = buildModel()

[FileName,PathName] = uigetfile('*.c3d','Select the static cal file');
calData = loadC3DFile(fullfile(PathName, FileName));
%calData = loadC3DFile('/home/yamokosk/research/1218/Processed/TPose4.c3d');

mData_bar = [mean(calData.mData(:,:,1)); ...
             mean(calData.mData(:,:,2)); ...
             mean(calData.mData(:,:,3))];
         
% fig = figure('Position', [50, 50, 1024, 768], ...
%              'Renderer', 'OpenGL', ...
%              'UserData', 'xode');
% 
% % Scene axes
% set(gca, 'DataAspectRatio',   [1, 1, 1], ...
%          'CameraPosition',    [1000,1000,500], ...
%          'CameraTarget',      [0,0,200], ...
%          'CameraUpVector',    [0,0,1], ...
%          'Visible',           'off');
%          %'XLimMode',          'manual', ...
%          %'YLimMode',          'manual', ...
%          %'ZLimMode',          'manual', ...
%          %'XLim',              [-400 400], ...
%          %'YLim',              [-400 400], ...
%          %'ZLim',              [-400 400]);
% light('Position',[0 0 2000],'Style','infinite');
% 
% for n = 1:calData.nMarkers
%     T = eye(4); T(1:3,4) = mData_bar(:,n);
%     fv = createSphere(T, 10);
%     h = patch(fv, 'FaceColor', [1,0,0]);
%     set(h, 'FaceLighting', 'flat');
%     set(h, 'EdgeColor', 'none');
%     %set(h, 'ButtonDownFcn', @btdown);
%     set(h, 'UserData', n);    
% end
% 
% 
%           
% names = {'RASIS', 'LASIS', 'RPSIS', 'LPSIS'};
% vectors = zeros(3,4);
% for n = 1:length(names)
%     disp(['Select the ' names{n} ' marker.']);
%     pause;
%     h = gco;
%     if strcmp(get(h, 'Type'), 'patch')
%         ind = get(h, 'UserData');
%         set(h, 'FaceColor', [0, 1, 0]);
%         vectors(:,n) = mData_bar(:,ind);
%     end
% end
% 
% T = eye(4); T(1:3,4) = mean(vectors,2);
% fv = createSphere(T, 10);
% h = patch(fv, 'FaceColor', [0,0,1]);
% set(h, 'FaceLighting', 'flat');
% set(h, 'EdgeColor', 'none');

stamarkers = {'RPP', 'LPP', 'RAS', 'LAS', 'RIC', 'LIC', 'RHP', 'LHP', ...
              'RTH1', 'RTH2', 'RTH3', 'RTH4', 'RLK', 'RMK', 'RSK1', 'RSK2', 'RSK3', 'RSK4', 'RLA', 'RMA', 'RFT1', 'RFT2', 'RFT3', ...
              'LTH1', 'LTH2', 'LTH3', 'LTH4', 'LLK', 'LMK', 'LSK1', 'LSK2', 'LSK3', 'LSK4', 'LLA', 'LMA', 'LFT1', 'LFT2', 'LFT3'};

dynmarkers_g = {'gRPP', 'gLPP', 'gRAS', 'gLAS', ...
              'gRTH1', 'gRTH2', 'gRTH3', 'gRTH4', 'gRSK1', 'gRSK2', 'gRSK3', 'gRSK4', 'gRFT1', 'gRFT2', 'gRFT3', ...
              'gLTH1', 'gLTH2', 'gLTH3', 'gLTH4', 'gLSK1', 'gLSK2', 'gLSK3', 'gLSK4', 'gLFT1', 'gLFT2', 'gLFT3'};

dynmarkers_l = {'lRPP', 'lLPP', 'lRAS', 'lLAS', ...
              'lRTH1', 'lRTH2', 'lRTH3', 'lRTH4', 'lRSK1', 'lRSK2', 'lRSK3', 'lRSK4', 'lRFT1', 'lRFT2', 'lRFT3', ...
              'lLTH1', 'lLTH2', 'lLTH3', 'lLTH4', 'lLSK1', 'lLSK2', 'lLSK3', 'lLSK4', 'lLFT1', 'lLFT2', 'lLFT3'};

          

fig = gcf;
if ( ~strcmp('xode', get(fig, 'UserData')) )
    close(fig);
    fig = figure('Position', [50, 50, 1024, 768], ...
             'Renderer', 'zbuffer', ...
             'UserData', 'buildModel');
end


% Scene axes
set(gca, 'DataAspectRatio',   [1, 1, 1], ...
         'CameraPosition',    [1000,1000,500], ...
         'CameraTarget',      [0,0,200], ...
         'CameraUpVector',    [0,0,1], ...
         'Visible',           'off');
light('Position',[0 0 2000],'Style','infinite');

for n = 1:38
    plotMarker(mData_bar(:,n), [1,0,0], n, stamarkers{n});
end

% -------------------------------------------------------------------------
%   Pelvis coordinate system
% -------------------------------------------------------------------------
RAS = mData_bar(:, find(strcmp('RAS', stamarkers)) );
LAS = mData_bar(:, find(strcmp('LAS', stamarkers)) );
RPP = mData_bar(:, find(strcmp('RPP', stamarkers)) );
LPP = mData_bar(:, find(strcmp('LPP', stamarkers)) );

PelvisCenter = (LAS + RAS) / 2;

V1 = LAS - RAS;
PelvisYAxis = V1 ./ norm(V1);

V1 = PelvisCenter - ((RPP + LPP)/2);
V1 = cross(V1, PelvisYAxis);
PelvisZAxis = V1 ./ norm(V1);

PelvisXAxis = cross(PelvisYAxis,PelvisZAxis);

T_Lab_Pelvis = [PelvisXAxis, PelvisYAxis, PelvisZAxis, PelvisCenter; 0, 0, 0, 1];
plotMarker(PelvisCenter, [0,1,0], 0, 'PELVIS');
drawCoordinateSystem(fig, T_Lab_Pelvis, '');

% HJC
PW = norm(RAS - LAS); % pelvic width;
LHipCenter = T_Lab_Pelvis * [-0.19*PW; 0.36*PW; -0.3*PW; 1]; % left
RHipCenter = T_Lab_Pelvis * [-0.19*PW; -0.36*PW; -0.3*PW; 1]; % right

LHipCenter = LHipCenter(1:3,1);
RHipCenter = RHipCenter(1:3,1);

% Convert tracking markers markers
T_Pelvis_Lab = invTMatrix(T_Lab_Pelvis);
v = T_Pelvis_Lab * [RAS; 1]; RAS_b = v(1:3,1);
v = T_Pelvis_Lab * [LAS; 1]; LAS_b = v(1:3,1);
v = T_Pelvis_Lab * [RPP; 1]; RPP_b = v(1:3,1);
v = T_Pelvis_Lab * [LPP; 1]; LPP_b = v(1:3,1);

GM = [RPP, LPP, RAS, LAS];
LM = [RPP_b, LPP_b, RAS_b, LAS_b];
%T = rotEstimate(GM, LM);
%drawCoordinateSystem(fig, T, 'Pelvis_{estimate}');

% -------------------------------------------------------------------------
%   Right thigh coordinate system
% -------------------------------------------------------------------------
RMK = mData_bar(:, find(strcmp('RMK', stamarkers)) );
RLK = mData_bar(:, find(strcmp('RLK', stamarkers)) );
[T_Lab_RThigh, RKneeCenter, RThighLength] = computeThigh(RMK, RLK, RHipCenter);
plotMarker(T_Lab_RThigh(1:3,4), [0,1,0], 0, 'RHIP');
drawCoordinateSystem(fig, T_Lab_RThigh, '');

% Convert tracking markers markers
T_RThigh_Lab = invTMatrix(T_Lab_RThigh);
RTH1 = mData_bar(:, find(strcmp('RTH1', stamarkers)) );
RTH2 = mData_bar(:, find(strcmp('RTH2', stamarkers)) );
RTH3 = mData_bar(:, find(strcmp('RTH3', stamarkers)) );
RTH4 = mData_bar(:, find(strcmp('RTH4', stamarkers)) );
v = T_RThigh_Lab * [RTH1; 1]; RTH1_b = v(1:3,1);
v = T_RThigh_Lab * [RTH2; 1]; RTH2_b = v(1:3,1);
v = T_RThigh_Lab * [RTH3; 1]; RTH3_b = v(1:3,1);
v = T_RThigh_Lab * [RTH4; 1]; RTH4_b = v(1:3,1);

GM = [RTH1, RTH2, RTH3, RTH4];
LM = [RTH1_b, RTH2_b, RTH3_b, RTH4_b];
%T = rotEstimate(GM, LM);
%drawCoordinateSystem(fig, T, 'RThigh_{estimate}');

% -------------------------------------------------------------------------
%   Right shank coordinate system
% -------------------------------------------------------------------------
RMA = mData_bar(:, find(strcmp('RMA', stamarkers)) );
RLA = mData_bar(:, find(strcmp('RLA', stamarkers)) );
[T_Lab_RShank, RAnkleCenter, RShankLength] = computeShank(RMA, RLA, RKneeCenter);
plotMarker(T_Lab_RShank(1:3,4), [0,1,0], 0, 'RKNEE');
drawCoordinateSystem(fig, T_Lab_RShank, '');

% Convert tracking markers markers
T_RShank_Lab = invTMatrix(T_Lab_RShank);
RSK1 = mData_bar(:, find(strcmp('RSK1', stamarkers)) );
RSK2 = mData_bar(:, find(strcmp('RSK2', stamarkers)) );
RSK3 = mData_bar(:, find(strcmp('RSK3', stamarkers)) );
RSK4 = mData_bar(:, find(strcmp('RSK4', stamarkers)) );
v = T_RShank_Lab * [RSK1; 1]; RSK1_b = v(1:3,1);
v = T_RShank_Lab * [RSK2; 1]; RSK2_b = v(1:3,1);
v = T_RShank_Lab * [RSK3; 1]; RSK3_b = v(1:3,1);
v = T_RShank_Lab * [RSK4; 1]; RSK4_b = v(1:3,1);

GM = [RSK1, RSK2, RSK3, RSK4];
LM = [RSK1_b, RSK2_b, RSK3_b, RSK4_b];
%T = rotEstimate(GM, LM);
%drawCoordinateSystem(fig, T, 'RShank_{estimate}');

% -------------------------------------------------------------------------
%   Right foot coordinate system
% -------------------------------------------------------------------------
RFT1 = mData_bar(:, find(strcmp('RFT1', stamarkers)) );
RFT2 = mData_bar(:, find(strcmp('RFT2', stamarkers)) );
RFT3 = mData_bar(:, find(strcmp('RFT3', stamarkers)) );
C = (RFT2 + RFT3) / 2;
[T_Lab_RFoot] = computeFoot(RMA, RLA, C, RAnkleCenter);
plotMarker(T_Lab_RFoot(1:3,4), [0,1,0], 0, 'RANKLE');
drawCoordinateSystem(fig, T_Lab_RFoot, '');

% Convert tracking markers markers
T_RFoot_Lab = invTMatrix(T_Lab_RFoot);
v = T_RFoot_Lab * [RFT1; 1]; RFT1_b = v(1:3,1);
v = T_RFoot_Lab * [RFT2; 1]; RFT2_b = v(1:3,1);
v = T_RFoot_Lab * [RFT3; 1]; RFT3_b = v(1:3,1);

GM = [RFT1, RFT2, RFT3];
LM = [RFT1_b, RFT2_b, RFT3_b];
%T = rotEstimate(GM, LM);
%drawCoordinateSystem(fig, T, 'RFoot_{estimate}');

% -------------------------------------------------------------------------
%   Left thigh coordinate system
% -------------------------------------------------------------------------
LMK = mData_bar(:, find(strcmp('LMK', stamarkers)) );
LLK = mData_bar(:, find(strcmp('LLK', stamarkers)) );
[T_Lab_LThigh, LKneeCenter, LThighLength] = computeThigh(LMK, LLK, LHipCenter);
plotMarker(T_Lab_LThigh(1:3,4), [0,1,0], 0, 'LHIP');
drawCoordinateSystem(fig, T_Lab_LThigh, '');

% Convert tracking markers markers
T_LThigh_Lab = invTMatrix(T_Lab_LThigh);
LTH1 = mData_bar(:, find(strcmp('LTH1', stamarkers)) );
LTH2 = mData_bar(:, find(strcmp('LTH2', stamarkers)) );
LTH3 = mData_bar(:, find(strcmp('LTH3', stamarkers)) );
LTH4 = mData_bar(:, find(strcmp('LTH4', stamarkers)) );
v = T_LThigh_Lab * [LTH1; 1]; LTH1_b = v(1:3,1);
v = T_LThigh_Lab * [LTH2; 1]; LTH2_b = v(1:3,1);
v = T_LThigh_Lab * [LTH3; 1]; LTH3_b = v(1:3,1);
v = T_LThigh_Lab * [LTH4; 1]; LTH4_b = v(1:3,1);

GM = [LTH1, LTH2, LTH3, LTH4];
LM = [LTH1_b, LTH2_b, LTH3_b, LTH4_b];
%T = rotEstimate(GM, LM);
%drawCoordinateSystem(fig, T, 'LThigh_{estimate}');

% -------------------------------------------------------------------------
%   Left shank coordinate system
% -------------------------------------------------------------------------
LMA = mData_bar(:, find(strcmp('LMA', stamarkers)) );
LLA = mData_bar(:, find(strcmp('LLA', stamarkers)) );
[T_Lab_LShank, LAnkleCenter, LShankLength] = computeShank(LMA, LLA, LKneeCenter);
plotMarker(T_Lab_LShank(1:3,4), [0,1,0], 0, 'LKNEE');
drawCoordinateSystem(fig, T_Lab_LShank, '');

% Convert tracking markers markers
T_LShank_Lab = invTMatrix(T_Lab_LShank);
LSK1 = mData_bar(:, find(strcmp('LSK1', stamarkers)) );
LSK2 = mData_bar(:, find(strcmp('LSK2', stamarkers)) );
LSK3 = mData_bar(:, find(strcmp('LSK3', stamarkers)) );
LSK4 = mData_bar(:, find(strcmp('LSK4', stamarkers)) );
v = T_LShank_Lab * [LSK1; 1]; LSK1_b = v(1:3,1);
v = T_LShank_Lab * [LSK2; 1]; LSK2_b = v(1:3,1);
v = T_LShank_Lab * [LSK3; 1]; LSK3_b = v(1:3,1);
v = T_LShank_Lab * [LSK4; 1]; LSK4_b = v(1:3,1);

GM = [LSK1, LSK2, LSK3, LSK4];
LM = [LSK1_b, LSK2_b, LSK3_b, LSK4_b];
%T = rotEstimate(GM, LM);
%drawCoordinateSystem(fig, T, 'LShank_{estimate}');

% -------------------------------------------------------------------------
%   Left foot coordinate system
% -------------------------------------------------------------------------
LFT1 = mData_bar(:, find(strcmp('LFT1', stamarkers)) );
LFT2 = mData_bar(:, find(strcmp('LFT2', stamarkers)) );
LFT3 = mData_bar(:, find(strcmp('LFT3', stamarkers)) );
C = (LFT2 + LFT3) / 2;
[T_Lab_LFoot] = computeFoot(LMA, LLA, C, LAnkleCenter);
plotMarker(T_Lab_LFoot(1:3,4), [0,1,0], 0, 'LANKLE');
drawCoordinateSystem(fig, T_Lab_LFoot, '');

% Convert tracking markers markers
T_LFoot_Lab = invTMatrix(T_Lab_LFoot);
v = T_LFoot_Lab * [LFT1; 1]; LFT1_b = v(1:3,1);
v = T_LFoot_Lab * [LFT2; 1]; LFT2_b = v(1:3,1);
v = T_LFoot_Lab * [LFT3; 1]; LFT3_b = v(1:3,1);

GM = [LFT1, LFT2, LFT3];
LM = [LFT1_b, LFT2_b, LFT3_b];
%T = rotEstimate(GM, LM);
%drawCoordinateSystem(fig, T, 'LFoot_{estimate}');

transMarkers = [RPP_b, LPP_b, RAS_b, LAS_b, ...
                RTH1_b, RTH2_b, RTH3_b, RTH4_b, RSK1_b, RSK2_b, RSK3_b, RSK4_b, RFT1_b, RFT2_b, RFT3_b, ...
                LTH1_b, LTH2_b, LTH3_b, LTH4_b, LSK1_b, LSK2_b, LSK3_b, LSK4_b, LFT1_b, LFT2_b, LFT3_b]';
orignMarkers = [RPP, LPP, RAS, LAS, ...
                RTH1, RTH2, RTH3, RTH4, RSK1, RSK2, RSK3, RSK4, RFT1, RFT2, RFT3, ...
                LTH1, LTH2, LTH3, LTH4, LSK1, LSK2, LSK3, LSK4, LFT1, LFT2, LFT3]';
[r,c] = size(transMarkers);            
transMarkers_cell = mat2cell(transMarkers, ones(r,1), 3);
orignMarkers_cell = mat2cell(orignMarkers, ones(r,1), 3);
localMarkers = [dynmarkers_l', transMarkers_cell];
globalMarkers = [dynmarkers_g', orignMarkers_cell];

function [T_Lab_Thigh, KneeCenter, Length] = computeThigh(MK, LK, HipCenter)
KneeCenter = (MK + LK)/2;

ThighZVector = KneeCenter - HipCenter;
Length = norm(ThighZVector);
ThighZAxis = ThighZVector ./ Length;

ThighXVector = cross(LK - HipCenter, MK - HipCenter);
ThighXAxis = ThighXVector ./ norm(ThighXVector); 

ThighYVector = cross(ThighZAxis,ThighXAxis);
ThighYAxis = ThighYVector ./ norm(ThighYVector);

T_Lab_Thigh = [ThighXAxis, ThighYAxis, ThighZAxis, HipCenter; 0, 0, 0, 1];


function [T_Lab_Shank, AnkleCenter, Length] = computeShank(MA, LA, KneeCenter)
AnkleCenter = (MA + LA)/2;

ShankZVector = AnkleCenter - KneeCenter;
Length = norm(ShankZVector);
ShankZAxis = ShankZVector ./ Length;

ShankXVector = cross(LA - KneeCenter, MA - KneeCenter);
ShankXAxis = ShankXVector ./ norm(ShankXVector); 

ShankYVector = cross(ShankZAxis,ShankXAxis);
ShankYAxis = ShankYVector ./ norm(ShankYVector);

T_Lab_Shank = [ShankXAxis, ShankYAxis, ShankZAxis, KneeCenter; 0, 0, 0, 1];


function [T_Lab_Foot] = computeFoot(MA, LA, C, AnkleCenter)
FootZVector = LA - MA;
FootZAxis = FootZVector ./ norm(FootZVector); 

FootXVector = cross(C-AnkleCenter,FootZAxis);
FootXAxis = FootXVector ./ norm(FootXVector);

FootYVector = cross(FootZAxis, FootXAxis);
FootYAxis = FootYVector ./ norm(FootYVector);

T_Lab_Foot = [FootXAxis, FootYAxis, FootZAxis, AnkleCenter; 0, 0, 0, 1];


function plotMarker(pos, color, id, name)
T = eye(4); T(1:3,4) = pos;
fv = createSphere(T, 10);
h = patch(fv, 'FaceColor', color);
set(h, 'FaceLighting', 'flat');
set(h, 'EdgeColor', 'none');
set(h, 'UserData', id);
text(T(1,4)+5, T(2,4)+5, T(3,4)+5, name);


function Tinv = invTMatrix(T)
Tinv = eye(4);
Tinv(1:3,1:3) = T(1:3,1:3)';
Tinv(1:3,4) = -Tinv(1:3,1:3) * T(1:3,4);