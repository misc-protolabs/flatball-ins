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
