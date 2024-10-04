/** @file
  Implements sample policy for PEI environment.

  Copyright (c) Microsoft Corporation
  Copyright (c) 2024, American Megatrends International LLC. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PolicyLib.h>
#include <Guid/IpmiWatchdogPolicy.h>
#include <Guid/PlatformPowerRestorePolicy.h>
#include <IndustryStandard/IpmiNetFnApp.h>
#include <Ppi/Policy.h>
#include <AutoGen/Generated/ConfigClientGenerated.h>
#include <AutoGen/Generated/ConfigServiceGenerated.h>

/**
  Entry point for the policy PEIM. Installs the policy service PPI.

  @param[in] FileHandle   -  Pointer to Firmware File System file header.
  @param[in] PeiServices  -  General purpose services available to every PEIM.

  @retval EFI_SUCCESS  -  Successfully ran policy code.
  @retval Others       -  Failure status returned by policy interface.

**/
EFI_STATUS
EFIAPI
PeiPolicyEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                      Status;
  IPMI_WATCHDOG_POLICY            IpmiWatchdogPolicy;
  PLATFORM_POWER_RESTORE_POLICY   PlatformPowerRestorePolicy;
  PLATFORM_CONFIG                 PlatformConfig;
  FRB2_CONFIG                     Frb2Config;
  WATCHDOG_CONFIG                 WatchDogConfig;

  Status = ConfigGetFrb2Config (&Frb2Config);
  if (!EFI_ERROR (Status)) {
    IpmiWatchdogPolicy.Frb2Enabled = Frb2Config.BiosFrb2En;
    IpmiWatchdogPolicy.Frb2TimeoutAction = IPMI_WATCHDOG_TIMER_ACTION_HARD_RESET;
    IpmiWatchdogPolicy.Frb2TimeoutSeconds = Frb2Config.BiosFrb2Timeout * 60;
  } else {
    //Set default Policy
    IpmiWatchdogPolicy.Frb2Enabled = 1;
    IpmiWatchdogPolicy.Frb2TimeoutAction = IPMI_WATCHDOG_TIMER_ACTION_NO_ACTION;
    IpmiWatchdogPolicy.Frb2TimeoutSeconds = 600;
  }

  Status = ConfigGetWatchDogConfig (&WatchDogConfig);
  if (!EFI_ERROR (Status)) {
    IpmiWatchdogPolicy.OsTimeoutAction = WatchDogConfig.OsBootWdtTimeoutAction;
    IpmiWatchdogPolicy.OsTimeoutSeconds = WatchDogConfig.OsBootWdtTimeout * 60;
    IpmiWatchdogPolicy.OsWatchdogEnabled = WatchDogConfig.OsBootWdt;
  } else {
    //Set default Policy
    IpmiWatchdogPolicy.OsTimeoutAction = 0;
    IpmiWatchdogPolicy.OsTimeoutSeconds = 0;
    IpmiWatchdogPolicy.OsWatchdogEnabled = 0;
  }

  Status = ConfigGetPlatformConfig (&PlatformConfig);
  if (!EFI_ERROR (Status)) {
    PlatformPowerRestorePolicy.PolicyValue = PlatformConfig.PowerRestorePolicy;
  } else {
    //Set default Policy
    PlatformPowerRestorePolicy.PolicyValue = PowerRestorePolicyNoChange;
  }

  Status = SetPolicy (
                &gIpmiWatchdogPolicyGuid, 
                POLICY_ATTRIBUTE_FINALIZED, 
                &IpmiWatchdogPolicy, 
                sizeof (IPMI_WATCHDOG_POLICY));
  if (EFI_ERROR(Status)) {
    return Status;
  }

  Status = SetPolicy (
                &gPlatformPowerRestorePolicyGuid, 
                POLICY_ATTRIBUTE_FINALIZED, 
                &PlatformPowerRestorePolicy, 
                sizeof (PLATFORM_POWER_RESTORE_POLICY));
  return Status;
}
