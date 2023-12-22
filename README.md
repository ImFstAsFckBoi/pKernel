# pKernel
A coroutine library that only depends on glibc (except some Linux specific debug functions). Might become an actual kernel for an embedded system in the future (but probably never).

Only works on 32 bit x86 right now. Goal is to be 64 bit.

## Build
```
# Build example and library
make -j

# Build library (libpKernel.a)
make -j -C pKernel/
```
