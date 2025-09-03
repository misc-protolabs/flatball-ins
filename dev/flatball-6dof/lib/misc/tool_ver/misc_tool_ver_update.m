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
