function M = sceneAnimate(varNames, varargin)

[numVars,junk] = size(varNames);

if nargin > 1
    A = varargin{1};
    [numFrames, junk] = size(A);
    
    if (numVars ~= nargin - 1)
        error(['Number of variables (' num2str(numVars) ') does not match the number of data inputs (' num2str(nargin-1) ').']);
    end
    
    for j = 1:numFrames
        vars = {};
        for n = 1:numVars
            A = varargin{n};
            [r,c] = size(A);
            
            vars(n,1) = mat2cell(A(j,:), 1, c);
        end
        sceneSetVars([varNames, vars]);
        sceneRender();
        M(j) = getframe;
    end
else
    M = [];
end
