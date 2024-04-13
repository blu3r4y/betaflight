# Guideline for development on Windows with VS Code

## Prerequisites

### Software

Install the following software system-wide:

- [MSYS2](https://www.msys2.org/#installation) to have UNIX build tools available
- [STM32 ST-LINK Utilty V2](https://www.st.com/en/development-tools/stsw-link004.html) to flash the STM32

There is no need to modify any `PATH` environment variables.
After installation the following executables should be available at these locations:

- `C:/msys64/usr/bin/bash.exe`
- `C:/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI.exe`

### VS Code Extensions

We highly recommend to install the following extensions:

- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) for C/C++ IntelliSense
- [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) for STM32 debugging
- [Tasks](https://marketplace.visualstudio.com/items?itemName=actboy168.tasks) to run build tasks from the status bar

## Setup

Open the **MSYS2 MSYS** terminal and install a few packages:

    pacman -S make git unzip

In the same terminal, navigate to this repository and let `make` setup the ARM toolcahin:

    cd /path/to/this/repo
    make arm_sdk_install

Download the latest [xPack OpenOCD](https://github.com/xpack-dev-tools/openocd-xpack/releases) for Windows.
You should download a file similar to `xpack-openocd-0.12.0-3-win32-x64.zip`.
Extract the contents of the ZIP file to `./tools` so that the `bin` directory is at `./tools/xpack-openocd-0.12.0-3/bin`.
You may also unpack the OpenOCD archive to a different location.

Now open the user settings in VS Code and add the following entry.
Make sure to adjust the path to the OpenOCD binary according to your installation.
You may also configure the workspace settings instead.

```json
{
    "cortex-debug.openocdPath": "C:/path/to/where/you/unpacked/tools/xpack-openocd-0.12.0-3/bin/openocd.exe"
}
```	

You are ready to go. Try the build and launch tasks now.

### Adding the MSYS2 terminal to VS Code

To open the MSYS2 terminal in VS Code, add the following entry to your workspace or user settings.
You can skip the `terminal.integrated.defaultProfile.windows` line if you don't want to make it the default terminal.

```json
{
    "terminal.integrated.profiles.windows": {
        "MSYS2 MSYS": {
            "path": "cmd.exe",
            "args": [
                "/c",
                "C:\\msys64\\msys2_shell.cmd -defterm -here -no-start -msys"
            ]
        }
    }
}
```
