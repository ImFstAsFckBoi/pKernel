# pKernel
A coroutine library that only depends on glibc. Might become an actual kernel for an embedded system in the future.

Only works on 32 bit x86 right now. Goal is to be 64 bit, and maybe ARM, compatible at some point.

## Build
```
# Build example and library
make -j

# Build library (libpKernel.a)
make -j -C pKernel/
```
