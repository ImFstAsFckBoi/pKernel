CC          = gcc
CFLAGS      = -Wall -Wextra -pedantic -g -no-pie -m32 -I../include/pKernel
COBJFLAGS   = $(CFLAGS) -c

AS          = nasm
ASFLAGS     = -f elf32 -g -F dwarf 

OBJS = lock.o rwlock.o sema.o

libpk_sync.a: ${OBJS}
	ar rcs $@ $^

%.o: %.c
	${CC} ${COBJFLAGS} $<

%.o: %.asm
	${AS} ${ASFLAGS} $<

super: libpk_sync.a
	cp $< ../

.PHONY: clean
clean:
	rm -f libpk_sync.a *.o

