COMP=clang
OPTIONS=-Wall -Wextra -Werror -Wno-unused-but-set-parameter -O3
DEBUG=-fsanitize=address,undefined -g3

SOURCES=gears.c
OUTPUT=gears

final:
	${COMP} ${OPTIONS} ${SOURCES} -o ${OUTPUT}

debug:
	${COMP} ${OPTIONS} ${DEBUG} ${SOURCES} -o ${OUTPUT}
