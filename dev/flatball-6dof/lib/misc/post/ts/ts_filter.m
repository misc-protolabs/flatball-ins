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


function y = ts_filter( x, tau)
%
% x - matlab timeseries
% dt - timestep
% tau - time constant of 1st order filter
%

y = x;

dt = mean( diff( y.Time));

two_dt_thr = 2*dt;

if tau < two_dt_thr
    tau = two_dt_thr;
    warning( sprintf( '(%5.3f < %5.3f) Inappropriate selection for time constant, setting to 2x the timestep...', tau, two_dt_thr));
end
a0 = dt / tau;
a1 = 1 - a0;

sz = size(y.Data);
for i=1:sz(2)
    y.Data(:,i) = filter( 1, [1/a0 -a1/a0], x.Data(:,i), mean( x.Data(1:100,i))*a1);
end
