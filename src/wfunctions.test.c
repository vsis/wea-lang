#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <criterion/criterion.h>

#include "wexpression.h"
#include "wfunctions.h"

Test(wfunctions, should_add_two_integers) {
  werror_t error;
  char *five = weval_plus_operator_int("2", "3", &error);
  cr_assert_str_eq(five, "5");
  cr_assert(error == WOK);
  free(five);
}

Test(wfunctions, shoud_return_error_when_sum_not_a_number) {
  werror_t error;
  char *will_be_null = weval_plus_operator_int("2", "3asd", &error);
  cr_assert(!will_be_null);
  cr_assert(error == WERROR_NOT_A_NUMBER);
}
