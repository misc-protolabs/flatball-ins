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

function kme2mat( kme_fname, dbc_fname, cleanup_flg)

if ~exist( kme_fname, 'file')
    kme_fname = [kme_fname '.kme40'];
    if ~exist( kme_fname, 'file')
        disp( ['File: ' kme_fname ' not found']);
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

csv_fname = strrep( which(kme_fname), '.kme40', '.kvacsv');
mat_fname = strrep( csv_fname, '.kvacsv', '.mat');
pl_fname = which( 'kvacsv2csvs.pl');

disp( [kme_fname]);

%%
tic;
disp( ['Converting to .kvacsv file...']);
kme2kvacsv( kme_fname);
toc

%%
tic;
disp( ['Converting to .csv files...']);
perl( pl_fname, csv_fname, dbc_fname);
toc

%%
tic;
disp( ['Converting to tscollections...']);
csv_list = ls( 'x*.csv');

raw = struct;
raw.info = struct;
raw.info.kme = kme_fname;
raw.info.dbc = dbc_fname;
raw.info.ver = '1.0';
sz = size( csv_list);
for i=1:sz(1)
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
cleanup = 0;
if nargin == 2
    cleanup = 1;
else
    if cleanup_flg == 1
        cleanup = 1;
    end
end
if cleanup
    disp( ['Cleaning up...']);
    sz = size( csv_list);
    for i=1:sz(1)
        csv_ = strrep( csv_list(i,:), ' ', '');
        delete( csv_);
    end
    delete( csv_fname);
    disp( [mat_fname ' created...']);
end

disp( 'Done.');
