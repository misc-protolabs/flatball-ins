% Copyright 2025 Michael V. Schaefer
% 
% Licensed under the Apache License, Version 2.0 (the "License");
% you may not use this file except in compliance with the License.
% You may obtain a copy of the License at:
% 
%     http://www.apache.org/licenses/LICENSE-2.0
% 
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS,
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing permissions and
% limitations under the License.


function h = new_formatted_fig( varargin)

h = figure( ...
    'WindowStyle', 'normal', ...
    'DockControls', 'on', ...
    'WindowStyle', 'docked', ...
    'PaperOrientation', 'Landscape', ...
    'PaperPositionMode', 'manual', ...
    'PaperPosition', [0.25 0.25 10.50 8.00]);

% legend;

if nargin > 0
    name = varargin{1};
    set(h,'Name', name);
end

end % function

