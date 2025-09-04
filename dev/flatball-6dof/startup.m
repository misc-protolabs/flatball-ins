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

%%
clear;
tic0 = tic;

%% python setup - NOTE: this is done in local scripts where necessary
try
    pe = pyenv( "Version", "C:\Program Files\Python39\pythonw.exe");
catch
end

addpath( genpath( './lib'));
addpath( genpath( './.wrk'));
toc( tic0);

misc_tool_ver_check;
toc( tic0);
misc_stddef;
toc( tic0);

%% publish related settings
pub_opts = struct( ...
    'format','pdf', ...
    'outputDir','./wrk/reports', ...
    'showCode',true, ...
    'useNewFigure', true);

%%
beep off;
