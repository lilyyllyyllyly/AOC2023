COMP=clang
OPTIONS=-Wall -Wextra -Werror -Wno-unused-but-set-parameter -O3
DEBUG=-fsanitize=address,undefined -g3

SEEDS=seeds.c
RANGES=ranges.c
SEEDS_OUTPUT=seeds
RANGES_OUTPUT=ranges

final:
	${COMP} ${OPTIONS} ${SEEDS} -o ${SEEDS_OUTPUT}
	${COMP} ${OPTIONS} ${RANGES} -o ${RANGES_OUTPUT}

debug:
	${COMP} ${OPTIONS} ${DEBUG} ${SEEDS} -o ${SEEDS_OUTPUT}
	${COMP} ${OPTIONS} ${DEBUG} ${RANGES} -o ${RANGES_OUTPUT}
