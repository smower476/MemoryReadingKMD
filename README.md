## Overview

MemoryReadingKMD is a Kernel-Mode Driver for reading and modifying information in memory. You can easily bypass all User-Mode and some Kernel-Mode Anti-Cheats, AntiViruses (Probably).

## Demo

https://github.com/smower476/MemoryReadingKMD/assets/121474491/a319e409-b5aa-4c4d-93e2-2ea96fec66be

## Prerequisites

Download and install the Visual Studio 2022 and Windows Driver Kit (WDK) as provided [here](https://learn.microsoft.com/en-gb/windows-hardware/drivers/download-the-wdk).

## Build the sample

You can build this driver in two ways: using the Visual Studio Integrated Development Environment (IDE) or from the command line using the Visual Studio Command Prompt window and the Microsoft Build Engine (MSBuild.exe).

### Building the sample using Visual Studio

1. Open Visual Studio. From the **File** menu, select **Open Project/Solution** and open the MemoryReadingKMD.sln project file.

1. Right-click the solution in the **Solution Explorer** and select **Configuration Manager**.

1. From the **Configuration Manager**, select the **Active Solution Configuration** and the **Active Solution Platform** (for example, Win32) that correspond to the type of build you are interested in.

1. From the **Build** menu, click **Build Solution** (Ctrl+Shift+B).

### Building the sample using the command line (MSBuild)

1. Open a Visual Studio Command Prompt window. Click **Start** and search for **Developer Command Prompt**. If your project is under %PROGRAMFILES%, you need to open the command prompt window using elevated permissions (**Run as administrator**). From this window you can use MsBuild.exe to build any Visual Studio project by specifying the project (.VcxProj) or solutions (.Sln) file.

1. Navigate to each of the respective project directories and enter the appropriate **MSbuild** command for your target. For example, to perform a clean build of a Visual Studio driver project called BthEcho.vcxproj, navigate to the samples\\bluetooth\\bthecho\\bthcli\\sys project directory and enter the following MSBuild command: **msbuild /t:clean /t:build .\\MemoryReadingKMD.vcxproj**.

1. If the build succeeds, you will find the driver (MemoryReadingKMD.sys) and User-mode executable (KernelCsCheatUM.exe) in the binary output directory corresponding to the target platform.

## How to run

1. Open PowerShell in the same directory with files and enter the following command:
```
.\kdmapper.exe .\KernelCsCheatUM.exe
```
2. Run WINMINE.EXE.
3. Run KernelCsCheatUM.exe and follow the instructions in the terminal.
