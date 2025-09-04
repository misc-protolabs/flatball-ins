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


function pfUtil_plot_signal_f( data, signal, y_m, y_b, plotstyle, varargin)

flds = data.fieldnames;

if sum(cellfun(@(x) strcmp(x, signal), flds))
% if isfield( data, signal)
    hold on
    h = plot(data.(signal).Time, data.(signal).Data .* y_m + y_b, plotstyle);
    if nargin > 1
        if rem(length(varargin), 2) == 0
            for ii = 2:2:length(varargin)
                set(h, varargin{ii-1}, varargin{ii});
            end
        end
    end
    hold off
    pfUtil_update_legend_f( signal);
end
    