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
% global physical constant definitions
%
misc_dbg( '.. creating physical constants container C');

create_phys_const( 'PI', pi);
% create_phys_const( '2PI', 2*pi);
create_phys_const( 'RPM2RADSEC', (2*pi)/60);
create_phys_const( 'RADSEC2RPM', 60/(2*pi));
create_phys_const( 'DEG2RAD', (2*pi)/360);
create_phys_const( 'RAD2DEG', 360/(2*pi));
create_phys_const( 'MPS2KPH', 3600/1000);
create_phys_const( 'KPH2MPS', 1000/3600);
create_phys_const( 'PA2PSI', 1/6.89476e3);
create_phys_const( 'PSI2PA', 6.89476e3);
create_phys_const( 'FALSE', 0);
create_phys_const( 'TRUE', 1);
create_phys_const( 'ZERO', 0);
create_phys_const( 'PCT2NORM', 0.01);
create_phys_const( 'NORM2PCT', 1/get_phys_const('PCT2NORM'));
create_phys_const( 'CM32M3', 1e-6);
create_phys_const( 'M32CM3', 1/get_phys_const('CM32M3'));
create_phys_const( 'KM2M', 1e3);
create_phys_const( 'M2KM', 1/get_phys_const('KM2M'));
create_phys_const( 'GAL2LTR', 3.785);
create_phys_const( 'LTR2GAL', 1/get_phys_const('GAL2LTR'));
create_phys_const( 'MI2M', 1609.344);
create_phys_const( 'M2MI', 1/get_phys_const('MI2M'));
create_phys_const( 'CH2O', 4.18e6);
create_phys_const( 'IN2M', 0.0254);
create_phys_const( 'M2IN', 1/0.0254);
create_phys_const( 'BAR2PA', 1e5);
create_phys_const( 'PA2BAR', 1e-5);
create_phys_const('R',8314);
create_phys_const('AIR_MM', 28.97); % kg/kmol
create_phys_const('H2O_SPEC_HT', 4186); % J/(kg*K)
create_phys_const('H2O_RHO', 1000); % kg/(m^3)
create_phys_const('FT32M3', 0.028317);
create_phys_const('M32FT3', 1/0.028317);
create_phys_const('KCFS2M3PS', 1000*0.028317);
create_phys_const('SV2M3PS', 1e6);
create_phys_const('M3PS2SV', 1e-6);
create_phys_const('H2S', 60*60);
create_phys_const('D2S', 24*60*60);
create_phys_const('Y2S', 365*24*60*60);
