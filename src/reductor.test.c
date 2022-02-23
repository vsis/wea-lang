#include <stdlib.h>
#include <stdbool.h>
#include <criterion/criterion.h>

#include "wexpression.h"
#include "wdeclaration.h"
#include "wfunctions.h"
#include "reductor.h"

Test(reductor, should_not_find_NULL_as_literal) {
  werror_t error = WOK;
  cr_assert(! is_literal(NULL, &error));
  cr_assert(error == WERROR);
}

Test(reductor, should_not_find_nonliteral_as_literal) {
  werror_t error1 = WERROR;
  werror_t error2 = WERROR;
  werror_t error3 = WERROR;
  wexpression_t *my_operator = wexpression_create(WOPERATOR, "+");
  wexpression_t *my_function = wexpression_create(WFUNCTION, "foo");
  wexpression_t *my_unknown = wexpression_create(WUNKNOWN, "(");
  cr_assert(! is_literal(my_operator, &error1));
  cr_assert(! is_literal(my_function, &error2));
  cr_assert(! is_literal(my_unknown, &error3));
  cr_assert(error1 == WOK);
  cr_assert(error2 == WOK);
  cr_assert(error3 == WOK);
  wexpression_free(my_operator);
  wexpression_free(my_function);
  wexpression_free(my_unknown);
}

Test(reductor, should_not_find_long_expression_as_literal) {
  werror_t error1 = WERROR;
  werror_t error2 = WERROR;
  werror_t error3 = WERROR;
  wexpression_t *my_operator = wexpression_create(WOPERATOR, "+");
  wexpression_t *a = wexpression_create(WCHAR, "1");
  wexpression_t *b = wexpression_create(WCHAR, "2");
  wexpression_append(my_operator, a);
  wexpression_append(my_operator, b);
  cr_assert(! is_literal(my_operator, &error1));
  cr_assert(! is_literal(a, &error2));
  cr_assert(is_literal(b, &error3));
  cr_assert(error1 == WOK);
  cr_assert(error2 == WOK);
  cr_assert(error3 == WOK);
  wexpression_free(my_operator);
}

Test(reductor, should_detect_a_literal) {
  werror_t error1 = WERROR;
  werror_t error2 = WERROR;
  werror_t error3 = WERROR;
  wexpression_t *a = wexpression_create(WCHAR, "1");
  wexpression_t *b = wexpression_create(WINTEGER, "2");
  wexpression_t *c = wexpression_create(WBOOLEAN, "false");
  cr_assert(is_literal(a, &error1));
  cr_assert(is_literal(b, &error2));
  cr_assert(is_literal(c, &error3));
  cr_assert(error1 == WOK);
  cr_assert(error2 == WOK);
  cr_assert(error3 == WOK);
  wexpression_free(a);
  wexpression_free(b);
  wexpression_free(c);
}

Test(reductor, should_detect_a_delta_reductible_expression) {
  werror_t error = WERROR;
  wexpression_t *plus = wexpression_create(WOPERATOR, "+");
  wexpression_t *one = wexpression_create(WINTEGER, "1");
  wexpression_t *two = wexpression_create(WINTEGER, "2");
  wexpression_append(plus, one);
  wexpression_append(plus, two);
  cr_assert(is_delta_reductible(plus, &error));
  cr_assert(error == WOK);
  wexpression_free(plus);
}

Test(reductor, should_not_classify_a_NULL_expression_as_delta_reductible) {
  werror_t error = WOK;
  cr_assert(! is_delta_reductible(NULL, &error));
  cr_assert(error == WERROR);
}

Test(reductor, should_not_classify_a_nonexisting_operator_as_delta_reductible) {
  werror_t error = WOK;
  wexpression_t *plus = wexpression_create(WOPERATOR, "+asdf");
  wexpression_t *one = wexpression_create(WINTEGER, "1");
  wexpression_t *two = wexpression_create(WINTEGER, "2");
  wexpression_append(plus, one);
  wexpression_append(plus, two);
  cr_assert(! is_delta_reductible(plus, &error));
  cr_assert(error == WERROR);
  wexpression_free(plus);
}

Test(reductor, should_not_classify_a_expression_with_a_nested_one_as_delta_reductible) {
  werror_t error1 = WOK;
  werror_t error2 = WERROR;
  wexpression_t *plus = wexpression_create(WOPERATOR, "+");
  wexpression_t *plus2 = wexpression_create(WOPERATOR, "+");
  wexpression_t *parenthesis = wexpression_create(WUNKNOWN, "(");
  wexpression_t *one = wexpression_create(WINTEGER, "1");
  wexpression_t *two = wexpression_create(WINTEGER, "2");
  wexpression_t *three = wexpression_create(WINTEGER, "3");
  // create outter expression
  wexpression_append(plus, one);
  wexpression_append(plus, parenthesis);
  // create inner expression
  wexpression_append(plus2, two);
  wexpression_append(plus2, three);
  // nest expressions
  wexpression_nest(parenthesis, plus2);
  // outter expression is not delta-reductible
  cr_assert(! is_delta_reductible(plus, &error1));
  cr_assert(error1 == WERROR);
  // inner expression is
  cr_assert(is_delta_reductible(plus2, &error2));
  cr_assert(error2 == WOK);
  wexpression_free(plus);
}

Test(reductor, should_not_classify_a_expression_with_wrong_number_of_arguments_as_delta_reductible) {
  werror_t error = WOK;
  wexpression_t *plus = wexpression_create(WOPERATOR, "+");
  wexpression_t *one = wexpression_create(WINTEGER, "1");
  wexpression_t *two = wexpression_create(WINTEGER, "2");
  wexpression_t *three = wexpression_create(WINTEGER, "3");
  wexpression_append(plus, one);
  wexpression_append(plus, two);
  wexpression_append(plus, three);
  cr_assert(! is_delta_reductible(plus, &error));
  cr_assert(error == WERROR_TOO_MANY_ARGS);
  wexpression_free(plus);
}

Test(reductor, should_correctly_classify_a_expression_with_operator_and_too_few_args) {
  werror_t error = WERROR;
  wexpression_t *plus = wexpression_create(WOPERATOR, "+");
  wexpression_t *one = wexpression_create(WINTEGER, "1");
  wexpression_append(plus, one);
  cr_assert(! is_delta_reductible(plus, &error));
  cr_assert(error == WOK); // Expression is not delta-reductible, but it's a valid one
  wexpression_free(plus);
}

Test(reductor, should_not_classify_a_expression_with_not_a_function_as_delta_reductible) {
  werror_t error = WOK;
  wexpression_t *one = wexpression_create(WINTEGER, "1");
  wexpression_t *two = wexpression_create(WINTEGER, "2");
  wexpression_append(one, two);
  cr_assert(! is_delta_reductible(one, &error));
  cr_assert(error == WERROR_NOT_EXECUTABLE);
  wexpression_free(one);
}

Test(reductor, should_not_classify_a_expression_with_unknown_function_as_delta_reductible) {
  werror_t error = WERROR;
  wexpression_t *function = wexpression_create(WFUNCTION, "my_function");
  wexpression_t *two = wexpression_create(WINTEGER, "2");
  wexpression_append(function, two);
  cr_assert(! is_delta_reductible(function, &error));
  cr_assert(error == WOK);
  wexpression_free(function);
}