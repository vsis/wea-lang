#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <criterion/criterion.h>

#include "wexpression.h"
#include "wfunctions.h"

Test(wfunctions, should_add_two_integers) {
  werror_t error;
  char *five = weval_operator_int("+", "2", "3", &error);
  cr_assert(error == WOK);
  cr_assert_str_eq(five, "5");
  free(five);
}

Test(wfunctions, should_return_error_when_sum_first_arg_not_a_number) {
  werror_t error;
  char *will_be_null = weval_operator_int("+", "2", "3asd", &error);
  cr_assert(!will_be_null);
  cr_assert(error == WERROR_NOT_A_NUMBER);
}

Test(wfunctions, should_return_error_when_sum_second_arg_not_a_number) {
  werror_t error;
  char *will_be_null = weval_operator_int("+", "2asdf", "3", &error);
  cr_assert(!will_be_null);
  cr_assert(error == WERROR_NOT_A_NUMBER);
}

Test(wfunctions, should_substract_integers) {
  werror_t error = WERROR;
  char *sub = weval_operator_int("-", "10", "1", &error);
  cr_assert_str_eq(sub, "9");
  cr_assert(error == WOK);
  free(sub);
}

Test(wfunctions, should_substract_big_integers) {
  werror_t error = WERROR;
  char *sub = weval_operator_int("-", "1000000000", "999999999", &error);
  cr_assert_str_eq(sub, "1");
  cr_assert(error == WOK);
  free(sub);
}

Test(wfunctions, should_multiply_integers) {
  werror_t error = WERROR;
  char *mul = weval_operator_int("*", "1000", "5000", &error);
  cr_assert_str_eq(mul, "5000000");
  cr_assert(error == WOK);
  free(mul);
}

Test(wfunctions, should_divide_integers) {
  werror_t error = WERROR;
  char *div = weval_operator_int("/", "15000", "3000", &error);
  cr_assert_str_eq(div, "5");
  cr_assert(error == WOK);
  free(div);
}

Test(wfunctions, should_not_try_to_divide_by_zero) {
  werror_t error = WERROR;
  char *div = weval_operator_int("/", "15000", "0", &error);
  cr_assert(div == NULL);
  cr_assert(error == WERROR_DIVISION_BY_ZERO);
}

Test(wfunctions, should_divide_zero_by_something) {
  werror_t error = WERROR;
  char *div = weval_operator_int("/", "000", "3000", &error);
  cr_assert_str_eq(div, "0");
  cr_assert(error == WOK);
  free(div);
}

Test(wfunctions, should_get_modulo) {
  werror_t error = WERROR;
  char *mod = weval_operator_int("%", "1000", "3", &error);
  cr_assert_str_eq(mod, "1");
  cr_assert(error == WOK);
  free(mod);
}

Test(wfunctions, should_not_try_to_get_modulo_by_zero) {
  werror_t error = WERROR;
  char *mod = weval_operator_int("%", "15000", "00", &error);
  cr_assert(mod == NULL);
  cr_assert(error == WERROR_DIVISION_BY_ZERO);
}

Test(wfuntions, should_not_try_to_calculate_non_existing_operation) {
  werror_t error = WOK;
  char *mod = weval_operator_int(")", "15000", "00", &error);
  cr_assert(mod == NULL);
  cr_assert(error == WERROR);
}

Test(wfunctions, should_calculate_equality_to_true) {
  werror_t error = WERROR;
  char *eq = weval_operator_int("=", "42", "42", &error);
  cr_assert_str_eq(eq, "1");
  cr_assert(error == WOK);
}

Test(wfunctions, should_calculate_equality_to_false) {
  werror_t error = WERROR;
  char *eq = weval_operator_int("=", "42", "24", &error);
  cr_assert_str_eq(eq, "0");
  cr_assert(error == WOK);
}

Test(wfunctions, should_return_NULL_for_a_nonexisting_function) {
  wfunction_t *function = wget_function("wea.do_not_exists");
  cr_assert(function == NULL);
}

Test(wfunctions, should_get_plus_operator) {
  wfunction_t *function = wget_function("+");
  cr_assert(function != NULL);
  cr_assert(function->wtype == WOPERATOR);
  cr_assert(function->wargc == 2);
}

Test (wfunctions, should_not_get_plus_operator_with_garbage) {
  wfunction_t *function = wget_function("+adsf");
  cr_assert(function == NULL);
}

Test(wfunctions, should_get_integer_operation_type_with_char) {
  cr_assert(wget_operation_wtype(WINTEGER, WCHAR) == WINTEGER);
  cr_assert(wget_operation_wtype(WCHAR, WINTEGER) == WINTEGER);
}

Test(wfunctions, should_get_integer_operation_type_with_boolean) {
  cr_assert(wget_operation_wtype(WINTEGER, WBOOLEAN) == WINTEGER);
  cr_assert(wget_operation_wtype(WBOOLEAN, WINTEGER) == WINTEGER);
}

Test(wfunctions, should_get_char_operation_type_with_boolean) {
  cr_assert(wget_operation_wtype(WCHAR, WBOOLEAN) == WCHAR);
  cr_assert(wget_operation_wtype(WBOOLEAN, WCHAR) == WCHAR);
}

Test(wfunctions, should_get_boolean_operation_type_with_boolean) {
  cr_assert(wget_operation_wtype(WBOOLEAN, WBOOLEAN) == WBOOLEAN);
}

Test(wfunctions, shout_get_unknown_operation_with_non_numbers) {
  cr_assert(wget_operation_wtype(WBOOLEAN, WOPERATOR) == WUNKNOWN);
  cr_assert(wget_operation_wtype(WBOOLEAN, WFUNCTION) == WUNKNOWN);
  cr_assert(wget_operation_wtype(WBOOLEAN, WRESERVED) == WUNKNOWN);
}
