/** @file
  This file contains code to setup SMM Relocation Init

  Copyright (c) 2024, American Megatrends International LLC. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SmmRelocationLib.h>
#include <PiPei.h>
#include <Ppi/MpServices2.h>

/**
  The Entry point of this driver, run after memory Available.

  This function will call SmmRelocationInit 

  @param  FileHandle    Handle of the file being invoked.
  @param  PeiServices   Describes the list of possible PEI Services.

  @retval EFI_SUCCESS   MpServicePpi is installed successfully.
**/
EFI_STATUS
EFIAPI
SmmRebasePeiEntry(
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
    EFI_STATUS                  Status;
    EDKII_PEI_MP_SERVICES2_PPI  *MpServices;

    Status = PeiServicesLocatePpi (
                &gEdkiiPeiMpServices2PpiGuid,
                0,
                NULL,
                (VOID **)&MpServices
                );

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: Locate MpServices Ppi fail, status %r\n", __func__, Status));
        ASSERT_EFI_ERROR (Status);
        return Status;
    }

    Status = SmmRelocationInit(MpServices);
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: Call SmmRelocationInit return fail, status %r\n", __func__, Status));
        ASSERT_EFI_ERROR (Status);
    }

    return Status;
}
