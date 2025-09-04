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


function y = ts_eff( ts1, ts2, lo, hi, dt, z, wn)


n = ts_filt( ts1, dt, z, wn, 0) ./ ts_filt( ts2, dt, z, wn, 0);

n = min( n, ones( length( n),1) * hi);
n = max( n, ones( length( n),1) * lo);

y = n;
