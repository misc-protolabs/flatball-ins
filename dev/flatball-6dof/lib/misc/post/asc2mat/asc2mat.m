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

function asc2mat( asc_fname, dbc_fname)

if ~exist( asc_fname, 'file')
    asc_fname = [asc_fname '.asc'];
    if ~exist( asc_fname, 'file')
        disp( ['File: ' asc_fname ' not found']);
        return;
    end
end

if ~exist( dbc_fname, 'file')
    dbc_fname = [dbc_fname '.dbc'];
    if ~exist( dbc_fname, 'file')
        disp( ['File: ' dbc_fname ' not found']);
        return;
    end
end

mat_fname = strrep( asc_fname, '.asc', '.mat');
pl_fname = which( 'asc2csvs.pl');

disp( [asc_fname]);

%%
tic;
disp( ['Converting to .csv files...']);
perl( pl_fname, asc_fname, dbc_fname);
toc

%%
tic;
disp( ['Converting to tscollections...']);
csv_list = ls( 'x*.csv');

raw = struct;
raw.info = struct;
raw.info.asc = asc_fname;
raw.info.dbc = dbc_fname;
raw.info.ver = '1.0';
for i=1:length(csv_list)
    id = strrep( csv_list(i,:), '.csv', '');
    id = strrep( id, ' ', '');
    dat = importdata( [id '.csv'], ',', 1);
    tsc = tscollection();
    tsc.Name = id;
    if isfield( dat, 'colheaders')
        if length( dat.colheaders) > 1
            for j=2:length( dat.colheaders)
                sig = dat.colheaders(j);
                ts = timeseries( dat.data(:,j), dat.data(:,1), 'Name', sig);
                tsc = tsc.addts( ts, char(sig));
            end
		else
			ts = timeseries( dat.data(:,1), dat.data(:,1), 'Name', 'time');
			tsc = tsc.addts( ts, 'NUL');
        end
	else
		ts = timeseries( dat.data(:,1), dat.data(:,1), 'Name', 'time');
		tsc = tsc.addts( ts, 'NUL');
    end
	raw.(id) = tsc;
end

raw = CAN_msg_stats( raw);

save( mat_fname, 'raw');
toc

%%
disp( ['Cleaning up...']);
for i=1:length(csv_list)
    csv_ = strrep( csv_list(i,:), ' ', '');
    delete( csv_);
end
disp( [mat_fname ' created...']);

disp( 'Done.');
