clear mex

mbglfiles = {'sceneml.cpp', 'common.cpp'};

c = computer;

large_arrays = 0;
solaris = 0;
mac = 0;
libname = '';

switch (computer)
    case 'PCWIN'
        libname = 'sceneml';
    case 'GLNX86'
        libname = 'sceneml';
    case 'MACI'
        libname = 'sceneml';
    otherwise
        error('Not currently supported...\n');
end

mexflags = '';

if (large_arrays)
    mexflags = [mexflags ' -largeArrayDims -DMATLAB_BGL_LARGE_ARRAYS '];
end
	     
if (ispc)
	% must change /MD to /ML in mexopts.bat
	mexflags = [mexflags sprintf('-O -I\\include -L\\lib -L${ODE_HOME}\lib\release -l%s  -lode', libname)];
elseif (mac)
    % mac specific options
elseif (isunix)
    mexflags = [mexflags '-O -I/include -L/lib -L${ODE_HOME}\lib\release'];
    mexflags = [mexflags sprintf('-l%s -lode', libname)];
end

for file = mbglfiles
    mexflags = [mexflags ' %s' char(file)];
end

mexstr = ['mex ' mexflags];
fprintf('%s\n', mexstr);
eval(mexstr);