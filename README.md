# juwonpee/miniOS.git
My personal 32-bit Operating system project without any specific goals in mind. 

## Requirements
qemu-system-i386 with a pentium instruction set and at least 64M of memory

## To run
```bash
make run
```
Or to run in debug node
```bash
make run_debug
```
## License
[Apache 2.0](https://opensource.org/licenses/Apache-2.0)


## Changes:

### Version 0.0.5-pre_release
Completed PCI device discovery and initialization
Working on AHCI controllers

### Version 0.0.4-pre_release
Fixed many bugs in memory manager
New function in memory manager to directly map virtual and physical memory
Working on PCI initialization functions

### Version 0.0.3-pre_release
Removed "legacy" print statement, now replaced with printf()
New memory manager now supports demand paging and separates between kernel space and user space

### Verision 0.0.2-pre_release
Totally new kernel memory manager
malloc() is smarter albiet a bit slower
1. now respects page boundaries and has a counter to references so it can free unused pages

### Version 0.0.1-pre_release
A bootable kernel with serial interface





