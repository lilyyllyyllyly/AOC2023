COMP=clang
OPTIONS=-Wall -Wextra -Werror -O3
DEBUG=-fsanitize=address,undefined -g3

SOURCES=spell.c
OUTPUT=spell

final:
	${COMP} ${OPTIONS} ${SOURCES} -o ${OUTPUT}

debug:
	${COMP} ${OPTIONS} ${DEBUG} ${SOURCES} -o ${OUTPUT}
