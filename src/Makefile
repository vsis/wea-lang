BINS := wexpression.o wdeclaration.o
TEST_BINS := wexpression.test wdeclaration.test

.PHONY: check clear

%.test: %.o %.test.o ${BINS}
	${CC} ${CFLAGS}  ${BINS} $@.o -o $@ -lcriterion
	./$@ --xml=$@.xml
	gcov $<

%.o: %.c
	${CC} ${CFLAGS} -c $<

all: ${BINS}

check: CFLAGS = -g -fprofile-arcs -ftest-coverage
check: ${TEST_BINS}

clear:
	rm -vrf *.o *.test *.gcov *.xml *gcda *gcno
