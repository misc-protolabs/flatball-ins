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


%***********************************************************************************************************************
% TITLE:      Toolchain Version Function
%              - Get Version Information
%-----------------------------------------------------------------------------------------------------------------------
% COMPANY:    mvschaeferLLC
%-----------------------------------------------------------------------------------------------------------------------
% COPYRIGHT:  Place copyright information here
%-----------------------------------------------------------------------------------------------------------------------
% PUBLIC FUNCTION:  misc_tool_ver_get()
%-----------------------------------------------------------------------------------------------------------------------
% DESCRIPTION:
%   Gathers the version information of all tools in use for controller/plant code generation.
%-----------------------------------------------------------------------------------------------------------------------
% INPUTS                [datatype]      Description
%
%-----------------------------------------------------------------------------------------------------------------------
% OUTPUTS
%   verInfo_st_o         struct         Data structure containing the present TMW toolchain version information. 
%***********************************************************************************************************************
function verInfoNow_st_o = misc_tool_ver_get()

        %% Gather the toolchain versions presently in use
    verInfoNow_st_o = ver( 'matlab');
    try
        verInfoNow_st_o(end+1) = ver( 'simulink');
    catch
        warning('PLATFORM:ToolchainVersion', 'Simulink is missing so model viewing/editing/simulation will not be possible');
    end

    try
        verInfoNow_st_o(end+1) = ver( 'stateflow');
    catch
        warning('PLATFORM:ToolchainVersion', 'Stateflow is missing so model viewing/editing/simulation will not be possible');
    end

    try
        verInfoNow_st_o(end+1) = ver( 'simscape');
    catch
        warning('PLATFORM:ToolchainVersion', 'Simscape is missing so plant model viewing/editing/simulation will not be possible');
    end

    try
        verInfoNow_st_o(end+1) = ver( 'matlabcoder');
    catch
        warning('PLATFORM:ToolchainVersion', 'MATLAB coder is missing so code generation will not be possible');
    end

    try
        verInfoNow_st_o(end+1) = ver( 'simulinkcoder');
    catch
        warning('PLATFORM:ToolchainVersion', 'Simulink coder is missing so code generation will not be possible');
    end

    try
        verInfoNow_st_o(end+1) = ver( 'embeddedcoder');
    catch
        warning('PLATFORM:ToolchainVersion', 'Embedded coder is missing so PLC code generation will not be possible');
    end
end