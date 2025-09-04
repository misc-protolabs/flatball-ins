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


function q = create_phys_quantity( quant, min, max, res, sw_cnv_k)
%
% create physical quantity information
%

q = {};
q.name = quant;
q.eng_min = min;
q.eng_max = max;
q.eng_res = res;
q.sw_cnv_k = sw_cnv_k;

try
    Q = evalin( 'base', 'Q');
catch
    Q = struct;
end

Q.(quant) = q;
assignin( 'base', 'Q', Q);
