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


function add_vert_marker( t_marker)

yl = ylim;
hold all;
plot( [1 1] .* t_marker, [-1 1].*1e4, '-.k');
% plot( [1 1].*tsc.Time(idx_launch), [-1 1].*1e4, '-.');
% plot( [1 1].*tsc.Time(idx_impact), [-1 1].*1e4, '-.');
ylim( yl);
hold off;
