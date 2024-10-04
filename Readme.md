# Source Download

Clone the project using below command.

```
git clone https://github.com/ocp-hm-openbmc-opf-ami/Aptio_CE_MU_Genoa_openSIL.git
```

# Prerequisites to build

## Windows
1. [VS Build Tools](https://aka.ms/vs/17/release/vs_buildtools.exe) - Install below components
   - Windows 10 SDK (Latest)
   - MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)
   - C++ Build Tools core features
   - Windows Universal C Runtime
2. Python 3.10.x

## Ubuntu
1. GCC
2. Python 3.10.x
3. Install Nuget and dependencies:
    ```
    sudo apt-get install mono-complete nuget make
    ```

# Build Steps

1. Open command prompt and traverse to the root of the project

2. Create the virtual environment. Make sure that virtual environment is created outside of the project root folder
   ```
   python -m venv <virtual_env_path>
   ```

3. Activate the virutal environment
   - Windows: 
     ```
     <virtual_env_path>\Scripts\activate
     ```
   - Linux: 
     ```
     source <virtual_env_path>/bin/activate
     ```

4. Install necessary python modules 
   ```
   pip install --upgrade -r pip-requirements.txt
   ```

5. Run stuart_setup to clone the submodules recursively 
   ```
   stuart_setup -c Platform/PlatformPkg/PlatformBuild.py
   ```

6. Run stuart_update to download necessary external dependencies 
   ```
   stuart_update -c Platform/PlatformPkg/PlatformBuild.py
   ```

7. Run stuart_build to build the project
   ```
   stuart_build -c Platform/PlatformPkg/PlatformBuild.py TARGET=[DEBUG|RELEASE] TOOL_CHAIN_TAG=[VS2019|VS2022|GCC]
   ```
8. Final binary (`MU_CE_OnyxGenoa.tar.gz`) will be created in the project root folder after successful build.


# Source Code Details

The followings are the submodules of this project:

- [MU_BASECORE](https://github.com/microsoft/mu_basecore.git) (*tag: v2024050000.0.1*)

- [Common/MU_TIANO](https://github.com/microsoft/mu_tiano_plus.git) (*tag: v2024050000.0.0*)

- [Common/MU](https://github.com/microsoft/mu_plus.git) (*tag: v2024050000.0.0*)

- [Common/mu_feature_ipmi](https://github.com/microsoft/mu_feature_ipmi.git) (*tag: v3.0.6*)

- [Common/mu_feature_config](https://github.com/microsoft/mu_feature_config.git) (*tag: v3.0.0*)

- [Common/mu_oem_sample](https://github.com/microsoft/mu_oem_sample.git) (*tag: v2024050000.0.0*)

- [Common/intel_min_platform](https://github.com/microsoft/mu_common_intel_min_platform.git) (*tag: v2024050000.0.0*)

- AMD Reference Code repos

  - [Silicon/Amd/AmdOpenSilPkg/opensil-uefi-interface](https://github.com/openSIL/opensil-uefi-interface.git) (*branch: genoa_poc*)

  - [Silicon/Amd/PSPBin](https://github.com/openSIL/amd_firmwares.git) (*branch: genoa_poc*)

  - [Platform/Amd](https://github.com/openSIL/EDKII-Platform.git) (*branch: genoa_poc*)

  - [Platform/AGCL-R](https://github.com/openSIL/AGCL-R.git) (*branch: genoa_poc*)

# Validation

- Installed and booted to Ubuntu 22.02 using M2 NVME
- Installed and booted to Windows Server 2022 using M2 NVME

# Features

## SecureBoot

* SecureBoot is disabled by default. Enable the `PcdUefiSecureBootEnable` and provide the path for Secure Boot keys in PlatformPkg.fdf
* Enter Setup &rarr; Device Manager &rarr; Secure Boot Configuration &rarr; Reset Secure Boot Keys &rarr; Reset.

# Additional Support and Customizations
- To get dedicated support or additional features or customizations for Aptio CommunityEdition, feel free to email sales@ami.com
