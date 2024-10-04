/** @file
  Platform specific implementation of the IPMI platform library.

  Copyright (c) Microsoft Corporation
  Copyright (c) 2024, American Megatrends International LLC. All rights reserved.<BR>
  PDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IpmiPlatformLib.h>
#include <Library/PciLib.h>
#include <Fch/FchIsaReg.h>

#define REG_GEN2_DEC            0x88
#define LPC_DECODE_ENABLE       0x01
#define KCS_BASE_ADDRESS_MASK   0xFFF0
#define ENABLE_16_BYTE          0x000C0000
#define LPC_GEN_PMCON_3         0xA4
#define PWR_FLR_BIT             BIT1

/**
  Checks for the results of device specific self test codes.

  @param[in]  Result      The self-test result to be checked by the platform.
  @param[out] BmcStatus   A pointer that receives the BMC status change if needed.
**/
VOID
EFIAPI
GetDeviceSpecificTestResults (
  IN  IPMI_SELF_TEST_RESULT_RESPONSE  *Result,
  OUT BMC_STATUS                      *BmcStatus
  )
{
  *BmcStatus = BMC_HARDFAIL;
  return;
}

/**
  Sets a platform IO range for use in the IPMI stack.

  @param[in]    IpmiIoBase      The IO port range to set.

  @retval       EFI_SUCCESS     The port was successfully set.
**/
EFI_STATUS
EFIAPI
PlatformIpmiIoRangeSet (
  IN UINT16  IpmiIoBase
  )
{

  DEBUG ((DEBUG_INFO, "%a - IpmiIoBase: 0x%x\n", __func__, IpmiIoBase));

  // Enable decoding Lpc BMC base address region. Enable 16 Bytes.
  PciWrite32 (
        ((UINTN) PCI_LIB_ADDRESS(FCH_LPC_BUS, FCH_LPC_DEV, FCH_LPC_FUNC, REG_GEN2_DEC)), \
        ((IpmiIoBase & KCS_BASE_ADDRESS_MASK) | \
        LPC_DECODE_ENABLE) | ENABLE_16_BYTE );

  return EFI_SUCCESS;
}

/**
  Performs any platform specific initialization needed for the BMC or IPMI
  stack.

  @retval   EFI_SUCCESS   The platform initialization was successfully completed.
**/
EFI_STATUS
EFIAPI
PlatformIpmiInitialize (
  VOID
  )
{
  return EFI_SUCCESS;
}
