## @file
#  Project MU based Uefi boot modules include flash description file
#
#  Copyright (c) 2024, American Megatrends International LLC. All rights reserved.<BR>
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

#### MU MsCorePkg, PolicyServicePkg modules 

#### MU_BASECORE modules
  !include $(SHARED_CRYPTO_PATH)/Driver/Bin/CryptoDriver.SMM.inc.fdf
  !include $(SHARED_CRYPTO_PATH)/Driver/Bin/CryptoDriver.DXE.inc.fdf

