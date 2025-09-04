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


function ts_struct_filt = apply_filters( ts_struct)

ts_struct_filt = ts_struct;
% z = sqrt(2)/2;
% wc = 50;

signals = fieldnames( ts_struct_filt);
for i=1:length(signals)
    sig = char( signals(i));
    m = regexpi( sig, '^P\d+$');
    if ~isempty(m)
        % pressure signal found
        wc = 50;
        ts_struct_filt.(sig) = ts_filter( ts_struct_filt.(sig), 1/wc);
    end
    if strcmp( sig, 'P4')
        % water level signal found
        wc = 10;
        ts_struct_filt.(sig) = ts_filter( ts_struct_filt.(sig), 1/wc);
    end
    m = regexpi( sig, '^T\d+$');
    if ~isempty(m)
        % temperature signal found
        wc = 1;
        ts_struct_filt.(sig) = ts_filter( ts_struct_filt.(sig), 1/wc);
    end
    if strcmp( sig, 'KVA80') || strcmp( sig, 'KVA75')
        % water level signal found
        wc = 50;
        ts_struct_filt.(sig) = ts_filter( ts_struct_filt.(sig), 1/wc);
    end
    if strcmp( sig, 'L1') || strcmp( sig, 'L2')
        % water level signal found
        wc = 1;
        ts_struct_filt.(sig) = ts_filter( ts_struct_filt.(sig), 1/wc);
    end
    if strcmp( sig, 'L10') || strcmp( sig, 'L11')
        % convert platen position to pct
        wc = 50;
        ts_struct_filt.(sig) = ts_filter( ts_struct_filt.(sig), 1/wc);
    end
    if strcmp( sig, 'S10') || strcmp( sig, 'S11')
        % convert platen speed
        wc = 50;
        ts_struct_filt.(sig) = ts_filter( ts_struct_filt.(sig), 1/wc);
    end
    if strcmp( sig, 'dD61dt')
        % 
        wc = 25;
        ts_struct_filt.(sig) = ts_filter( ts_struct_filt.(sig), 1/wc);
    end
    if strcmp( sig, 'S61') || ...
            strcmp( sig, 'S70') || ...
            strcmp( sig, 'S80')
        % convert rotational speeds
        wc = 50;
        ts_struct_filt.(sig) = ts_filter( ts_struct_filt.(sig), 1/wc);
    end
    if strcmp( sig, 'SG1') || strcmp( sig, 'SG2') || ...
            strcmp( sig, 'SG3') || strcmp( sig, 'SG4') || ...
            strcmp( sig, 'SG5') || strcmp( sig, 'SG6') || ...
            strcmp( sig, 'SG7') || strcmp( sig, 'SG8')
        % convert strain gauges
        wc = 50;
        ts_struct_filt.(sig) = ts_filter( ts_struct_filt.(sig), 1/wc);
    end
end
