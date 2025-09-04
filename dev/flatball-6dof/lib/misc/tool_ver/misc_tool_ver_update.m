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
%              - Update Version Information
%-----------------------------------------------------------------------------------------------------------------------
% COMPANY:    mvschaeferLLC
%-----------------------------------------------------------------------------------------------------------------------
% COPYRIGHT:  Place copyright information here
%-----------------------------------------------------------------------------------------------------------------------
% PUBLIC FUNCTION:  misc_tool_ver_update()
%-----------------------------------------------------------------------------------------------------------------------
% DESCRIPTION:
%   Saves the expected version information for all tools needed for controller/plant code generation.
%   Typically the "expected" versions are those under Configuration Management for Production code generation
%-----------------------------------------------------------------------------------------------------------------------
% INPUTS                        [datatype]      Description
% None
%-----------------------------------------------------------------------------------------------------------------------
% OUTPUTS
%   misc_tool_ver_info_expected.mat                  .MAT file containing the expected TMW toolchain version information. 
%***********************************************************************************************************************
function misc_tool_ver_update()

    verInfoExpected_st = misc_tool_ver_get();
    verInfoPath_s = which( 'misc_tool_ver_update');
    verInfoPath_s = verInfoPath_s(1:end-17);
    save( [verInfoPath_s 'misc_tool_ver_info_expected.mat'], 'verInfoExpected_st');
end
