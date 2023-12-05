COMP=clang
OPTIONS=-Wall -Wextra -Werror -Wno-unused-but-set-parameter -O3
DEBUG=-fsanitize=address,undefined -g3

SOURCES=cubes.c
OUTPUT=cubes

final:
	${COMP} ${OPTIONS} ${SOURCES} -o ${OUTPUT}

debug:
	${COMP} ${OPTIONS} ${DEBUG} ${SOURCES} -o ${OUTPUT}
