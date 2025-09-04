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
addpath ./arxiv/data/SN-2024110603/calibration/

SN_str = "SN-2024110603";

% fname = 'x00000019.csv';
fname = sprintf( "f-dev-cal-%s.csv", SN_str);
fname = which( fname);
% fname = 'x00000003.csv';
tsc = flatball_log_proc( fname);

%%
fdev_acc_cal;

%%
fdev_gyro_cal;

%%
fdev_mag_cal;

%%
title_str = sprintf( "%s - calibration", SN_str);
fdev_cal_plots;
