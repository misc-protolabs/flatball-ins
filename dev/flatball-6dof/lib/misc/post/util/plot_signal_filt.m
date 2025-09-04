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


function plot_signal_filt( data, signal, y_m, y_b, tau, plotstyle, varargin)

if isfield( data, signal)
    x = ts_filter( data.(signal), tau);
    hold on
    h = plot(x.Time, x.Data .* y_m + y_b, plotstyle);
    if nargin > 1
        if rem(length(varargin), 2) == 0
            for ii = 2:2:length(varargin)
                set(h, varargin{ii-1}, varargin{ii});
            end
        end
    end
    hold off
    update_legend( signal);
end
    