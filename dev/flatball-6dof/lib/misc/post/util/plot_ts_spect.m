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
