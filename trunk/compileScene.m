clear mex

% Path to code
dricspath = fullfile('/','home','yamokosk','code','drics','');
incpath = fullfile(dricspath,'src','libs','');
libpath = fullfile(dricspath,'src','libs','xode','');
srcpath = fullfile(libpath,'mex','');

% Mex flags
%mexflags = [mexflags sprintf('-l%s', libname)];

% Compile mex library
str = sprintf('mex -I%s -L%s -lxode -lodegim %s %s', ...
                incpath, ...
                libpath, ...
                fullfile(srcpath, 'scene.cpp'), ...
                fullfile(srcpath, 'common.cpp'));
fprintf('%s\n', str);
eval(str);