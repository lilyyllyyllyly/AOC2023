COMP=clang
OPTIONS=-Wall -Wextra -Werror -O3
DEBUG=-fsanitize=address,undefined -g3

SOURCES=trebuchet.c spell.c
OUTPUT=trebuchet

final:
	${COMP} ${OPTIONS} ${SOURCES} -o ${OUTPUT}

debug:
	${COMP} ${OPTIONS} ${DEBUG} ${SOURCES} -o ${OUTPUT}
