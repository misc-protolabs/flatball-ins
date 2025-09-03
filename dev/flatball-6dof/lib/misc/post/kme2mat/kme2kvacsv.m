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
