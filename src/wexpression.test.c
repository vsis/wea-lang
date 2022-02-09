#include <stdlib.h>
#include <criterion/criterion.h>

#include "wvariable.h"
#include "wexpression.h"

Test(wexpression, should_create_a_literal_expression) {
  char a = 'a';
  wexpression_t *my_expression = wexpression_create(WTYPE_CHAR, "", &a);
  cr_assert_str_eq(my_expression->wvar->name, "");
  cr_assert(my_expression->wvar->wdata.wchar == 'a');
  cr_assert(my_expression->arg_wexpression == NULL);
  cr_assert(my_expression->nested_wexpression == NULL);
  wexpression_free(my_expression);
}

Test(wexpression, should_append_expression) {
  // Make an expression like: + a b
  char *plus = "+";
  char *a = "a";
  char *b = "b";
  wexpression_t *my_expression = wexpression_create(WTYPE_BUILTIN_FUNCTION, plus, NULL);
  cr_assert(my_expression->wargc == 0);
  wexpression_t *a_expression = wexpression_create(WTYPE_INT, a, &(int){1});
  wexpression_append(my_expression, a_expression);
  cr_assert(my_expression->wargc == 1);
  cr_assert(my_expression->arg_wexpression == a_expression);
  wexpression_t *b_expression = wexpression_create(WTYPE_INT, b, &(int){2});
  wexpression_append(my_expression, b_expression);
  cr_assert(my_expression->wargc == 2);
  cr_assert(a_expression->wargc == 1);
  cr_assert(b_expression->wargc == 0);
  cr_assert(my_expression->arg_wexpression == a_expression);
  cr_assert(a_expression->arg_wexpression == b_expression);
  cr_assert(b_expression->arg_wexpression == NULL);
  wexpression_free(my_expression); // This should free these 3 expressions
}

Test(wexpression, should_nest_expression) {
  // Make an expression like: + (+ a b) c
  char *plus = "+";
  char *parenthesis = "(";
  char *a = "a";
  char *b = "b";
  char *c = "c";
  // Make inner expression (+ a b)
  wexpression_t *nested_expression = wexpression_create(WTYPE_BUILTIN_FUNCTION, parenthesis, NULL);
  wexpression_t *a_expression = wexpression_create(WTYPE_INT, a, NULL);
  wexpression_t *b_expression = wexpression_create(WTYPE_INT, b, NULL);
  wexpression_append(nested_expression, a_expression);
  wexpression_append(nested_expression, b_expression);
  cr_assert(nested_expression->wargc == 2);
  cr_assert(nested_expression->arg_wexpression == a_expression);
  cr_assert(nested_expression->arg_wexpression->arg_wexpression == b_expression);
  cr_assert(nested_expression->arg_wexpression->arg_wexpression->arg_wexpression == NULL);

  // Make outter expression + "(" c
  wexpression_t *my_expression = wexpression_create(WTYPE_BUILTIN_FUNCTION, plus, NULL);
  wexpression_t *arg_expression = wexpression_create(WTYPE_UNEVALUATED, parenthesis, NULL);
  wexpression_t *c_expression = wexpression_create(WTYPE_INT, c, NULL);
  wexpression_append(my_expression, arg_expression);
  wexpression_nest(arg_expression, nested_expression);
  wexpression_append(my_expression, c_expression);
  cr_assert(my_expression->wargc == 2);
  cr_assert(my_expression->arg_wexpression == arg_expression);
  cr_assert(my_expression->arg_wexpression->arg_wexpression == c_expression);
  cr_assert(my_expression->arg_wexpression->arg_wexpression->arg_wexpression == NULL);

  // Check if nested
  cr_assert(my_expression->arg_wexpression->nested_wexpression == nested_expression);
  cr_assert(my_expression->arg_wexpression->nested_wexpression->arg_wexpression == a_expression);
  cr_assert(my_expression->arg_wexpression->nested_wexpression->arg_wexpression->arg_wexpression == b_expression);
  cr_assert(my_expression->arg_wexpression->nested_wexpression->arg_wexpression->arg_wexpression->arg_wexpression == NULL);
  cr_assert(nested_expression->wargc == 2); // Make sure nested expression has not been modified

  // Free everythig
  wexpression_free(my_expression);
}

Test(wexpression, should_not_free_a_NULL_expression) {
  wexpression_t *my_expression = NULL;
  wexpression_free(my_expression);
}

Test(wexpression, should_not_append_a_NULL_expression) {
  wexpression_t *my_expression = wexpression_create(WTYPE_BUILTIN_FUNCTION, "", NULL);
  wexpression_append(my_expression, NULL);
  cr_assert(my_expression->wargc == 0);
  cr_assert(my_expression->arg_wexpression == NULL);
  wexpression_free(my_expression);
}

Test(wexpression, should_not_append_to_NULL_expression) {
  wexpression_t *my_expression = wexpression_create(WTYPE_BUILTIN_FUNCTION, "", NULL);
  wexpression_append(NULL, my_expression);
  wexpression_free(my_expression);
}

Test(wexpression, should_not_nest_a_NULL_expresion) {
  wexpression_t *my_expression = wexpression_create(WTYPE_BUILTIN_FUNCTION, "", NULL);
  wexpression_nest(my_expression, NULL);
  cr_assert(my_expression->nested_wexpression == NULL);
  wexpression_free(my_expression);
}

Test(wexpression, should_not_nest_to_NULL_expression) {
  wexpression_t *my_expression = wexpression_create(WTYPE_BUILTIN_FUNCTION, "", NULL);
  wexpression_nest(NULL, my_expression);
  wexpression_free(my_expression);
}

Test(wexpression, should_not_nest_twice) {
  wexpression_t *my_expression = wexpression_create(WTYPE_BUILTIN_FUNCTION, "", NULL);
  wexpression_t *a_expression = wexpression_create(WTYPE_INT, "", NULL);
  wexpression_t *b_expression = wexpression_create(WTYPE_INT, "", NULL);
  wexpression_nest(my_expression, a_expression);
  wexpression_nest(my_expression, b_expression);
  cr_assert(my_expression->nested_wexpression == a_expression);
  cr_assert(my_expression->nested_wexpression != b_expression);
  wexpression_free(my_expression);
  wexpression_free(b_expression);
}

Test(wexpression, should_not_fail_to_create_an_expression_with_empty_name) {
  wexpression_t *my_expression = wexpression_create(WTYPE_BUILTIN_FUNCTION, "", NULL);
  cr_assert_str_eq(my_expression->wvar->name, "");
  wexpression_free(my_expression);
}
