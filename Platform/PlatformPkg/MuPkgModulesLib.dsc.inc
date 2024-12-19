## @file
#  Project MU based dsc include file
#
#  Copyright (c) 2024, American Megatrends International LLC. All rights reserved.<BR>
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  DEFINE PEI_CRYPTO_SERVICES = TINY_SHA
  DEFINE DXE_CRYPTO_SERVICES = STANDARD
  DEFINE RUNTIMEDXE_CRYPTO_SERVICES = STANDARD
  DEFINE SMM_CRYPTO_SERVICES = STANDARD
  DEFINE STANDALONEMM_CRYPTO_SERVICES = NONE
  DEFINE PEI_CRYPTO_ARCH = IA32
  DEFINE DXE_CRYPTO_ARCH = X64
  DEFINE SMM_CRYPTO_ARCH = X64
  DEFINE RUNTIMEDXE_CRYPTO_ARCH = X64
  DEFINE STANDALONEMM_CRYPTO_ARCH = NONE

  !include $(SHARED_CRYPTO_PATH)/Driver/Bin/CryptoDriver.inc.dsc

[Packages]
  PcBdsPkg/PcBdsPkg.dec

[PcdsFixedAtBuild]
### MU PcBdsPkg
!if $(BUILTIN_SHELL_BOOT_ENABLE) == TRUE
  #Shell FV(FV.FvUefiBootUncompact) GUID E889A6E3-385B-4DAF-A19A-E9B1D41EB046
  gPcBdsPkgTokenSpaceGuid.PcdShellFvGuid|{ 0xE3, 0xA6, 0x89, 0xE8, 0x5B, 0x38, 0xAF, 0x4D, 0xA1, 0x9A, 0xE9, 0xB1, 0xD4, 0x1E, 0xB0, 0x46}

  #shell file guid 7C04A583-9E3E-4f1c-AD65-E05268D0B4D1
  gPcBdsPkgTokenSpaceGuid.PcdShellFile|{0x83, 0xA5, 0x04, 0x7C, 0x3E, 0x9E, 0x1C, 0x4F, 0xAD, 0x65, 0xE0, 0x52, 0x68, 0xD0, 0xB4, 0xD1}
!endif

[PcdsFeatureFlag]
  gEfiMdeModulePkgTokenSpaceGuid.PcdRequireIommu|FALSE

[LibraryClasses]
  ExceptionPersistenceLib|MdeModulePkg/Library/BaseExceptionPersistenceLibNull/BaseExceptionPersistenceLibNull.inf
  
### MU BASE_CORE
  PolicyLib|PolicyServicePkg/Library/DxePolicyLib/DxePolicyLib.inf
  ImagePropertiesRecordLib|MdeModulePkg/Library/ImagePropertiesRecordLib/ImagePropertiesRecordLib.inf
  DxeMemoryProtectionHobLib|MdeModulePkg/Library/MemoryProtectionHobLib/DxeMemoryProtectionHobLib.inf
  MemoryTypeInfoSecVarCheckLib|MdeModulePkg/Library/MemoryTypeInfoSecVarCheckLib/MemoryTypeInfoSecVarCheckLib.inf
  DeviceStateLib|MdeModulePkg/Library/DeviceStateLib/DeviceStateLib.inf
  SecurityLockAuditLib|MdeModulePkg/Library/SecurityLockAuditLibNull/SecurityLockAuditLibNull.inf
  AdvLoggerAccessLib|MdeModulePkg/Library/AdvLoggerAccessLibNull/AdvLoggerAccessLib.inf
  MemoryTypeInformationChangeLib|MdeModulePkg/Library/MemoryTypeInformationChangeLibNull/MemoryTypeInformationChangeLibNull.inf
  PanicLib|MdePkg/Library/BasePanicLibNull/BasePanicLibNull.inf
  StackCheckFailureHookLib|MdePkg/Library/StackCheckFailureHookLibNull/StackCheckFailureHookLibNull.inf
  MemoryBinOverrideLib|MdeModulePkg/Library/MemoryBinOverrideLibNull/MemoryBinOverrideLibNull.inf

### MU MsCorePkg
  PlatformBootManagerLib|MsCorePkg/Library/PlatformBootManagerLib/PlatformBootManagerLib.inf

### MU TIANO
  SecureBootVariableLib|SecurityPkg/Library/SecureBootVariableLib/SecureBootVariableLib.inf
  PlatformPKProtectionLib|SecurityPkg/Library/PlatformPKProtectionLibVarPolicy/PlatformPKProtectionLibVarPolicy.inf
  OemTpm2InitLibVendor|SecurityPkg/Library/OemTpm2InitLibNull/OemTpm2InitLibVendorNull.inf

### MU GraphicsPkg
  BootGraphicsLib|MsGraphicsPkg/Library/BootGraphicsLibNull/BootGraphicsLib.inf
  BootGraphicsProviderLib|MsGraphicsPkg/Library/BootGraphicsProviderLibNull/BootGraphicsProviderLib.inf
  
### MU PcBdsPkg
  MsBootOptionsLib|PcBdsPkg/Library/MsBootOptionsLib/MsBootOptionsLib.inf
  GraphicsConsoleHelperLib|PcBdsPkg/Library/GraphicsConsoleHelperLib/GraphicsConsoleHelper.inf
  MsBootPolicyLib|PcBdsPkg/Library/MsBootPolicyLibNull/MsBootPolicyLibNull.inf
  ConsoleMsgLib|PcBdsPkg/Library/ConsoleMsgLibNull/ConsoleMsgLibNull.inf
  MsNetworkDependencyLib|PcBdsPkg/Library/MsNetworkDependencyLib/MsNetworkDependencyLib.inf
  MsNVBootReasonLib|PcBdsPkg/Library/MsNVBootReasonLibNull/MsNVBootReasonLibNull.inf
  MsAltBootLib|PcBdsPkg/Library/MsAltBootLibNull/MsAltBootLibNull.inf

#### MU Feature SetupDataPkg
  ConfigVariableListLib|SetupDataPkg/Library/ConfigVariableListLib/ConfigVariableListLib.inf

#### IPMI Feature Pkg modules
  IpmiTransportLib|IpmiFeaturePkg/Library/IpmiTransportLibKcs/KcsIpmiTransportLib.inf
  IpmiWatchdogLib|IpmiFeaturePkg/Library/IpmiWatchdogLib/IpmiWatchdogLib.inf
  IpmiCommandLib|IpmiFeaturePkg/Library/IpmiCommandLib/IpmiCommandLib.inf
  IpmiBaseLib|IpmiFeaturePkg/Library/IpmiBaseLibDxe/IpmiBaseLibDxe.inf
  IpmiSelLib|IpmiFeaturePkg/Library/IpmiSelLib/IpmiSelLib.inf
  IpmiBootOptionLib|IpmiFeaturePkg/Library/IpmiBootOptionLib/IpmiBootOptionLib.inf

### MU Platform Libraries
  MsPlatformDevicesLib|PlatformPkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf
  DeviceBootManagerLib|PlatformPkg/Library/DeviceBootManagerLib/DeviceBootManagerLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  ActiveProfileIndexSelectorLib|OemPkg/Library/ActiveProfileIndexSelectorPcdLib/ActiveProfileIndexSelectorPcdLib.inf

[LibraryClasses.common.PEIM]
#### MU BASE_CORE
  PolicyLib|PolicyServicePkg/Library/PeiPolicyLib/PeiPolicyLib.inf
  PcdDatabaseLoaderLib|MdeModulePkg/Library/PcdDatabaseLoaderLib/Pei/PcdDatabaseLoaderLibPei.inf

#### MU Feature SetupDataPkg
  ConfigKnobShimLib|SetupDataPkg/Library/ConfigKnobShimLib/ConfigKnobShimPeiLib/ConfigKnobShimPeiLib.inf

#### IPMI Feature Pkg modules
  IpmiBaseLib|IpmiFeaturePkg/Library/IpmiBaseLibPei/IpmiBaseLibPei.inf

[LibraryClasses.common.DXE_DRIVER]
#### MU BASE_CORE
  PcdDatabaseLoaderLib|MdeModulePkg/Library/PcdDatabaseLoaderLib/Dxe/PcdDatabaseLoaderLibDxe.inf

### MU MsCorePkg
  SecureBootKeyStoreLib|MsCorePkg/Library/SecureBootKeyStoreLibNull/SecureBootKeyStoreLibNull.inf

[LibraryClasses.Common.DXE_SMM_DRIVER]
#### MU BASE_CORE
  MmMemoryProtectionHobLib|MdeModulePkg/Library/MemoryProtectionHobLib/SmmMemoryProtectionHobLib.inf
  SmmCpuSyncLib|UefiCpuPkg/Library/SmmCpuSyncLib/SmmCpuSyncLib.inf

[LibraryClasses.Common.SMM_CORE]
#### MU BASE_CORE
  MmMemoryProtectionHobLib|MdeModulePkg/Library/MemoryProtectionHobLib/SmmMemoryProtectionHobLib.inf

[LibraryClasses.common.SEC, LibraryClasses.common.PEI_CORE]
#### MU BASE_CORE
  NULL|MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf

[LibraryClasses.common.PEIM, LibraryClasses.common.MM_CORE_STANDALONE, LibraryClasses.common.MM_STANDALONE]
#### MU BASE_CORE
  NULL|MdePkg/Library/StackCheckLib/StackCheckLibStaticInit.inf

[LibraryClasses.common.DXE_CORE, LibraryClasses.common.SMM_CORE, LibraryClasses.common.DXE_SMM_DRIVER, LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.DXE_RUNTIME_DRIVER, LibraryClasses.common.DXE_SAL_DRIVER, LibraryClasses.common.UEFI_DRIVER, LibraryClasses.common.UEFI_APPLICATION]
#### MU BASE_CORE  
  NULL|MdePkg/Library/StackCheckLib/StackCheckLibDynamicInit.inf

[LibraryClasses.common.DXE_SMM_DRIVER,LibraryClasses.common.SMM_CORE]
#### IPMI Feature Pkg modules
  IpmiBaseLib|IpmiFeaturePkg/Library/IpmiBaseLibSmm/IpmiBaseLibSmm.inf


[Components.IA32]
#### MU MsCorePkg, PolicyServicePkg modules
  PolicyServicePkg/PolicyService/Pei/PolicyPei.inf
  MsCorePkg/Core/GuidedSectionExtractPeim/GuidedSectionExtract.inf {
    <LibraryClasses>
      NULL|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf
  }

#### IPMI Feature Pkg modules
  IpmiFeaturePkg/GenericIpmi/Pei/PeiGenericIpmi.inf
  IpmiFeaturePkg/IpmiWatchdog/Pei/IpmiWatchdogPei.inf
  IpmiFeaturePkg/IpmiPowerRestorePolicy/IpmiPowerRestorePolicy.inf

#### MU Platform Modules
  PlatformPkg/Pei/PlatformPolicyPei/PlatformPolicyPei.inf

  OemPkg/OemConfigPolicyCreatorPei/OemConfigPolicyCreatorPei.inf {
    <LibraryClasses>
      NULL|PlatformPkg/Library/PlatformConfigDataLib/PlatformConfigDataLib.inf
  }

[Components.X64]
#### MU MsCorePkg, PolicyServicePkg modules
  PolicyServicePkg/PolicyService/DxeMm/PolicyDxe.inf

#### IPMI Feature Pkg modules
  IpmiFeaturePkg/GenericIpmi/Dxe/DxeGenericIpmi.inf {
  <PcdsFixedAtBuild>
    gIpmiFeaturePkgTokenSpaceGuid.PcdIpmiCheckSelfTestResults|TRUE
  }
  IpmiFeaturePkg/GenericIpmi/Smm/SmmGenericIpmi.inf
  IpmiFeaturePkg/BmcAcpi/BmcAcpi.inf
  IpmiFeaturePkg/IpmiWatchdog/Dxe/IpmiWatchdogDxe.inf
  IpmiFeaturePkg/BmcAcpiPowerState/BmcAcpiPowerStateSmm.inf
  IpmiFeaturePkg/SpmiTable/SpmiTable.inf
  IpmiFeaturePkg/IpmiSmbios/IpmiSmbios.inf

#### MU_BASECORE modules
  SecurityPkg/RandomNumberGenerator/RngDxe/RngDxe.inf

#### MU Platform Modules
