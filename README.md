# pxml - XML Parser in C

This README provides a comprehensive guide to the project, including its features, installation steps, and usage.

---

**pxml** is a simple command-line tool written in C for parsing XML files and retrieving values based on keys.

## Table of Contents
  - [Introduction](#introduction)
  - [Features](#features)
  - [Updates](#updates)
  - [Dependencies](#dependencies)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
    - [Clone the repository](#clone-the-repository)
    - [Compile the program](#compile-the-program)
    - [Install the program](#install-the-program)
  - [Uninstallation](#uninstallation)
  - [Usage](#usage)
  - [Help](#help)
  - [Files](#files)
  - [Customize](#customize)
  - [Examples](#examples)
    - [Integrating 'pxml' with Bash Scripts](#integrating-pxml-with-bash-scripts)
  - [Advantages](#advantages)
  - [License](#license)

## Introduction

**pxml** is a lightweight and efficient C program for parsing XML files.
It allows retrieval of values based on specific keys and is ideal for configuration management and automation scripts.

![GitHub Release](https://img.shields.io/github/v/release/AntiSecTech/pxml?include_prereleases&display_name=release)
![GitHub License](https://img.shields.io/github/license/AntiSecTech/pxml)
![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/AntiSecTech/pxml/total)

---

## Features

- **Minimal Dependencies:** Only requires the `libxml2` library.
- **High Performance:** Written in C for speed and efficiency.
- **Ease of Use:** Simple command-line interface for easy integration.
- **Parse** XML files.
- **Retrieve nested values** using dot notation for keys.
- **Customizable output** for parsed XML content.

---

## Updates

This release is currently in version: **1.0.0**
Please refer to the `changelog` for all information on current changes.

You can view it [here](https://github.com/AntiSecTech/pxml/blob/main/CHANGELOG.md).

![GitHub Release](https://img.shields.io/github/v/release/AntiSecTech/pxml?include_prereleases&display_name=release)
![GitHub Tag](https://img.shields.io/github/v/tag/AntiSecTech/pxml?include_prereleases&label=build)

---

## Dependencies

The `libxml2` library is required to compile this project.
It can be installed on Linux systems using package managers.

### Linux:

```sh
sudo apt-get update
sudo apt-get install libxml2-dev   # for Debian/Ubuntu
sudo dnf install libxml2-devel     # for Fedora
sudo pacman -S libxml2             # for Arch Linux
```

---

## Prerequisites

To compile the C program, you need to have the `GCC` compiler installed on your system.

### Linux:
```sh
sudo apt-get update
sudo apt-get install gcc   # for Debian/Ubuntu
sudo dnf install gcc       # for Fedora
sudo pacman -S gcc         # for Arch Linux
```

---

## Installation
### Clone the repository:
```sh
git clone https://github.com/AntiSecTech/pxml.git
cd pxml
```
### Compile the program:
```sh
make compile
```
### Install the program:
```sh
sudo make install
```
If you do not install the program and only compile the shared library, you must **create a link for the configuration file** [`pxml.xml`](https://github.com/AntiSecTech/pxml/blob/main/pxml.xml) so that it can be read by the binary.
```sh
sudo ln -s -r pxml.xml /etc
```
If you have any further questions about the `Make` process, please refer to the source code of the [Makefile](https://github.com/AntiSecTech/pxml/blob/main/Makefile).

---

## Uninstallation
```sh
cd pxml
sudo make uninstall
```
---

## Usage
```sh
   pxml <file.xml> [<dot.notated.key>]
```

---

## Help
The following options are available to help you use **pxml**.

**Built-in**
```sh
   pxml --help
```
***Overview***
|Command|Option|Argument|Optional Argument|Description|
|---|---|---|---|---|
| pxml | | | | Show the usage |
| pxml | | <File.xml> | | Show content of <`File.xml`> |
| pxml | | <File.xml> | \<xpath> | Show the value from <`key`> |
| pxml | -h \| --help | | | Show the help |
| pxml | -v | | | Show version number |
| pxml | --version | | | Show detailed version information |


**Manpage**

To open the manual, use the following command:
```sh
   man pxml
```

---

## Files
Here you will find a small overview of the files used and their locations.
|Filename|Filetype|Project Path|Install Path|Description|
|---|---|---|---|---|
| pxml	| shared library | ./pxml | /usr/local/bin | Executable file |
| pxml.c | C source | ./pxml | | Source code |
| pxml.xml | xml | ./pxml | /etc | Configuration |
| pxml.1 | troff | ./pxml | /usr/local/man/man1 | Manpage |

---

## Customize

The program provides customizable output options for parsed XML files. Users can modify the output format by editing the configuration file.

Open the [`pxml.xml`](https://github.com/AntiSecTech/pxml/blob/main/pxml.xml) file located in the same directory as the program.
Locate the **output** field under the **Config** section.
Change its value to your desired output format.
```xml
    <Config>
        <Scheme>default</Scheme>
    </Config>
```
For example, you can change the output format to `monokai`:
```xml
    <Config>
        <Scheme>monokai</Scheme>
    </Config>
```
After editing the configuration file, the new output format will be applied automatically.

---

## Examples
Given the following XML file [`test.xml`](https://github.com/AntiSecTech/pxml/blob/main/test.xml):
```sh
<?xml version="1.0" encoding="UTF-8"?>
<root>
    <Config>
        <scheme>default</scheme>
        <version>2.0.0</version>
    </Config>
    <Data>
        <item1>value1</item1>
        <item2>value2</item2>
        <nested>
            <item3>value3</item3>
        </nested>
    </Data>
</root>
```
Retrieve the following keys:

 - /root/Config/version
 - /root/Data/item1
 - /root/Data/nested/item3
```sh
   pxml test.xml root.Config.version
   pxml test.xml root.Data.item1
   pxml test.xml root.Data.nested.item3
```
it also works without the `root` keyword like:
```sh
   pxml test.xml Config.version
   pxml test.xml Data.item1
   pxml test.xml Data.nested.item3
```
Output:
```sh
   2.0.0
   value1
   value3
```

### Integrating 'pxml' with Bash Scripts
One of the primary advantages of **pxml** is its seamless integration with Bash scripts.
This allows for dynamic reading and setting of configuration parameters directly within your scripts.

**Example Bash Script:**
```sh
   #!/bin/bash

   # Parse XML file and store values in variables
   config_file="config.xml"
   key=$(pxml $config_file Property.Key)

   # Use the parsed values
   echo "Value: $key"
```
```sh
<Property>
  <Key>value</Key>
</Property>
```
Output:
```sh
   Value: value
```

---

## Advantages
 - **Efficiency:** Written in C, pxml provides high-speed parsing capabilities, reducing overhead in scripts.
 - **Simplicity:** With its straightforward command-line interface, pxml can be easily integrated into existing workflows and automation scripts.
 - **Flexibility:** Able to parse any standard XML file, making it versatile for various applications and configurations.

---

## License
This project is licensed under the [`MIT License`](https://github.com/AntiSecTech/pxml/blob/main/LICENSE).
See the [LICENSE](https://raw.githubusercontent.com/AntiSecTech/pxml/main/LICENSE) file for more details.
