BINS := wexpression.o wdeclaration.o wfunctions.o reductor.o
TEST_BINS := wexpression.test wdeclaration.test wfunctions.test reductor.test
SPLINT_FLAGS := -memimp -nullret -mustfree -temptrans -branchstate -kepttrans -boolops -globstate -exportlocal

.PHONY: check clear lint

%.test: %.o %.test.o ${BINS}
	${CC} ${CFLAGS} ${BINS} $@.o -o $@ -lcriterion
	./$@ --xml=$@.xml
	gcov $<

%.o: %.c
	${CC} ${CFLAGS} -c $<

all: ${BINS}

check: CFLAGS = -Wall -g -fprofile-arcs -ftest-coverage
check: ${TEST_BINS}
	rm -vrf *test.gcda *test.gcno

lint:
	splint $(SPLINT_FLAGS) $$(ls *c | grep -v test)

clear:
	rm -vrf *.o *.test *.gcov *.xml *gcda *gcno
