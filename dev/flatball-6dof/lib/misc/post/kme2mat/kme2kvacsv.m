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


function kme2kvacsv( kme40_fname)

if ~exist( kme40_fname, 'file')
    kme40_fname = [kme40_fname '.kme40'];
    if ~exist( kme40_fname, 'file')
        disp( ['File: ' kme40_fname ' not found']);
        return;
    end
end

%%
csv_fname = strrep( which(kme40_fname), '.kme40', '.kvacsv');
exe = which( 'kvaKmeConv.exe');
cmd = [ exe ' -inf kme40 -kme ' which(kme40_fname) ' -outf csv -o ' csv_fname ' -c 3'];

system( cmd);
