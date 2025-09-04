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

function tsc = flatball_6dof_post( logsout, disc, amb)
%%
logs = logsout;

tsc = tscollection;
% ts = logs.getElement( 'X').Values;
% tsc = tsc.addts( ts);
ts = logs.getElement( 'F2').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'M2').Values;
tsc = tsc.addts( ts);

ts = logs.getElement( 'alpha').Values;
tsc = tsc.addts( ts);

ts = logs.getElement( 'lift').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'drag').Values;
tsc = tsc.addts( ts);

% ts = logs.getElement( 'AdvR').Values;
% tsc = tsc.addts( ts);

ts = logs.getElement( 'x').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'y').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'z').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'u').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'v').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'w').Values;
tsc = tsc.addts( ts);

ts = logs.getElement( 'phi').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'theta').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'psi').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'p').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'q').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'r').Values;
tsc = tsc.addts( ts);

ts = logs.getElement( 'Cdrag').Values;
tsc = tsc.addts( ts);
ts = logs.getElement( 'Clift').Values;
tsc = tsc.addts( ts);

ts = logs.getElement( 'v_inf').Values;
tsc = tsc.addts( ts);
% ts = logs.getElement( 'p').Values;
% tsc = tsc.addts( ts);
% ts = logs.getElement( 'q').Values;
% tsc = tsc.addts( ts);
% ts = logs.getElement( 'r').Values;
% tsc = tsc.addts( ts);

if ~isempty(logsout.getElement( 'Fx_launch'))
    % launch forces and moments
    ts = logs.getElement( 'Fx_launch').Values;
    tsc = tsc.addts( ts);
    ts = logs.getElement( 'Fy_launch').Values;
    tsc = tsc.addts( ts);
    ts = logs.getElement( 'Fz_launch').Values;
    tsc = tsc.addts( ts);
    
    ts = logs.getElement( 'Mx_launch').Values;
    tsc = tsc.addts( ts);
    ts = logs.getElement( 'My_launch').Values;
    tsc = tsc.addts( ts);
    ts = logs.getElement( 'Mz_launch').Values;
    tsc = tsc.addts( ts);
%     
%     % launch forces and moments
%     ts = logs.getElement( 'Fx').Values;
%     tsc = tsc.addts( ts);
%     ts = logs.getElement( 'Fy').Values;
%     tsc = tsc.addts( ts);
%     ts = logs.getElement( 'Fz').Values;
%     tsc = tsc.addts( ts);
%     
%     ts = logs.getElement( 'Mx').Values;
%     tsc = tsc.addts( ts);
%     ts = logs.getElement( 'My').Values;
%     tsc = tsc.addts( ts);
%     ts = logs.getElement( 'Mz').Values;
%     tsc = tsc.addts( ts);
    
    % control error terms
%     ts = logs.getElement( 'Fx_launch_err').Values;
%     tsc = tsc.addts( ts);
%     ts = logs.getElement( 'Fy_launch_err').Values;
%     tsc = tsc.addts( ts);
%     ts = logs.getElement( 'Fz_launch_err').Values;
%     tsc = tsc.addts( ts);
%     
%     ts = logs.getElement( 'Mx_launch_err').Values;
%     tsc = tsc.addts( ts);
%     ts = logs.getElement( 'My_launch_err').Values;
%     tsc = tsc.addts( ts);
%     ts = logs.getElement( 'Mz_launch_err').Values;
%     tsc = tsc.addts( ts);
end

%%
advr = tsc.r .* (disc.d/2) ./ tsc.v_inf;
tsc = tsc.addts( advr, 'AdvR');

flight_length = ts_integrate( tsc.v_inf);
tsc = tsc.addts( flight_length.Data, 'flight_length');

revs = ts_integrate( tsc.r);
tsc = tsc.addts( revs.Data, 'revs');

if ~isempty(logsout.getElement( 'Fx_launch'))
    p_trans = tsc.Fx_launch .* tsc.u + tsc.Fy_launch .* tsc.v + tsc.Fz_launch .* tsc.w;
    tsc = tsc.addts( p_trans.Data, 'pwr_trans');
    
    p_rot = tsc.Mx_launch .* tsc.p + tsc.My_launch .* tsc.q + tsc.Mz_launch .* tsc.r;
    tsc = tsc.addts( p_rot.Data, 'pwr_rot');
    
    e_launch = ts_integrate( tsc.pwr_rot + tsc.pwr_trans);
    tsc = tsc.addts( e_launch.Data, 'e_launch');
end

ke_trans = tsc.v_inf .* tsc.v_inf .* (disc.m/2);
tsc = tsc.addts( ke_trans.Data, 'ke_trans');

ke_rot = tsc.r .* tsc.r .* (disc.Ia/2);
ke_rot = ke_rot + tsc.p .* tsc.p .* (disc.Id/2);
ke_rot = ke_rot + tsc.q .* tsc.q .* (disc.Id/2);
tsc = tsc.addts( ke_rot.Data, 'ke_rot');

ke_tot = tsc.ke_trans + tsc.ke_rot;
tsc = tsc.addts( ke_tot.Data, 'ke_tot');

%% 
tsc = tsc.addts( tsc.z.Data .* -1, 'z_up');
