
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)
TEST_SRCS := $(wildcard *.test.c)
TEST_BINS := wvariable.test

%.test: %.o %.test.o
	${CC} $@.o $< -o $@ -lcriterion 

%.o: %.c
	${CC} -c $<

ALL: ${OBJS}

tests: ${TEST_BINS}
	@for TEST_F in ${TEST_BINS}; do ./$$TEST_F; done
