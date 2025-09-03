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
