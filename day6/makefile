COMP=clang
OPTIONS=-Wall -Wextra -Werror -Wno-unused-but-set-parameter -O3
DEBUG=-fsanitize=address,undefined -g3

BOATS=boats.c
RACE=race.c
BOATS_OUTPUT=boats
RACE_OUTPUT=race

final:
	${COMP} ${OPTIONS} ${BOATS} -o ${BOATS_OUTPUT}
	${COMP} ${OPTIONS} ${RACE} -o ${RACE_OUTPUT}

debug:
	${COMP} ${OPTIONS} ${DEBUG} ${BOATS} -o ${BOATS_OUTPUT}
	${COMP} ${OPTIONS} ${DEBUG} ${RACE} -o ${RACE_OUTPUT}
