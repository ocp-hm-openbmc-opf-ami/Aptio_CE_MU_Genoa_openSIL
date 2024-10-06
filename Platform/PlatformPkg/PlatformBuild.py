# @file
# Script to Build AMD Genoa platform
#
# Copyright (c) Microsoft Corporation.
# Copyright (c) 2024, American Megatrends International LLC. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

import logging
import os
import sys
from typing import Tuple
import shutil
import io

from edk2toolext.environment.uefi_build import UefiBuilder
from edk2toolext.invocables.edk2_platform_build import BuildSettingsManager
from edk2toolext.invocables.edk2_pr_eval import PrEvalSettingsManager
from edk2toolext.invocables.edk2_setup import (RequiredSubmodule,
                                               SetupSettingsManager)
from edk2toolext.invocables.edk2_update import UpdateSettingsManager
from edk2toolext.invocables.edk2_parse import ParseSettingsManager
from edk2toollib.utility_functions import GetHostInfo, RunCmd

LOG_DBG  = 0x1
LOG_INFO = 0x2
LOG_WARN = 0x4
LOG_ERR  = 0x8

    # ####################################################################################### #
    #                                Common Configuration                                     #
    # ####################################################################################### #
class CommonPlatform():
    ''' Common settings for this platform.  Define static data here and use
        for the different parts of stuart
    '''
    PLATFORM_NAME = "Onyx"
    PackagesSupported = ("PlatformPkg",)
    ArchSupported = ("IA32", "X64")
    TargetsSupported = ("DEBUG", "RELEASE")
    Scopes = (PLATFORM_NAME, 'configdata', 'edk2-build', 'cibuild')
    WorkspaceRoot = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
    MuPackagesPath = ("Platform",
                    "MU_BASECORE",
                    "Common/MU_TIANO",
                    "Common/intel_min_platform",
                    "Common/MU",
                    "Common/mu_feature_ipmi",
                    "Common/mu_feature_config",
                    "Common/mu_oem_sample",
                    "Silicon/Amd",
                    "Silicon/Amd/AmdOpenSilPkg/opensil-uefi-interface",
                    "Silicon/Amd/AmdOpenSilPkg/opensil-uefi-interface/OpenSIL",
                    "Silicon/Amd/PSPBin",
                    "Platform/AGCL-R",
                    "Platform/Amd",
                    "Platform/Amd/GenoaOpenBoardPkg"
                    )

    @staticmethod
    def add_common_command_line_options(parserObj) -> None:
        """Adds command line options common to settings managers."""
        parserObj.add_argument('--codeql', dest='codeql', action='store_true', default=False,
            help="Optional - Produces CodeQL results from the build. See "
                 "MU_BASECORE/.pytool/Plugin/CodeQL/Readme.md for more information.")

    @staticmethod
    def retrieve_common_command_line_options(args) -> bool:
        """Retrieves command line options common to settings managers."""
        return args.codeql

    @staticmethod
    def get_active_scopes(codeql_enabled: bool) -> Tuple[str]:
        """Returns the active scopes for the platform."""
        active_scopes = CommonPlatform.Scopes

        # Enable the CodeQL plugin if chosen on command line
        if codeql_enabled:
            if GetHostInfo().os == "Linux":
                active_scopes += ("codeql-linux-ext-dep",)
            else:
                active_scopes += ("codeql-windows-ext-dep",)
            active_scopes += ("codeql-build", "codeql-analyze")

        return active_scopes

    # ####################################################################################### #
    #                         Configuration for Update & Setup                                #
    # ####################################################################################### #
class SettingsManager(UpdateSettingsManager, SetupSettingsManager, PrEvalSettingsManager, ParseSettingsManager):
    def AddCommandLineOptions(self, parserObj):
        """Add command line options to the argparser"""
        CommonPlatform.add_common_command_line_options(parserObj)

    def RetrieveCommandLineOptions(self, args):
        """Retrieve command line options from the argparser"""
        self.codeql = CommonPlatform.retrieve_common_command_line_options(args)

    def GetPackagesSupported(self):
        ''' return iterable of edk2 packages supported by this build.
        These should be edk2 workspace relative paths '''
        return CommonPlatform.PackagesSupported

    def GetArchitecturesSupported(self):
        ''' return iterable of edk2 architectures supported by this build '''
        return CommonPlatform.ArchSupported

    def GetTargetsSupported(self):
        ''' return iterable of edk2 target tags supported by this build '''
        return CommonPlatform.TargetsSupported

    def GetRequiredSubmodules(self):
        ''' return iterable containing RequiredSubmodule objects.
        If no RequiredSubmodules return an empty iterable
        '''
        rs = []

        # To avoid maintenance of this file for every new submodule
        # lets just parse the .gitmodules and add each if not already in list.
        # The GetRequiredSubmodules is designed to allow a build to optimize
        # the desired submodules but it isn't necessary for this repository.
        result = io.StringIO()
        ret = RunCmd("git", "config --file .gitmodules --get-regexp path",
                     workingdir=self.GetWorkspaceRoot(), outstream=result)
        # Cmd output is expected to look like:
        # submodule.CryptoPkg/Library/OpensslLib/openssl.path CryptoPkg/Library/OpensslLib/openssl
        # submodule.SoftFloat.path ArmPkg/Library/ArmSoftFloatLib/berkeley-softfloat-3
        if ret == 0:
            for line in result.getvalue().splitlines():
                _, _, path = line.partition(" ")
                if path is not None:
                    if path not in [x.path for x in rs]:
                        # add it with recursive since we don't know
                        rs.append(RequiredSubmodule(path, True))
        return rs

    def SetArchitectures(self, list_of_requested_architectures):
        ''' Confirm the requests architecture list is valid and configure SettingsManager
        to run only the requested architectures.

        Raise Exception if a list_of_requested_architectures is not supported
        '''
        unsupported = set(list_of_requested_architectures) - \
            set(self.GetArchitecturesSupported())
        if(len(unsupported) > 0):
            errorString = (
                "Unsupported Architecture Requested: " + " ".join(unsupported))
            logging.critical( errorString )
            raise Exception( errorString )
        self.ActualArchitectures = list_of_requested_architectures

    def GetWorkspaceRoot(self):
        ''' get WorkspacePath '''
        return CommonPlatform.WorkspaceRoot

    def GetActiveScopes(self):
        ''' return tuple containing scopes that should be active for this process '''
        return CommonPlatform.get_active_scopes(self.codeql)

    def FilterPackagesToTest(self, changedFilesList: list, potentialPackagesList: list) -> list:
        ''' Filter other cases that this package should be built
        based on changed files. This should cover things that can't
        be detected as dependencies. '''

        # Force a full rebuild for any change
        return CommonPlatform.PackagesSupported

    def GetName(self):
        return CommonPlatform.PLATFORM_NAME + "Main"

    def GetPackagesPath(self):
        ''' Return a list of paths that should be mapped as edk2 PackagesPath '''
        return CommonPlatform.MuPackagesPath

    # ####################################################################################### #
    #                         Actual Configuration for Platform Build                         #
    # ####################################################################################### #
class PlatformBuilder( UefiBuilder, BuildSettingsManager):
    def __init__(self):
        UefiBuilder.__init__(self)

    def AddCommandLineOptions(self, parserObj):
        ''' Add command line options to the argparser '''

        parserObj.add_argument('-a', "--arch", dest="build_arch", type=str, default="IA32,X64",
            help="Optional - CSV of architecture to build.  IA32,X64 will use IA32 for PEI and "
            "X64 for DXE and is the only valid option for this platform.")

        CommonPlatform.add_common_command_line_options(parserObj)

    def RetrieveCommandLineOptions(self, args):
        '''  Retrieve command line options from the argparser '''

        if args.build_arch.upper() != "IA32,X64":
            raise Exception("Invalid Arch Specified.  Please see comments in PlatformBuild.py::PlatformBuilder::AddCommandLineOptions")

        self.codeql = CommonPlatform.retrieve_common_command_line_options(args)

        if self.Clean:
            logging.info("Removing all temporary files")
            for file in os.listdir(self.GetWorkspaceRoot()):
                if file.startswith("spi_image_"):
                    logging.info("    Cleaning " + file)
                    os.remove(file)
                if file.startswith("bios_region_"):
                    logging.info("    Cleaning " + file)
                    os.remove(file)
        return

    def GetWorkspaceRoot(self):
        ''' get WorkspacePath '''
        return CommonPlatform.WorkspaceRoot

    def GetPackagesPath(self):
        ''' Return a list of workspace relative paths that should be mapped as edk2 PackagesPath '''
        return CommonPlatform.MuPackagesPath

    def GetActiveScopes(self):
        ''' return tuple containing scopes that should be active for this process '''
        return CommonPlatform.get_active_scopes(self.codeql)

    def GetName(self):
        ''' Get the name of the repo, platform, or product being build '''
        ''' Used for naming the log file, among others '''
        return CommonPlatform.PLATFORM_NAME + "Main"

    def GetLoggingLevel(self, loggerType):
        ''' Get the logging level for a given type
        base == lowest logging level supported
        con  == Screen logging
        txt  == plain text file logging
        md   == markdown file logging
        '''
        return logging.INFO

    def SetPlatformEnv(self):
        logging.debug("PlatformBuilder SetPlatformEnv")

        self.env.SetValue("ACTIVE_PLATFORM", "PlatformPkg/PlatformPkg.dsc", "Platform Hardcoded")
        self.env.SetValue("TARGET_ARCH", "IA32 X64", "Platform Hardcoded")
        self.env.SetValue("RP_PKG", 'OnyxBoardPkg', "Platform Hardcoded")

        #Set WoringSpace until its env variables set by WorkspaceRoot + PackagesPath
        self.ws = CommonPlatform.WorkspaceRoot

        self.env.SetValue("BOARD_PKG", os.path.basename(os.path.dirname(os.path.abspath(__file__))), "Multiple Boards")
        self.env.SetValue("BLD_*_" + 'BOARD_PKG', self.env.GetValue("BOARD_PKG"), "Multiple Boards")
        self.env.SetValue('SIL_PLATFORM_NAME', "Onyx", "Platform Hardcoded")
        self.env.SetValue("BLD_*_" + 'SIL_PLATFORM_NAME', self.env.GetValue("SIL_PLATFORM_NAME"), "Multiple Boards")
        os.environ['SIL_PLATFORM_NAME'] = self.env.GetValue("SIL_PLATFORM_NAME")
        self.env.SetValue("SOC_SKU", "Genoa", "Platform Hardcoded")
        self.env.SetValue("BLD_*_" + 'SOC_SKU', self.env.GetValue("SOC_SKU"), "Multiple Boards")

        #ensure that path to AutoGen exists
        auto_gen_path = os.path.join( os.path.dirname(os.path.abspath(__file__)), "Include", "AutoGen")
        if not os.path.exists(auto_gen_path):
            os.makedirs(auto_gen_path)

        return 0

    def SetBasicDefaults(self):
       self.setup_structure_pcd_path = os.path.join(self.GetWorkspaceRoot(), "Build", "GenoaOpenBoardPkg", 'SetupDefaultValue.dsc')

       # Create one dummy SetupDefaultValue.dsc file for build
       Rp_pkg_build_path = os.path.join(self.GetWorkspaceRoot(), "Build", "GenoaOpenBoardPkg")
       if not os.path.exists(Rp_pkg_build_path):
          os.mkdir(Rp_pkg_build_path)

       with open(self.setup_structure_pcd_path, 'w') as df:
          df.writelines([])

       return 0

    def get_work_space_path(self):
        return self.GetWorkspaceRoot()

    def get_package_path(self):
        return self.GetWorkspaceRoot()
 
    def get_tc_map(self, tc_short):
        return self.env.GetValue("TOOL_CHAIN_TAG")

    def get_abs_path (self, vars, name):
       if name == 'PLAT_PKG_DSC_PATH':
          logging.info(self.pathobj.GetAbsolutePathOnThisSystemFromEdk2RelativePath(self.env.GetValue("ACTIVE_PLATFORM")))
          return self.pathobj.GetAbsolutePathOnThisSystemFromEdk2RelativePath(self.env.GetValue("ACTIVE_PLATFORM"))
       return ""

    def log(self, level, log):
        if level == LOG_DBG:
            logging.debug(log)
        elif level == LOG_INFO:
            logging.info(log)
        elif level == LOG_WARN:
            logging.warning(log)
        elif level == LOG_ERR:
            logging.error(log)

    def execute_cmd(self,cmd, work_dir=None, bypass_error=False):
        ret = RunCmd("", cmd)
        return ret

    def PlatformPreBuild(self):

        self.env.SetValue("BLD_*_" + 'MAX_SOCKET', "2", "Platform Hardcoded")
        self.env.SetValue("BLD_*_" + 'MAX_CORE', "256", "Platform Hardcoded")
        self.env.SetValue("BLD_*_" + 'MAX_THREAD', "2", "Platform Hardcoded")
        self.env.SetValue("BLD_*_" + 'PEI_ARCH', "IA32", "Platform Hardcoded")
        self.env.SetValue("BLD_*_" + 'BUILD_TARGET', self.env.GetValue("TARGET"), "CommandLine")
        self.env.SetValue("BLD_*_" + 'RP_PKG', "OnyxBoardPkg", "Platform Hardcoded")
        self.env.SetValue("BLD_*_" + 'SOURCE_BASE', "MU", "Source Base")
        
        ## Enable a Build Report to be generatd for the Platform Build
        self.env.SetValue("FLASH_DEFINITION", "PlatformPkg/PlatformPkg.fdf", "Platform Hardcoded")
        self.env.SetValue("PRODUCT_NAME", CommonPlatform.PLATFORM_NAME, "Platform Hardcoded")

        # Copy required files
        common_override_files = {
                         'AgesaPublicModulePkg.dec' :  'Platform/AGCL-R/AgesaModulePkg',
                         'FchSmmDispatcher.c' : 'Platform/AGCL-R/AgesaModulePkg/Fch/9004/Fch9004SmmDispatcher',
                         'PciExpressPciCfg2.inf' : 'Platform/AGCL-R/AgesaPkg/Addendum/PciSegments/PciExpressPciCfg2',
                         'AmdDirectoryBaseLib.inf' : 'Platform/AGCL-R/AgesaModulePkg/Library/AmdDirectoryBaseLib',
                         'AmdMemoryHobInfoPeimGenoa.c' : 'Platform/AGCL-R/AgesaModulePkg/Mem/AmdMemoryHobInfoPeimGenoa',
                         'AmdPspDirectory.h' : 'Platform/AGCL-R/AgesaPkg/Include',
                         'BaseXApicX2ApicLib.c' : 'Platform/Amd/AmdCommonPkg/Library/BaseXApicX2ApicLib',
                         'BaseXApicX2ApicLib.inf' : 'Platform/Amd/AmdCommonPkg/Library/BaseXApicX2ApicLib',
                         'Ivrs.h' : 'Platform/Amd/AmdCommonPkg/Acpi/AcpiCommon',
                         'Ivrs.c' : 'Platform/Amd/AmdCommonPkg/Acpi/AcpiCommon',
                         'AcpiCommon.inf' : 'Platform/Amd/AmdCommonPkg/Acpi/AcpiCommon',
                         'AcpiCommon.c' : 'Platform/Amd/AmdCommonPkg/Acpi/AcpiCommon',
                         'AcpiCommon.h' : 'Platform/Amd/AmdCommonPkg/Acpi/AcpiCommon',
                         'pspdirectory.py' : 'Platform/Amd/PlatformTools/support',
                         'SilOnyxPei.inf' : 'Silicon/Amd/AmdOpenSilPkg/opensil-uefi-interface/Platform/Onyx-Genoa/Pei',
                         'NbioDataInit.c' : 'Silicon/Amd/AmdOpenSilPkg/opensil-uefi-interface/Platform/Onyx-Genoa/Pei',
                         'FchDataInit.c' : 'Silicon/Amd/AmdOpenSilPkg/opensil-uefi-interface/Platform/Onyx-Genoa/Pei',
                         'PciSegmentInfoLibSimple.c' : 'Common/intel_min_platform/MinPlatformPkg/Pci/Library/PciSegmentInfoLibSimple',
                         'FchSmmSwDispatcher.c' : 'Platform/AGCL-R/AgesaModulePkg/Fch/9004/Fch9004SmmDispatcher',
                         'AmdSpiHcSmm.c' : 'Platform/Amd/AmdCommonPkg/Spi/AmdSpiHc',
                         'AmdPlatformPei.c' : 'Platform/Amd/GenoaOpenBoardPkg/Pei',
                         'SilOnyxDxe.inf' : 'Silicon/Amd/AmdOpenSilPkg/opensil-uefi-interface/Platform/Onyx-Genoa/Dxe',
                         'SilDataInit.c' : 'Silicon/Amd/AmdOpenSilPkg/opensil-uefi-interface/Platform/Onyx-Genoa/Dxe',
                         'FchDataInitDxe.c' : 'Silicon/Amd/AmdOpenSilPkg/opensil-uefi-interface/Platform/Onyx-Genoa/Dxe',
                         'xPrfRas.c' : 'Silicon/Amd/AmdOpenSilPkg/opensil-uefi-interface/OpenSIL/xPRF/RAS',
                         'SecureBootVariableProvisionLib.c' : 'Common/MU_TIANO/SecurityPkg/Library/SecureBootVariableProvisionLib',
                         'SecureBootVariableProvisionLib.inf' : 'Common/MU_TIANO/SecurityPkg/Library/SecureBootVariableProvisionLib',
                         'SpiHcAdditional.h' : 'Platform/Amd/AmdCommonPkg/Include/Protocol',
                         'SpiIoAdditional.h' : 'Platform/Amd/AmdCommonPkg/Include/Protocol',
                         'BoardSpiBusDxe.c' : 'Platform/Amd/AmdCommonPkg/Spi/BoardSpiBus',
                         'BoardSpiBusSmm.c' : 'Platform/Amd/AmdCommonPkg/Spi/BoardSpiBus',
                         'SmmRelocationLib.c' : 'MU_BASECORE/UefiCpuPkg/Library/SmmRelocationLib',
                         'AmdSmramSaveStateConfig.c' : 'MU_BASECORE/UefiCpuPkg/Library/SmmRelocationLib',
                         'SmiEntry.nasm' : 'MU_BASECORE/UefiCpuPkg/PiSmmCpuDxeSmm/X64',
                         'PeiBoardInitPreMemLib.inf' : 'Platform/Amd/AmdBoardPkg/Library/BoardInitLib',
                         'MemoryInitPei.c' : 'Platform/Amd/AmdBoardPkg/Library/BoardInitLib',
                         'PeiBoardInitPreMemLib.c' : 'Platform/Amd/AmdBoardPkg/Library/BoardInitLib',
                         'MpioDataInitOnyx.c' : 'Platform/AGCL-R/AgesaPkg/Addendum/Oem/Onyx/Pei',
                         'AcpiPlatform.c' : 'Platform/Amd/AmdCommonPkg/Acpi/AcpiTables',
                         'PciSsdt.c' : 'Platform/Amd/AmdCommonPkg/Acpi/AcpiCommon'
                        }
        
        mu_specific_override_files = {
                         'PlatformInitPreMem.c' : 'Common/intel_min_platform/MinPlatformPkg/PlatformInit/PlatformInitPei',
                         'PlatformInitPreMem.inf' : 'Common/intel_min_platform/MinPlatformPkg/PlatformInit/PlatformInitPei',
                         'SecBoardInitLibNull.inf' : 'Common/intel_min_platform/MinPlatformPkg/PlatformInit/Library/SecBoardInitLibNull',
                         'MemoryAllocationLib.c' : 'Common/MU_TIANO/EmbeddedPkg/Library/PrePiMemoryAllocationLib'
                        }
        
        src_dir = os.path.join(self.GetWorkspaceRoot(), 'Platform/Override')

        # Append mu_specific_override_files for MU build
        file_list = [common_override_files]
        file_list.append(mu_specific_override_files)

        for files_to_copy in file_list:
            for key in files_to_copy:
                src = os.path.join(src_dir, key)
                dst = os.path.join(self.GetWorkspaceRoot(), files_to_copy[key])
                shutil.copy(src, dst)

        self.env.SetValue("BUILDREPORTING", "FALSE", "Platform Hardcoded")
        self.env.SetValue("BUILDREPORT_TYPES", "DEPEX FLASH BUILD_FLAGS LIBRARY PCD", "Platform Hardcoded")
        self.env.SetValue("BUILDREPORT_FILE", CommonPlatform.PLATFORM_NAME + "_BuildReport.txt", "Platform Hardcoded")
    
        self.env.SetValue("PE_VALIDATION_PATH", os.path.join(self.GetWorkspaceRoot(), "Platform/PlatformPkg/CfgData/image_validation.cfg"), "Platform Hardcoded")

        self.env.SetValue("CONF_AUTOGEN_INCLUDE_PATH", self.edk2path.GetAbsolutePathOnThisSystemFromEdk2RelativePath("Platform", "PlatformPkg", "Include", "AutoGen"), "Platform Defined")
        self.env.SetValue('MU_SCHEMA_DIR', self.edk2path.GetAbsolutePathOnThisSystemFromEdk2RelativePath("PlatformPkg", "CfgData", "XML"), "Platform Defined")
        self.env.SetValue('MU_SCHEMA_FILE_NAME', "PlatformConfigData.xml", "Platform Hardcoded")

        return 0

    def PlatformPostBuild(self):
        if (self.env.GetValue("ACTIVE_PLATFORM") == "PlatformPkg/PlatformPkg.dsc"):
          
            output_dir = os.path.join(self.GetWorkspaceRoot(), "Build", self.env.GetValue("RP_PKG"), self.env.GetValue("TARGET") + '_' + self.env.GetValue("TOOL_CHAIN_TAG"))
            searchpath = os.path.join(self.GetWorkspaceRoot(), 'Platform/Amd/PlatformTools/support')
            sys.path.insert(0, searchpath)

            os.environ["SOC_SKU"] = self.env.GetValue("SOC_SKU")
            os.environ['FIRMWARE_VERSION_STR'] = 'MU_CE_' + CommonPlatform.PLATFORM_NAME + self.env.GetValue("SOC_SKU")
            os.environ['AMD_PLATFORM_DIR'] = os.path.join(self.GetWorkspaceRoot(), "Platform/Amd/GenoaOpenBoardPkg/OnyxBoardPkg")
            os.environ['AMD_COMMON_PLATFORM_DIR'] = os.path.join(self.GetWorkspaceRoot(), "Platform/Amd/GenoaOpenBoardPkg")
            os.environ['BUILD_OUTPUT'] = output_dir
            os.environ['CUSTOM_APCB_PATH'] = os.path.join(self.GetWorkspaceRoot(), 'Platform/AGCL-R/AgesaModulePkg/Firmwares/Genoa')
            os.environ['CUSTOM_FIRMWARE_PATH'] = os.path.join(self.GetWorkspaceRoot(), 'Silicon/Amd/PSPBin/Firmwares')
            os.environ['AMD_PLATFORM_TOOLS'] = os.path.join(self.GetWorkspaceRoot(), "Platform/Amd/PlatformTools")
            os.environ['AMD_AGCL_PATH'] = os.path.join(self.GetWorkspaceRoot(), "Platform/AGCL-R")
            os.environ["SOCKET"] = "SP5"

            from pspdirectory import insert_psp_directory
            from call_bios_tar import tar_bios_image

            # Execute PSP insert after APCB
            insert_psp_directory()
            # tar the BIOS FD image
            tar_bios_image()
          
        return 0

    def Build(self):
        ''' Run the build.  This platform needs to run some pre build steps that use the defined compiler.  The path to the compiler
        other tools are not defined until the prebuild plugins run.  To leverage that this platform overrides the Build step,
        does it pre build steps that require compiler, and then calls the super UefiBuilder object Build command.  '''

        return UefiBuilder.Build(self)

if __name__ == "__main__":
    import argparse
    import sys
    from edk2toolext.invocables.edk2_update import Edk2Update
    from edk2toolext.invocables.edk2_setup import Edk2PlatformSetup
    from edk2toolext.invocables.edk2_platform_build import Edk2PlatformBuild
    print("Invoking Stuart")
    print("     ) _     _")
    print("    ( (^)-~-(^)")
    print("__,-.\_( 0 0 )__,-.___")
    print("  'W'   \   /   'W'")
    print("         >o<")
    SCRIPT_PATH = os.path.relpath(__file__)
    parser = argparse.ArgumentParser(add_help=False)
    parse_group = parser.add_mutually_exclusive_group()
    parse_group.add_argument("--update", "--UPDATE",
                             action='store_true', help="Invokes stuart_update")
    parse_group.add_argument("--setup", "--SETUP",
                             action='store_true', help="Invokes stuart_setup")
    args, remaining = parser.parse_known_args()
    new_args = ["stuart", "-c", SCRIPT_PATH]
    new_args = new_args + remaining
    sys.argv = new_args
    if args.setup:
        print("Running stuart_setup -c " + SCRIPT_PATH)
        Edk2PlatformSetup().Invoke()
    elif args.update:
        print("Running stuart_update -c " + SCRIPT_PATH)
        Edk2Update().Invoke()
    else:
        print("Running stuart_build -c " + SCRIPT_PATH)
        Edk2PlatformBuild().Invoke()
