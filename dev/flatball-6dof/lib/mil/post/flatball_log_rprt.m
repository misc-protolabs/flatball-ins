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

%% flatball log report

% take care of paths and initalizations
addpath( genpath( 'arxiv/data'));
flatball_6dof_init;

% load datalog and generate plots
csv_fname = '2025-08-31-002-rb';
tsc = flatball_log_proc( which( [csv_fname '.csv']));
flatball_log_plots( tsc)

%% available datalogs
% fname = '2025-08-16-001-rb';
% fname = '2025-08-16-002-rb';
% fname = '2025-08-16-003-rf';
% fname = '2025-08-16-004-rf';
% fname = '2025-08-16-005-ambient';
% fname = '2025-08-17-001-rb';
% fname = '2025-08-17-002-rb';
% fname = '2025-08-17-003-rf';
% fname = '2025-08-17-004-rf';
% fname = '2025-08-31-001-rb';
% fname = '2025-08-31-002-rb';
% fname = '2025-08-31-003-rf';
% fname = '2025-08-31-004-rf';
% fname = '2025-08-31-005-rh';
% fname = '2025-08-31-006-rh';

%% work-in-progress

% fitYawDampingMinimal( tsc.Time(idx_launch:idx_impact), tsc.gyro_z.Data(idx_launch:idx_impact), disc.I(3,3), 0.999, 1e3);
% fitYawDamping( tsc.Time(idx_launch_end:idx_impact), tsc.gyro_z.Data(idx_launch_end:idx_impact), disc.I(3,3));
