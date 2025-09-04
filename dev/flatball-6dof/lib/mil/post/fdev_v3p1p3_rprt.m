# Copyright 2025 Michael V. Schaefer
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

%% initialization
flatball_init_disc_params;

fldr_exp = '.\wrk\f-dev-3.1.3-datalogs\2024\07-oct';

%% thrower - mvs - right handed

%% back-hand
n_exp = 'x0000000b';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
close all;

%% back-hand
n_exp = 'x0000000c';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
close all;

%% fore-hand
n_exp = 'x0000000d';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
% close all;

%% fore-hand
n_exp = 'x0000000e';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
close all;

%% hammer
n_exp = 'x0000000f';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
close all;

%% hammer
n_exp = 'x00000010';
fname = [ fldr_exp '\' n_exp '.csv'];
tsc = flatball_log_proc( fname);
flatball_log_plots;
close all;
