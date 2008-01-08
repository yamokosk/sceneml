function [out, names] = getMCFrame(n,mData)

gmarkers = squeeze(mData(n, :, :));
[r,c] = size(gmarkers);
gmarkers_cell = mat2cell(gmarkers, ones(r,1), 3);
% 
dynmarkers_g = {'gRPP', 'gLPP', 'gRAS', 'gLAS', ...
               'gRTH1', 'gRTH2', 'gRTH3', 'gRTH4', 'gRSK1', 'gRSK2', 'gRSK3', 'gRSK4', 'gRFT1', 'gRFT2', 'gRFT3', ...
               'gLTH1', 'gLTH2', 'gLTH3', 'gLTH4', 'gLSK1', 'gLSK2', 'gLSK3', 'gLSK4', 'gLFT1', 'gLFT2', 'gLFT3'};
% 
rout = length(dynmarkers_g);
% 
out = [dynmarkers_g', gmarkers_cell(1:rout,:)];
%[r,c] = size(dynmarkers_g);
%for n = 1:c
%    out.(dynmarkers_g{n}) = squeeze(mData(:,n,:));    
%end

names = dynmarkers_g';