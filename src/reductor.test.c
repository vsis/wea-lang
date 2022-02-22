#include <stdlib.h>
#include <stdbool.h>
#include <criterion/criterion.h>

#include "wexpression.h"
#include "wdeclaration.h"
#include "reductor.h"

Test(reductor, should_not_find_NULL_as_literal) {
  cr_assert(! is_literal(NULL));
}

Test(reductor, should_not_find_nonliteral_as_literal) {
  wexpression_t *my_operator = wexpression_create(WOPERATOR, "+");
  wexpression_t *my_function = wexpression_create(WFUNCTION, "foo");
  wexpression_t *my_unknown = wexpression_create(WUNKNOWN, "(");
  cr_assert(! is_literal(my_operator));
  cr_assert(! is_literal(my_function));
  cr_assert(! is_literal(my_unknown));
  wexpression_free(my_operator);
  wexpression_free(my_function);
  wexpression_free(my_unknown);
}

Test(reductor, should_not_find_long_expression_as_literal) {
  wexpression_t *my_operator = wexpression_create(WOPERATOR, "+");
  wexpression_t *a = wexpression_create(WCHAR, "1");
  wexpression_t *b = wexpression_create(WCHAR, "2");
  wexpression_append(my_operator, a);
  wexpression_append(my_operator, b);
  cr_assert(! is_literal(my_operator));
  cr_assert(! is_literal(a));
  wexpression_free(my_operator);
}

Test(reductor, should_detect_a_literal) {
  wexpression_t *a = wexpression_create(WCHAR, "1");
  wexpression_t *b = wexpression_create(WINTEGER, "2");
  wexpression_t *c = wexpression_create(WBOOLEAN, "false");
  cr_assert(is_literal(a));
  cr_assert(is_literal(b));
  cr_assert(is_literal(c));
  wexpression_free(a);
  wexpression_free(b);
  wexpression_free(c);
}

Test(reductor, should_detect_a_delta_reductible_expression) {
  wexpression_t *plus = wexpression_create(WOPERATOR, "+");
  wexpression_t *one = wexpression_create(WINTEGER, "1");
  wexpression_t *two = wexpression_create(WINTEGER, "2");
  wexpression_append(plus, one);
  wexpression_append(plus, two);
  cr_assert(is_delta_reductible(plus));
  wexpression_free(plus);
}

Test(reductor, should_not_classify_a_NULL_expression_as_delta_reductible) {
  cr_assert(! is_delta_reductible(NULL));
}

Test(reductor, should_not_classify_a_nonexisting_operator_as_delta_reductible) {
  wexpression_t *plus = wexpression_create(WOPERATOR, "+asdf");
  wexpression_t *one = wexpression_create(WINTEGER, "1");
  wexpression_t *two = wexpression_create(WINTEGER, "2");
  wexpression_append(plus, one);
  wexpression_append(plus, two);
  cr_assert(! is_delta_reductible(plus));
  wexpression_free(plus);
}

Test(reductor, should_not_classify_a_expression_with_a_nested_one_as_delta_reductible) {
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
  cr_assert(! is_delta_reductible(plus));
  // inner expression is
  cr_assert(is_delta_reductible(plus2));
  wexpression_free(plus);
}

Test(reductor, should_not_classify_a_expression_with_wrong_number_of_arguments_as_delta_reductible) {
  wexpression_t *plus = wexpression_create(WOPERATOR, "+");
  wexpression_t *one = wexpression_create(WINTEGER, "1");
  wexpression_t *two = wexpression_create(WINTEGER, "2");
  wexpression_t *three = wexpression_create(WINTEGER, "3");
  wexpression_append(plus, one);
  wexpression_append(plus, two);
  wexpression_append(plus, three);
  cr_assert(! is_delta_reductible(plus));
  wexpression_free(plus);
}
