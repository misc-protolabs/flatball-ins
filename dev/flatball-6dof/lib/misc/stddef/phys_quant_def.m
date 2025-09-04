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
%
% global physical quantity definitions
%
misc_dbg( '.. creating physical quantity container Q');

create_phys_quantity( 'ACCEL', -1000, 1000, 1/2048, 1);
create_phys_quantity( 'ANG', -3600, 3600, 1e-7, get_phys_const('DEG2RAD'));
create_phys_quantity( 'CAP', 0, 1e5, 1e-3, 1);
create_phys_quantity( 'CHRG', 0, 1e8, 1e-3, 1);
create_phys_quantity( 'CHRGAH', 0, 1e9, 1e-3, 1);
create_phys_quantity( 'CURR', -1000, 1000, 1e-3, 1);
create_phys_quantity( 'DC', 0, 100, 1e-3, get_phys_const('PCT2NORM'));
create_phys_quantity( 'DIST', -527e6, 527e6, 1e-4, get_phys_const('KM2M'));
create_phys_quantity( 'DT', 0, 1000, 1e-3, 1);
create_phys_quantity( 'EFF', -1e9, 1e9, 1e-3, 1);
create_phys_quantity( 'ENER', 0, 1e9, 1e-3, 1);
create_phys_quantity( 'F', -25000000, 25000000, 5, 1);
create_phys_quantity( 'FLG', 0, 1, 1, 1);
create_phys_quantity( 'FLOW', 0, 10000, 0.014, 1);
create_phys_quantity( 'FREQ', 0, 1e6, 1/128, 1);
create_phys_quantity( 'GAL', 0, 2105540607.5*get_phys_const('LTR2GAL'), 1/48000, 1); % resolution based on ford PCM gallon res on CAN
create_phys_quantity( 'IND', 0, 1, 1e-3, 1);
create_phys_quantity( 'INRT', 0, 250000000, 4e-3, 1);
create_phys_quantity( 'LEN', 0, 100, 1e-3, 1);
create_phys_quantity( 'LTR', 0, 2105540607.5, get_phys_const('GAL2LTR'), 1); % max based on HD OBD req
create_phys_quantity( 'LINSPD', -500, 500, 1/256, get_phys_const('KPH2MPS'));
create_phys_quantity( 'M', 0, 25000, 0.1, 1);
create_phys_quantity( 'MI', -527e6*get_phys_const('KM2M')*get_phys_const('M2MI'), 527e6*get_phys_const('KM2M')*get_phys_const('M2MI'), 1/8000, 1);  % resolution based on ford PCM miles res on CAN
create_phys_quantity( 'PCT', 0, 100, 0.0025, get_phys_const('PCT2NORM'));
create_phys_quantity( 'PRESS', 0, 10000, 1e-3, 1);
create_phys_quantity( 'PWR', -1e6, 1e6, 1, 1);
create_phys_quantity( 'RATIO', -1e9, 1e9, 1e-6, 1);
create_phys_quantity( 'RES', 0, 1e6, 1e-3, 1);
create_phys_quantity( 'ROTSPD', -1e6, 1e6, 0.125, get_phys_const('RPM2RADSEC'));
create_phys_quantity( 'S8', -128, 127, 1, 1);
create_phys_quantity( 'S16', -32768, 32767, 1, 1);
create_phys_quantity( 'S32', -2147483648, 2147483647, 1, 1);
create_phys_quantity( 'SOC', 0, 100, 1e-3, get_phys_const('PCT2NORM'));
create_phys_quantity( 'T', 0, 43000000000, 1e-5, 1);
create_phys_quantity( 'TEMP', -100, 1000, 0.03125, 1);
create_phys_quantity( 'TRQ', -1e5, 1e5, 1, 1);
create_phys_quantity( 'VOL', 0, 2.2e12, 0.5, get_phys_const('CM32M3'));
create_phys_quantity( 'U8', 0, 255, 1, 1);
create_phys_quantity( 'U16', 0, 65535, 1, 1);
create_phys_quantity( 'U32', 0, 4294967295, 1, 1);
create_phys_quantity( 'VOLT', 0, 1000, 1/128, 1);
