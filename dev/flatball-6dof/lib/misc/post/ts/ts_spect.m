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


function [f, P_filt] = ts_spect( ts, tau)

dt = mean(diff(ts.Time));
hz = 1/dt;

ts_filt = ts_filter( ts, tau);
% ts_filt = ts_filter( ts, tau);
% ts_filt = ts_filt( ts, sqrt(2)/2, 1/dt);
% ts_filt = ts_filt( ts, z, wn);

y = ts.Data;
y_filt = ts_filt.Data;

n = length(y); % Window length
f = (0:n-1)*(hz/n); % Frequency range

Y = fft(y); % DFT of data
P = Y.*conj(Y)/n; % Power of the DFT

Y_filt = fft(y_filt); % DFT of data
P_filt = Y_filt.*conj(Y_filt)/n; % Power of the DFT

% figure;
% semilogy(f(1:uint32(end/2)),P(1:uint32(end/2)),'-b');
% 
% Y = fft(y_filt); % DFT of data
% P = Y.*conj(Y)/n; % Power of the DFT
% 
% hold on;
% semilogy(f(1:uint32(end/2)),P(1:uint32(end/2)),'-r');
% hold off;
% 
% title( [ts.Name ' Spectrum']);
