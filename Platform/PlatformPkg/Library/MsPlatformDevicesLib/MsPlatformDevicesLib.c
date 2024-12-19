/** @file
 *MsPlatformDevicesLib  - Device specific library.

Copyright (C) Microsoft Corporation. All rights reserved.
Copyright (c) 2024, American Megatrends International LLC. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/MsPlatformDevicesLib.h>
#include <IndustryStandard/Pci.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesLib.h>
#include <Protocol/PciIo.h>

/**
Library function used to provide the platform SD Card device path
**/
EFI_DEVICE_PATH_PROTOCOL *
EFIAPI
GetSdCardDevicePath (
  VOID
  )
{
  return NULL;
}

/**
  Library function used to determine if the DevicePath is a valid bootable 'USB' device.
  USB here indicates the port connection type not the device protocol.
  With TBT or USB4 support PCIe storage devices are valid 'USB' boot options.
**/
BOOLEAN
EFIAPI
PlatformIsDevicePathUsb (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  return FALSE;
}

/**
Library function used to provide the list of platform devices that MUST be
connected at the beginning of BDS
**/
EFI_DEVICE_PATH_PROTOCOL **
EFIAPI
GetPlatformConnectList (
  VOID
  )
{
  return NULL;
}

/**
 * Library function used to provide the list of platform console devices.
 */
BDS_CONSOLE_CONNECT_ENTRY *
EFIAPI
GetPlatformConsoleList (
  VOID
  )
{
  return NULL;
}

/**
Library function used to provide the list of platform devices that MUST be connected
to support ConsoleIn activity.  This call occurs on the ConIn connect event, and
allows platforms to do enable specific devices ConsoleIn support.
**/
EFI_DEVICE_PATH_PROTOCOL **
EFIAPI
GetPlatformConnectOnConInList (
  VOID
  )
{
  return NULL;
}

/*
  Function to Connect VGA Conout
*/
STATIC
EFI_HANDLE
ConnectConOut (
  OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath
  )
{
  EFI_STATUS                Status;
  UINTN                     HandleCount;
  UINTN                     Index;
  EFI_HANDLE                *HandleBuffer;
  EFI_HANDLE                VideoController;
  EFI_PCI_IO_PROTOCOL       *PciIo;
  PCI_TYPE00                Pci;
  EFI_DEVICE_PATH_PROTOCOL  *Gop;

  Status = gBS->LocateHandleBuffer (
                      ByProtocol,
                      &gEfiPciIoProtocolGuid,
                      NULL,
                      &HandleCount,
                      &HandleBuffer
                      );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                          HandleBuffer[Index], 
                          &gEfiPciIoProtocolGuid, 
                          (VOID **)&PciIo
                          );

    if (!EFI_ERROR (Status)) {
      // Check for all video controller
      Status = PciIo->Pci.Read (
                            PciIo,
                            EfiPciIoWidthUint32,
                            0,
                            sizeof (Pci) / sizeof (UINT32),
                            &Pci
                            );
      if (!EFI_ERROR (Status) && IS_PCI_VGA (&Pci)) {
        if ((Pci.Hdr.VendorId == PcdGet16 (PcdOnboardVideoPciVendorId)) &&
            (Pci.Hdr.DeviceId == PcdGet16 (PcdOnboardVideoPciDeviceId)))  {// onboard
          VideoController = HandleBuffer[Index];
        }
      }
    }
  }

  FreePool (HandleBuffer);

  if (VideoController != NULL) {
    Gop = EfiBootManagerGetGopDevicePath (VideoController);
    if (Gop) {
      // Exist Gop, exit
      FreePool (Gop);
      return NULL;
    }

    gBS->ConnectController (
                    VideoController, 
                    NULL, 
                    NULL, 
                    FALSE);

    Gop = EfiBootManagerGetGopDevicePath (VideoController);
    if (Gop != NULL) {
      *DevicePath = Gop;
    }
  }

  return VideoController;
}

/**
Library function used to provide the console type.  For ConType == DisplayPath,
device path is filled in to the exact controller to use.  For other ConTypes, DisplayPath
must NULL. The device path must NOT be freed.
**/
EFI_HANDLE
EFIAPI
GetPlatformPreferredConsole (
  OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath
  )
{
  return ConnectConOut (DevicePath);
}
