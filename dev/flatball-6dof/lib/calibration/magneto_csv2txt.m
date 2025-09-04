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
fname = which( 'mag-cal-0xca.csv');
tsc = flatball_log_proc( fname);

%%
fname = 'mag-cal-0xca.txt';
fid = fopen( fname, 'w');
for idx=1:length(tsc.Time)
    ln = sprintf( '%f\t%f\t%f\n', tsc.mag_x.Data(idx), tsc.mag_y.Data(idx), tsc.mag_z.Data(idx));
    fprintf( fid, '%s', ln);
end
fclose( fid);
