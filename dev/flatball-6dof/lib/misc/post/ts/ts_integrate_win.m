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


function y = ts_integrate_win( x, idx1, idx2)

try
    y = x.copy;
catch
    y = x;
end

% y.Data(isnan(y.Data)) = 0;
y = y .* 0;

tmp = cumtrapz( x.Time(idx1:idx2), x.Data(idx1:idx2));

y.Data(idx1:idx2) = tmp;
y.Data(idx2:end) = tmp(end);
