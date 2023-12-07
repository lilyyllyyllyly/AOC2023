COMP=clang
OPTIONS=-Wall -Wextra -Werror -Wno-unused-but-set-parameter -O3
DEBUG=-fsanitize=address,undefined -g3

POINTS=points.c
CARDS=cards.c
POINTS_OUTPUT=points
CARDS_OUTPUT=cards

final:
	${COMP} ${OPTIONS} ${CARDS} -o ${CARDS_OUTPUT}
	${COMP} ${OPTIONS} ${POINTS} -o ${POINTS_OUTPUT}

debug:
	${COMP} ${OPTIONS} ${DEBUG} ${CARDS} -o ${CARDS_OUTPUT}
	${COMP} ${OPTIONS} ${DEBUG} ${POINTS} -o ${POINTS_OUTPUT}
