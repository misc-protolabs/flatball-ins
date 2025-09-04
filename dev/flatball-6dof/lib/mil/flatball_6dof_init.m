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


%% define ambient parameters

amb.g = 9.8067; % m/s^2 - https://en.wikipedia.org/wiki/Gravitational_acceleration
amb.t = 15; % degC
amb.rho = 1.225; % Kg/m^3 - https://en.wikipedia.org/wiki/Density_of_air

%% define non-aerodynamic parameters
% non-aerodynamic parameters
% https://usaultimate.org/wp-content/uploads/2020/11/Disc-Technical-Drawing.pdf
% m = 175 +/- 3g
% d = 274 +/- 3mm
% h = 32 +/- 2mm
disc = struct;
disc.m = 0.175; % Kg
disc.d = 0.275; % m
disc.c = 0.275; % Reference chord length (m)
disc.Id = 0.001219; % moment of inertia about the planar axis
disc.Ia = 0.002352; % moment of inertia about the spinning axis
disc.I = [disc.Id 0 0; 0 disc.Id 0; 0 0 disc.Ia];
disc.S = pi*(disc.c/2)^2;   % Reference area (m^2)
disc.A = disc.S;
disc.acc_r_x = 19.575e-3;    % accelerometer x-offset
disc.acc_r_y = 6.75e-3;     % accelerometer y-offset

% define disc aerodynamic properties
% https://research.manchester.ac.uk/en/publications/simulation-of-a-spin-stabilised-sports-disc
disc.alpha0 = -0.052;
disc.CD0 = 0.085;
disc.CDa = 3.30;
disc.CL0 = 0.13;
disc.CLa = 3.09;
disc.CLp = -1.30e-2;
disc.CM0 = -0.01;
disc.CMa = 0.057;
disc.CMq = -1.40e-2;
disc.CNr = -3e-3; % NOTE: not part of literature
disc.CRp = -1.25e-2; % per Hummel source code

%% aero coefficient table lookups
disc.aoa_data = xlsread( which( 'crowther-wind-tunnel-data.xls'), 'DiscAerodynamicData', 'L4:L44');
disc.aoa_data = disc.aoa_data .* C.DEG2RAD;
disc.Clift_data = xlsread( which( 'crowther-wind-tunnel-data.xls'), 'DiscAerodynamicData', 'M4:M44');
disc.Cdrag_data = xlsread( which( 'crowther-wind-tunnel-data.xls'), 'DiscAerodynamicData', 'N4:N44');
disc.CM_data = xlsread( which( 'crowther-wind-tunnel-data.xls'), 'DiscAerodynamicData', 'O4:O44');


%% coefficients from crowther and potts - 2007 phd thesis?
% disc.CL0 = 0.13;
% disc.CLa = 3.09;
% disc.CD0 = 0.085;
% disc.CDa = 3.30;
% disc.CM0 = -0.01;
% disc.CMa = 0.057;

%% coefficients from "best-results" and initial sensitivity study...
% disc.CNr = -3e-5;
% disc.CMq= -1e-3;
% disc.CRr = 5e-4;
% disc.CRp = -1e-2;

%% coefficients from hummel thesis 2003 - table 3.2
% disc.CL0 = 0.33;
% disc.CLa = 1.91;
% disc.CD0 = 0.18;
% disc.CDa = 0.69;
% disc.CM0 = -0.08;
% disc.CMa = 0.43;

%% miscellaneous options from h&h and p&c...
%
% disc.CNr = -3.4e-5; % long flights
% disc.CMq= -1.40e-2; % long flights
% disc.CRr = 3e-3 * 0.1659; % NOTE: missing from 2014, borrowed from P&C, and hacked...
% disc.CRp =-1.3e-2; % long flights
% disc.CRr = 1.4e-2; % short flights
% disc.CNr = -7.1e-6; % short flights
% disc.CRp =-5.5e-3; % short flights
% disc.CMq= -5e-3; % short flights
