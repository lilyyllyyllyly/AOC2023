COMP=clang
OPTIONS=-Wall -Wextra -Werror -Wno-unused-but-set-parameter -O3
DEBUG=-fsanitize=address,undefined -g3

GEARS=gears.c
PARTS=parts.c
GEARS_OUTPUT=gears
PARTS_OUTPUT=parts

final:
	${COMP} ${OPTIONS} ${GEARS} -o ${GEARS_OUTPUT}
	${COMP} ${OPTIONS} ${PARTS} -o ${PARTS_OUTPUT}

debug:
	${COMP} ${OPTIONS} ${DEBUG} ${GEARS} -o ${GEARS_OUTPUT}
	${COMP} ${OPTIONS} ${DEBUG} ${PARTS} -o ${PARTS_OUTPUT}
