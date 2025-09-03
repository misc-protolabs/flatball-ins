function pfUtil_plot_ts_spect_f( ts)

%%
Fs = 1 / (ts.Time(2) - ts.Time(1));
L = length( ts.Time);
NFFT = 2^nextpow2( L); % Next power of 2 from length of y

%%
ts.Data( isnan( ts.Data)) = 0;
Y = fft( ts.Data, NFFT) / L;
f = Fs/2 * linspace( 0, 1, NFFT/2+1);
y = 2 * abs( Y(1:NFFT/2+1));

%%
subplot( 211);
ts.plot;
% Plot single-sided amplitude spectrum.
subplot( 212);
loglog( f, y);
% ylim( [-5 max(y)]);
xlim( [0.1 round( (Fs/2))]);
title('Single-Sided Amplitude Spectrum of y(t)');
xlabel('Frequency (Hz)');
ylabel('|Y(f)|');
