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


%% UPDATE_LEGEND Adds signal descriptions to the present plot legend
%------------------------------------------------------------------------------------------------------------------------------------
function pfUtil_update_legend_f(varargin)

        % Initialize
    [lgnd_h, obj_h, plot_h, sigDesc_cs] = legend(); % Get information about the present legend
    newSig_cnt = length(varargin);                  % total number of new signal descriptions to add
    oldLgndSz_cnt = length(sigDesc_cs);             % Save the original or old legend size
    begin_idx = length(sigDesc_cs) + 1;               
    end_idx = begin_idx + newSig_cnt - 1;
    
        % Add the new signal descriptions
    try
        for j = begin_idx:end_idx
            sigDesc_cs{j} = strrep( varargin{j - oldLgndSz_cnt}, '_', '\_');
        end % for
    catch err_msg
        error('Mismatch between number of signals in the plot and number of descriptions in the legend');
    end %
    
        % Add the updated legend
    legend(sigDesc_cs);
    legend('boxoff');
end % function