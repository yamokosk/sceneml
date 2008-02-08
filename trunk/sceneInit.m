function sceneInit(filename)
% sceneLoad - Instructs the SceneML library to load a scene from file
%
%   sceneLoad(filename) simply instructs the SceneML library to load the
%   scene XML definition stored in filename. 
%
%   This function only has to be called anytime the XML description
%   changes. Otherwise this function only needs to be called once.
%
%   Example:
%       sceneLoad('demo/twopa10.scene');

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
        if ( strcmp('xode', figUserData) )
            warning('Found a SceneML render window. Closing the old one but will re-render once the new scene is loaded.');
            oldFigNumber = fig;
            close(fig);
        end
    end
end

% Now load the new scene description
sceneml('Init', filename);

% If the user had a render window open before, lets be nice and re-render
% this one.
if (oldFigNumber > 0)
    figure(oldFigNumber);
    sceneRender;
end