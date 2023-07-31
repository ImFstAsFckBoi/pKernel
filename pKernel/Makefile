CC          = gcc
CFLAGS      = -Wall -Wextra -pedantic -g -no-pie -m32 -Iinclude/pKernel
COBJFLAGS   = $(CFLAGS) -c

AS          = nasm
ASFLAGS     = -f elf32 -g -F dwarf 

OBJS        = pKernel.o debug.o utils.o list.o syscall.o
LIBS        = libpk_context.a libpk_sync.a libpk_time.a
SUB_DIRS	= context sync time

libpKernel.a: ${OBJS} ${LIBS}
	ar rcsT $@ $^

%.o: %.c
	${CC} ${CFLAGS} -c $<

%.o: %.asm
	${AS} ${ASFLAGS} $<

.PHONY: libpk_%.a
libpk_%.a:
	+${MAKE} super -j -C $*

.PHONY: clean
clean:
	rm -f *.a *.o
	for f in ${SUB_DIRS}; do make clean -j -C $$f; done
