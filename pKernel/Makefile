CC          = gcc
CFLAGS      = -Wall -Wextra -pedantic -g -no-pie -m32 -Iinclude/pKernel
COBJFLAGS   = $(CFLAGS) -c

AS          = nasm
ASFLAGS     = -f elf32 -g -F dwarf 

OBJS        = context.o pKernel.o debug.o utils.o sema.o list.o


libpKernel.a: ${OBJS}
	ar rcs $@ ${OBJS}

%.o: %.c
	${CC} ${CFLAGS} -c $<

%.o: %.asm
	${AS} ${ASFLAGS} $<

.PHONY: clean
clean:
	rm -f libpKernel.a *.o

