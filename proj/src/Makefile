# name of the program (Minix service)
PROG = proj

.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/controller/timer
.PATH: ${.CURDIR}/controller/keyboard
.PATH: ${.CURDIR}/controller/mouse
.PATH: ${.CURDIR}/controller/graphics
.PATH: ${.CURDIR}/controller/rtc
.PATH: ${.CURDIR}/controller/serialport
.PATH: ${.CURDIR}/model
.PATH: ${.CURDIR}/view

# source code files to be compiled
SRCS = project.c utils.c timer.c keyboard.c mouse.c graphics.c model.c sprites.c view.c serialport.c queue.c rtc.c physics.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf -lxpm -llm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

