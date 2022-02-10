BINS := wexpression.o wdeclaration.o
TEST_BINS := wexpression.test wdeclaration.test

%.test: %.o %.test.o ${BINS}
	${CC} ${BINS} $@.o -o $@ -lcriterion

%.o: %.c
	${CC} -c $<

ALL: ${OBJS}

tests: ${TEST_BINS}
	@for TEST_F in ${TEST_BINS}; do ./$$TEST_F; done

clear:
	rm -vrf *.o *.test
