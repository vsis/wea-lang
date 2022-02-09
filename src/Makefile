BINS := wvariable.o wexpression.o
TEST_BINS := wvariable.test wexpression.test

%.test: %.o %.test.o ${BINS}
	${CC} ${BINS} $@.o -o $@ -lcriterion

%.o: %.c
	${CC} -c $<

ALL: ${OBJS}

tests: ${TEST_BINS}
	@for TEST_F in ${TEST_BINS}; do ./$$TEST_F; done

clear:
	rm -vrf *.o *.test
