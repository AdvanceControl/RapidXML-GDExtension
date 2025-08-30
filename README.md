## [简体中文](README_zh_CN.md) | [English](README.md)

# RapidXML GDExtension

## What is RapidXML GDExtension?
RapidXML GDExtension is a powerful XML parser created for the Godot Engine.

## Features
- Built upon the high-performance RapidXML C++ library.
- Provides a simple API for parsing XML files.
- Supports querying nodes and attributes via paths (e.g., `root/child`).

## Building from Source
Open a terminal or command prompt, navigate to the project's root directory, and execute the following command:

`scons platform=<platform> target=template_<release_or_debug> arch=<architecture>`

For example, to build for Windows 64-bit release:
`scons platform=windows target=template_release arch=x86_64`

Wait for the build process to complete.

## License
This project is licensed under the MIT License, the same license used by Godot Engine and RapidXML.

## Contributing to RapidXML GDExtension
See [Contributing-to-RapidXML-GDExtension](https://github.com/AdvanceControl/RapidXML-GDExtension/wiki/Contributing-to-RapidXML-GDExtension)