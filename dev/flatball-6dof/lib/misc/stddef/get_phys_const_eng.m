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


function val = get_phys_const_eng( nam)
%
% get physical constant - engineering value
%

sw_val = evalin( 'base', ['C_' nam]);
sw_k = evalin( 'base', ['C_' nam(1:end-4) '_K']);
val = sw_val / sw_k;
% val = get_phys_const( nam) / get_phys_const_k( nam);
