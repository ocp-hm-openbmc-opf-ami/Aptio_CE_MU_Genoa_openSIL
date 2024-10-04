## @file
#  Project MU based PostMemory modules include flash description file
#
#  Copyright (c) 2024, American Megatrends International LLC. All rights reserved.<BR>
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

#### MU MsCorePkg, PolicyServicePkg modules 
  INF PolicyServicePkg/PolicyService/DxeMm/PolicyDxe.inf

#### IPMI Feature Pkg modules
  INF IpmiFeaturePkg/GenericIpmi/Dxe/DxeGenericIpmi.inf
  INF IpmiFeaturePkg/GenericIpmi/Smm/SmmGenericIpmi.inf
  INF IpmiFeaturePkg/BmcAcpi/BmcAcpi.inf
  INF IpmiFeaturePkg/IpmiWatchdog/Dxe/IpmiWatchdogDxe.inf
  INF IpmiFeaturePkg/BmcAcpiPowerState/BmcAcpiPowerStateSmm.inf
  INF IpmiFeaturePkg/SpmiTable/SpmiTable.inf
  INF IpmiFeaturePkg/IpmiSmbios/IpmiSmbios.inf

#### MU_BASECORE modules
  INF SecurityPkg/RandomNumberGenerator/RngDxe/RngDxe.inf
