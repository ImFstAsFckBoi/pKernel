CC          = gcc
CFLAGS      = -O0 -Wall -Wextra -pedantic -g -no-pie -m32 -I../include/pKernel
COBJFLAGS   = $(CFLAGS) -c

AS          = nasm
ASFLAGS     = -f elf32 -g -F dwarf 

OBJS = context.o _context.o



libpk_context.a: ${OBJS}
	ar rcs $@ $^

%.o: %.c
	${CC} ${COBJFLAGS} $<

%.o: %.asm
	${AS} ${ASFLAGS} $<

super: libpk_context.a
	cp $< ../

.PHONY: clean
clean:
	rm -f libpk_context.a *.o

