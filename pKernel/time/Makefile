CC          = gcc
CFLAGS      = -Wall -Wextra -pedantic -g -no-pie -m32 -I../include/pKernel
COBJFLAGS   = $(CFLAGS) -c

AS          = nasm
ASFLAGS     = -f elf32 -g -F dwarf 

OBJS = time.o _time.o

libpk_time.a: ${OBJS}
	ar rcs $@ $^

%.o: %.c
	${CC} ${COBJFLAGS} $<

%.o: %.asm
	${AS} ${ASFLAGS} $<

super: libpk_time.a
	cp $< ../

.PHONY: clean
clean:
	rm -f libpk_time.a *.o

