/*****************************************************************************
 *
 * Copyright (C) 2020-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

/**
  AMD PCIe Resource Protocol Driver
  
**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcieResourcesLib.h>
#include <Protocol/AmdPciResourcesProtocol.h>


/**
  *Function to get the number of root bridges
  *
  *@param[in]      This                  Pointer to this protocol
  *@param[in,out]  NumberOfRootBridges   Number of root bridges returned
  *
**/
EFI_STATUS
EFIAPI
PciResourcesGetNumberOfRootBridges (
  IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
  OUT      UINTN                                 *NumberOfRootBridges
  )
{
  return AmdPciResourcesGetNumberOfRootBridges (NumberOfRootBridges);
}

/**
  *Function to get the root bridge info
  *
  *@param[in]     This                   Pointer to this protocol
  *@param[in]     RootBridgeIndex        Root bridge index for which the root bridge info will be returned
  *@param[out]    RootBridgeInfo         Returned Root bridge info
  *
**/
EFI_STATUS
EFIAPI
PciResourcesGetRootBridgeInfo (
  IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
  IN       UINTN                                 RootBridgeIndex,
  OUT      PCI_ROOT_BRIDGE_OBJECT                **RootBridgeInfo
  )
{
  return AmdPciResourcesGetRootBridgeInfo (
                          RootBridgeIndex,
                          RootBridgeInfo
                          );
}

/**
  *Function to get the number of root ports
  *
  *@param[in]      This                Pointer to this protocol
  *@param[in]      RootBridgeIndex     Root bridge index for which the no of root ports are returned
  *@param[in,out]  NumberOfRootPorts   Number of root ports returned
  *
**/
EFI_STATUS
EFIAPI
PciResourcesGetNumberOfRootPorts (
  IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
  IN       UINTN                                 RootBridgeIndex,
  OUT      UINTN                                 *NumberOfRootPorts
  )
{
  return AmdPciResourcesGetNumberOfRootPorts(
                            RootBridgeIndex,
                            NumberOfRootPorts
                            );
}

/**
  *Function to get the root port info
  *
  *@param[in]     This                   Pointer to this protocol
  *@param[in]     RootBridgeIndex        Root bridge index for which the root port info will be returned
  *@param[in]     RootPortIndex          Root port index for which the root port info will be returned
  *@param[out]    RootPortInfo           Returned Root port info
  *
**/
EFI_STATUS
EFIAPI
PciResourcesGetRootPortInfo (
  IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
  IN       UINTN                                 RootBridgeIndex,
  IN       UINTN                                 RootPortIndex,
  OUT      PCI_ROOT_PORT_OBJECT                  **RootPortInfo
  )
{
  return AmdPciResourcesGetRootPortInfo (
                      RootBridgeIndex,
                      RootPortIndex,
                      RootPortInfo
                      );
}

/**
  *Function to get the number of fixed resources
  *
  *@param[in]     This                      Pointer to this protocol
  *@param[in]      RootBridgeIndex          Root bridge index for which the no of fixed resources are returned
  *@param[in,out]  NumberOfFixedResources   Number of fixed resources returned
  *
**/
EFI_STATUS
EFIAPI
PciResourcesGetNumberOfFixedResources (
  IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
  IN       UINTN                                 RootBridgeIndex,
  OUT      UINTN                                 *NumberOfFixedResources
  )
{
  return AmdPciResourcesGetNumberOfFixedResources (
                          RootBridgeIndex,
                          NumberOfFixedResources
                          );
}


/**
  *Function to get the fixed resource info
  *
  *@param[in]     This                   Pointer to this protocol
  *@param[in]     RootBridgeIndex        Root bridge index for which the fixed resource info will be returned
  *@param[in]     FixedResourceIndex     Fixed resource index for which the fixed resource info will be returned
  *@param[out]    FixedResourceInfo      Returned fixed resource info
  *
**/
EFI_STATUS
EFIAPI
PciResourcesGetFixedResourceInfo (
  IN       AMD_PCI_RESOURCES_PROTOCOL            *This,
  IN       UINTN                                 RootBridgeIndex,
  IN       UINTN                                 FixedResourceIndex,
  OUT      FIXED_RESOURCES_OBJECT                **FixedResourceInfo
  )
  {
    return AmdPciResourcesGetFixedResourceInfo (
                            RootBridgeIndex,
                            FixedResourceIndex,
                            FixedResourceInfo
                            );
  }

AMD_PCI_RESOURCES_PROTOCOL      AmdPciResources = {
    PciResourcesGetNumberOfRootBridges,
    PciResourcesGetRootBridgeInfo,
    PciResourcesGetNumberOfRootPorts,
    PciResourcesGetRootPortInfo,
    PciResourcesGetNumberOfFixedResources,
    PciResourcesGetFixedResourceInfo
  };

/**
  Install AMD PCIe resource Prorocol

  @param ImageHandle     The image handle.
  @param SystemTable     The system table.

  @retval  EFI_SUCEESS  Successfully installed gAmdPciResourceProtocolGuid protocol.
**/
EFI_STATUS
EFIAPI
AmdPcieResourceEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                                      &Handle,
                                      &gAmdPciResourceProtocolGuid,
                                      &AmdPciResources,
                                      NULL
                                      );

  return Status;
}
