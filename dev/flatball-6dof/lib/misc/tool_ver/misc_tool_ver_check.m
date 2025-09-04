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
%              - Check Expected Version Information
%-----------------------------------------------------------------------------------------------------------------------
% COMPANY:    mvschaeferLLC
%-----------------------------------------------------------------------------------------------------------------------
% COPYRIGHT:  Place copyright information here
%-----------------------------------------------------------------------------------------------------------------------
% PUBLIC FUNCTION:  misc_tool_ver_update()
%-----------------------------------------------------------------------------------------------------------------------
% DESCRIPTION:
%   Verifies that the toolchain versions in use match the versions expected for controller/plant code generation.
%-----------------------------------------------------------------------------------------------------------------------
% INPUTS                [datatype]      Description
%
%-----------------------------------------------------------------------------------------------------------------------
% OUTPUTS               [datatype]      Description
%
%***********************************************************************************************************************
function misc_tool_ver_check(varargin)

    testing_flg = false;
    if(isempty(varargin))
        testing_flg = false;
    else
        try
            if(strcmp('testing',char(varargin(1))))
                testing_flg = true;
            end
        catch
            testing_flg = false;
        end

    end

        %% Verify toolchain versions
    try
            %% Load the expected toolchain versions
        verInfoPath_s = which( 'misc_tool_ver_check');
        verInfoPath_s = verInfoPath_s(1:end-16);
        load( [verInfoPath_s 'misc_tool_ver_info_expected.mat']);

            %% Gather the toolchain versions presently in use
        verInfoNow_st = misc_tool_ver_get();

            %% Verify that present versions match expected versions
        stat_flg = true;
        for i=1:length(verInfoExpected_st)
            match_flg = false;
            for j = 1:length(verInfoNow_st)
                if( strcmp(verInfoExpected_st(i).Name, verInfoNow_st(j).Name) )
                    match_flg = true;
                    
                        % Report tool version verification status
                    if( strcmp(verInfoExpected_st(i).Version, verInfoNow_st(j).Version) )
                        msg = ['.. ' verInfoExpected_st(i).Name ' ' verInfoExpected_st(i).Release];
                    else
                        msg = ['.. ' verInfoExpected_st(i).Name ', expected : ' verInfoExpected_st(i).Version ' ' verInfoExpected_st(i).Release ', found : ' verInfoNow_st(j).Version ' ' verInfoNow_st(i).Release];
                        stat_flg = false;
                    end
                    %disp( msg);
                    misc_dbg( msg);
                end
            end
            if( match_flg == false)
                msg = ['    No match : ' verInfoExpected_st(i).Name];
                %disp(msg);
                misc_dbg( msg);
            end
        end

            %% Inform user of the PLATFORM's toolchain status 
        if ((stat_flg == false) && (testing_flg == false) )
            disp ' ';
            disp 'You are operating the Development Platform with an unexpected combination of toolchain versions.';
            disp ' -- It is strongly recommended that you close MATLAB and try again.';
            disp ' ';
            c = input( 'Do you wish to proceed regardless? y/[n] ', 's');
            disp ' ';
            if( (~isempty(regexpi(c,'^Y'))) )
                    disp '!!! Be Careful. Ignoring PLATFORM toolchain version mismatch !!!';
            else
                    disp '!!! Aborting PLATFORM session !!!';
                    exit;
            end
        end
    catch
        misc_tool_ver_update;
        error('PLATFORM: Toolchain Version Check failure.')
    end
end