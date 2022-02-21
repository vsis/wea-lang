#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <criterion/criterion.h>

#include "wexpression.h"
#include "wfunctions.h"

Test(wfunctions, should_add_two_integers) {
  werror_t error;
  char *five = weval_plus_operator_int("2", "3", &error);
  cr_assert(error == WOK);
  cr_assert_str_eq(five, "5");
  free(five);
}

Test(wfunctions, shoud_return_error_when_sum_not_a_number) {
  werror_t error;
  char *will_be_null = weval_plus_operator_int("2", "3asd", &error);
  cr_assert(!will_be_null);
  cr_assert(error == WERROR_NOT_A_NUMBER);
}

Test(wfunctions, should_return_NULL_for_a_nonexisting_function) {
  wfunction_t *function = wget_function("wea.do_not_exists");
  cr_assert(function == NULL);
}

Test(wfunction, should_get_plus_operator) {
  wfunction_t *function = wget_function("+");
  cr_assert(function != NULL);
  cr_assert(function->wtype == WOPERATOR);
  cr_assert(function->wargc == 2);
}

Test (wfunction, should_not_get_plus_operator_with_garbage) {
  wfunction_t *function = wget_function("+adsf");
  cr_assert(function == NULL);
}
