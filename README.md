<p align="center"><a href="https://www.codechefvit.com" target="_blank"><img src="https://s3.amazonaws.com/codechef_shared/sites/all/themes/abessive/logo-3.png" title="CodeChef-VIT" alt="Codechef-VIT"></a>
</p>

# Simplicity

> <Subtitle>
> The Simplicity microkernel operating system

---
<!-- [![DOCS](https://img.shields.io/badge/Documentation-see%20docs-green?style=flat-square&logo=appveyor)](INSERT_LINK_FOR_DOCS_HERE) 
  [![UI ](https://img.shields.io/badge/User%20Interface-Link%20to%20UI-orange?style=flat-square&logo=appveyor)](INSERT_UI_LINK_HERE) -->

## Features
- ELF higher-half kernel
- Stivale2 boot protocol
- Microkernel architecture
- x64 only

<!-- ## Screenshots
<img src="https://github.com/akshatvg/common-entry-test/raw/master/static/img/header.png" alt="Project Screenshots"> -->

## Instructions to run
First, clone limine in the `external/` directory (if not already present)
```
$ cd external/
$ git clone https://github.com/limine-bootloader/limine
$ cd ..
```
Then, build limine
```
$ make -C external/limine
```
Next, if not using CLion, create the `cmake-build-debug` directory to store the built kernel
```
$ mkdir cmake-build-debug
```
Run CMake inside `cmake-build-debug`
```
$ cd cmake-build-debug
$ cmake ..
$ make
$ cd ..
```
You should now have a built kernel. Next, create the iso image
```
$ ./scripts/make-image.sh
```
The `start-qemu.sh` relies on `qemu-system-x64_64` being installed
```
$ ./scripts/start-qemu.sh
```
This will start `qemu` paused initially, waiting for a debugger (gdb) to connect. The included `.gdbinit` file should
automatically connect `gdb` to `qemu`.

## Contributors
- <a href="https://github.com/dc03">Dhruv Chawla</a>

## License
[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

<p align="center">
	With :heart: by <a href="https://www.codechefvit.com" target="_blank">CodeChef-VIT</a>
</p>
