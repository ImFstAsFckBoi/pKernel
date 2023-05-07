# tool macros
CC          = gcc
CFLAGS      = -Wall -Wextra -pedantic -g -no-pie -m32 -O0
CLIBS		=  -LpKernel/ -lpKernel
CINCLUDES	= -IpKernel/include

AS          = nasm
ASFLAGS     = -f elf32 -g -F dwarf 

OBJS        = main.o


pKernel_test: ${OBJS} libpKernel
	${CC} -o $@ ${OBJS} ${CFLAGS} ${CINCLUDES} ${CLIBS}

%.o: %.c
	${CC} ${CFLAGS} ${CINCLUDES} ${CLIBS} -c $<

.PHONY: clean
clean: clean_lib
	rm -f pKernel_test *.o

.PHONY: clean_lib
clean_lib:
	+${MAKE} clean -j -C pKernel/

.PHONY: libpKernel
libpKernel:
	+${MAKE} -j -C pKernel/

asm: main.c Makefile ${OBJS}
	${CC} -S ${OBJS} ${CFLAGS} ${CINCLUDES} ${CLIBS}

