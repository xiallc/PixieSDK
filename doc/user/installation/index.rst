Installation
############

These instructions primarily focus on the PixieSDK. We encourage all
users to migrate to the latest version of the library. For convenience,
we provide a backward compatible Pixie16Api library.

We will not be distributing any precompiled binaries for the time being.
This is due to the extreme variability of the compiler / architecture
options for Windows.

There are a number of `CMake Options <#cmake-options>`__ available
when generating the build system files. The defaults will always build
the PixieSDK and the Pixie16Api compatibility library.

Prerequisites
*************

You've already

1. installed the Broadcom PCI/PCIe SDK 8.00 or newer,
2. exported the ``PLX_SDK_DIR`` in your environment,
3. installed CMake 3.10+,
4. the cmake application is available in your path, and
5. installed a compiler that supports C++ 14 standard.

Windows 10
**********

At the moment we can only compile the 32-bit application on Windows.
This is due to an issue determining the appropriate system architecture,
compiler architecture, and variability in Broadcom's installation
process.

Instructions
============

1. Clone or download the project and put the ``pixie_sdk`` directory in
   your ``Downloads`` folder.

2. Open an `x64_x86 Cross Tools Visual Studio Developer Command
   Prompt <https://docs.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell?view=vs-2019>`__

3. Navigate to the source folder

   .. code:: shell

      C:\Program Files (x86)\Microsoft Visual Studio\2019\Community>cd %USERPROFILE%\Downloads\pixie_sdk

4. Create and enter the build directory

   .. code:: shell

      C:Users\username\Downloads\pixie_sdk>mkdir build && cd build

5. Generate the nmake build scripts

   .. code:: shell

      C:Users\username\Downloads\pixie_sdk\build>cmake.exe -G "CodeBlocks - NMake Makefiles" ../

6. Build the project

   .. code:: shell

      C:Users\username\Downloads\pixie_sdk\build>nmake

7. **Optional** Install the project (requires administrative command
   prompt)

   .. code:: shell

      C:Users\username\Downloads\pixie_sdk\build>nmake install

Linux
******

.. _instructions-1:

Instructions
============

These installation instructions assume that you're using git to obtain a
copy of the project. If you're starting with a zip folder from a release
then skip to Step 2.

1. Clone the project

   .. code:: shell

      [user@localhost ~]$ git clone https://github.com/xiallc/pixie_sdk.git

2. Navigate to the cloned project

   .. code:: shell

      [user@localhost ~]$ cd pixie_sdk

3. Configure and build the project

   .. code:: shell

     [user@localhost pixie_sdk]$ mkdir build; cd build; cmake ../

4. Compile the project

   .. code:: shell

     [user@localhost build]$ make

5. (**Optional**) Install the software system-wide. Defaults to ``/usr/local/xia/PixieSDK``.

   .. code:: shell

     [user@localhost build]$ sudo make install

6. (**Optional**) Add the ``XIA_PIXIE_SDK`` environment variable (assumes bash)

   .. code:: shell

     [user@localhost build]$ echo -e "export XIA_PIXIE_SDK=/usr/local/xia/PixieSDK\n" | sudo tee -a /etc/profile.d/xia_pixie_sdk.sh


CMake Options
*************

Primary Options
===============

============== ==================================== =======
Option         Description                          Default
============== ==================================== =======
BUILD_DOCO     Tells CMake to build documentation   OFF
BUILD_EXAMPLES Builds the example programs          ON
BUILD_LEGACY   Builds the legacy C API Pixie16App.a OFF
BUILD_SDK      Builds PixieSdk.a                    ON
BUILD_TESTS    Builds the test suites               OFF
============== ==================================== =======

Dependent Options
=================

+-------------------------+----------------------------------------------------+--------------------------+---------+
| Option                  | Description                                        | Depends on               | Default |
+=========================+====================================================+==========================+=========+
| BUILD_INTEGRATION_TESTS | Builds integration tests                           | BUILD_TESTS; BUILD_SDK   | ON      |
+-------------------------+----------------------------------------------------+--------------------------+---------+
| BUILD_LEGACY_UNIT_TESTS | Builds legacy unit tests                           | BUILD_TESTS;BUILD_LEGACY | ON      |
+-------------------------+----------------------------------------------------+--------------------------+---------+
| BUILD_PIXIE16_API       | Builds backward compatible Pixie16 SDK API Library | BUILD_SDK                | ON      |
+-------------------------+----------------------------------------------------+--------------------------+---------+
| BUILD_SYSTEM_TESTS      | Enables build of system tests                      | BUILD_TESTS;BUILD_SDK    | ON      |
+-------------------------+----------------------------------------------------+--------------------------+---------+
| BUILD _SDK_UNIT_TESTS   | Builds PixieSDK unit tests                         | BUILD_TESTS;BUILD_SDK    | ON      |
+-------------------------+----------------------------------------------------+--------------------------+---------+
| USE_USLEEP              | Adds the USE_USLEEP flag to Legacy builds          | BUILD_LEGACY             | OFF     |
+-------------------------+----------------------------------------------------+--------------------------+---------+
