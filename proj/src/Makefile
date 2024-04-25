# name of the program (Minix service)
PROG = proj

.PATH: ${.CURDIR}/devices
.PATH: ${.CURDIR}/devices/graphics
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/sprites

# source code files to be compiled
SRCS = project.c graphics.c timer.c sprites.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

