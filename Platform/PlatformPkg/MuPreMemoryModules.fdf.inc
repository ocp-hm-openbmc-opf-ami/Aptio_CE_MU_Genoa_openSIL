## @file
#  Project MU based PreMemory modules include flash description file
#
#  Copyright (c) 2024, American Megatrends International LLC. All rights reserved.<BR>
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

#### MU MsCorePkg, PolicyServicePkg modules
  INF PolicyServicePkg/PolicyService/Pei/PolicyPei.inf
  INF MsCorePkg/Core/GuidedSectionExtractPeim/GuidedSectionExtract.inf  #for compressed PEIMs

#### IPMI Feature Pkg modules
  INF IpmiFeaturePkg/GenericIpmi/Pei/PeiGenericIpmi.inf
  INF IpmiFeaturePkg/IpmiWatchdog/Pei/IpmiWatchdogPei.inf
  INF IpmiFeaturePkg/IpmiPowerRestorePolicy/IpmiPowerRestorePolicy.inf

#### MU Platform Modules
  INF PlatformPkg/Pei/PlatformPolicyPei/PlatformPolicyPei.inf

#### MU_BASECORE modules
  !include $(SHARED_CRYPTO_PATH)/Driver/Bin/CryptoDriver.PEI.inc.fdf

  INF OemPkg/OemConfigPolicyCreatorPei/OemConfigPolicyCreatorPei.inf

