// Copyright 2025 Michael V. Schaefer
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "sfe_ism_shim.h"
#include "sfe_ism330dhcx.h"
#include "st_src/ism330dhcx_reg.h"


// Initializes the interfacing struct for STMicroelectronic's library. 
void initCtx(void* handle, stmdev_ctx_t* dev){

	dev->handle = handle; 	
	dev->write_reg = sfeISMWrite;
	dev->read_reg = sfeISMRead;
}

// Points to the write method in the ISM330DHCX Arduino Library
int32_t sfeISMWrite(void* fTarget, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
	return (((QwDevISM330DHCX*)fTarget)->writeRegisterRegion(reg, (uint8_t*)bufp, len));
}

// Points to the read method in the ISM330DHCX Arduino Library
int32_t sfeISMRead(void* fTarget, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    return (((QwDevISM330DHCX*)fTarget)->readRegisterRegion(reg, bufp, len));
}



