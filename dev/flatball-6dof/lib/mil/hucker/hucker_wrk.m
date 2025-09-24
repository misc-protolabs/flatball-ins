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


%% load model
tic0 = tic;

flatball_6dof_init;
flatball_6dof_hucker_init;

sys = 'flatball_6dof';
open_system( sys);

toc( tic0);

%% desired launch conditions - forehand
%
% 1m above ground, ~18m/s NNE (at an easterly angle), 4deg roll, -12.5deg
% pitch, forehand (-) release ~65 rad/sec (~600rpm)
%
% NOTE: X = [x y z, u v w, phi theta psi, p q r]

XL_desired = [0 0 -1, 18.0 3.5 0, 4*pi/180 -12.5*pi/180 0, 0 0 -65];

% X0 z == 1m above ground, at rest
X0_z1m_at_rest = [1e-3 1e-3 -1, -1e-6 1e-6 -1e-6, 0 0 1e-6, 0 0 -1e-6];

%% run simulation
simout = sim( sys);
tsc = flatball_6dof_post( logsout, disc, amb);
toc( tic0);

%% flight metrics
flatball_6dof_flight_metrics;

%% launch plots
flatball_6dof_launch_plots;

%% flight plots
flatball_6dof_plots;

%% desired launch conditions - backhand
%
% 1m above ground, ~18m/s NNE (at an easterly angle), -4deg roll, -12.5deg
% pitch, backhand (+) release ~65 rad/sec (~600rpm)
%
% NOTE: X = [x y z, u v w, phi theta psi, p q r]

XL_desired = [0 0 -1, 18.0 -3.5 0, -0*pi/180 -0*pi/180 0, 0 0 65];

%% run simulation
simout = sim( sys);
tsc = flatball_6dof_post( logsout, disc, amb);
toc( tic0);

%% flight metrics
flatball_6dof_flight_metrics;

%% launch plots
flatball_6dof_launch_plots;

%% flight plots
flatball_6dof_plots;
toc( tic0);
