# PixieSDK

[![Build Status](https://github.com/xiallc/pixie_sdk/actions/workflows/cmake.yml/badge.svg)](https://github.com/xiallc/pixie_sdk/actions)
[![Latest Release](https://img.shields.io/github/v/release/xiallc/pixie_sdk)](https://github.com/xiallc/pixie_sdk/releases/latest)

The PixieSDK is a fully featured, cross-platform, C++ based software development kit for XIA LLC's
Pixie-16 product line. We've tried to provide all the tools necessary to get a fresh system
up-and-running without the user writing a single line of code.

## Documentation

The [project documentation is available on the web](https://docs.pixie16.xia.com).

## API Update Policy

By necessity, we will be making changes to the public API calls and the SDK. We will adopt a process
of depreciating interfaces then removing them. We will provide notice of

* no less than 12 months before removing an interface,
* and no less than 6 months before changing an interface.

Notices will appear in the source code, documentation, and release notes. We will notify users via
email 30 days prior to the deprecation date.

Deprecated interfaces will receive bug fixes during their deprecation phase, but will receive no
added functionality. This policy may not cover all situations, and will evolve over time. For
situations that don’t fit neatly into this policy, we will ensure that the information is available
in all relevant Release Notes.

## Tested Systems

| Operating System | CMake Version | Compiler | Compiler Version |
|------------------|---------------|----------|------------------|
| Windows 11       | 3.25.2        | MSVC     | 14.35.32215      |
| Windows 10       | 3.20.2        | MSVC     | 14.28.29333      |
| Debian 11        | 3.18.4        | GCC      | 10.2.1           |
| Debian 10        | 3.13.4        | GCC      | 8.3.0            |

End of life for Windows 10 (Oct 14 2025)
End of life for Debian 10 (Jun 30 2024)

Ubuntu should work as well as it is a Debian variant, but we can not guarantee this.

## Dependencies and Prerequisites

* Compiler supporting the C++ 14 standard
* Cmake 3.10+
* [Broadcom PCI/PCIe SDK 8.00+](https://github.com/xiallc/broadcom_pci_pcie_sdk)
    * You should have `PLX_SDK_DIR` exported in your environment before building.

The [compilation documentation is available on the web](https://docs.pixie16.xia.com/latest/pixie-sdk/installation.html).

## Copyright

Copyright (c) 2005 - 2023, XIA LLC All rights reserved.