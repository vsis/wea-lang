BINS := wexpression.o wdeclaration.o wfunctions.o reductor.o
TEST_BINS := wexpression.test wdeclaration.test wfunctions.test reductor.test

.PHONY: check clear

%.test: %.o %.test.o ${BINS}
	${CC} ${CFLAGS}  ${BINS} $@.o -o $@ -lcriterion
	./$@ --xml=$@.xml
	gcov $<

%.o: %.c
	${CC} ${CFLAGS} -c $<

all: ${BINS}

check: CFLAGS = -Wall -g -fprofile-arcs -ftest-coverage
check: ${TEST_BINS}
	rm -vrf *test.gcda *test.gcno

clear:
	rm -vrf *.o *.test *.gcov *.xml *gcda *gcno
