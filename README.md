# juwonpee/miniOS.git
My personal 32-bit Operating system project without any specific goals in mind. 

## Requirements
qemu-system-i386 with pentium instruction set and at least 64M of memory
## To run
```bash
make run
```
## License
[Apache 2.0](https://opensource.org/licenses/Apache-2.0)'


## Changes:
### Verision 0.0.2-pre_release
Totally new kernel memory manager
malloc() is smarter albiet a bit slower
    now respects page boundaries and has a counter to references so it can free unused pages
### Version 0.0.1-pre_release
A bootable kernel with serial interface





