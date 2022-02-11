BINS := wexpression.o wdeclaration.o
TEST_BINS := wexpression.test wdeclaration.test

.PHONY: check clear

%.test: %.o %.test.o ${BINS}
	${CC} -g ${BINS} $@.o -o $@ -lcriterion
	./$@ --xml=$@.xml

%.o: %.c
	${CC} -c $<

all: ${BINS}

check: ${TEST_BINS}

clear:
	rm -vrf *.o *.test
