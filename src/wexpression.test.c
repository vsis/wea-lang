#include <stdlib.h>
#include <criterion/criterion.h>

#include "wvariable.h"
#include "wexpression.h"

Test(wexpression, should_create_an_add_expression) {
  wexpression_t *my_expression = wexpression_create(WOP_ADD);
  cr_assert(my_expression->woperator == WOP_ADD);
  cr_assert(my_expression->wargv == NULL);
  cr_assert(my_expression->wargc == 0);
  free(my_expression);
}

Test(wexpression, should_create_a_function_expression) {
  wexpression_t *my_expression = wexpression_create(WOP_FUNCTION);
  cr_assert(my_expression->woperator != WOP_ADD);
  cr_assert(my_expression->woperator == WOP_FUNCTION);
  cr_assert(my_expression->wargv == NULL);
  cr_assert(my_expression->wargc == 0);
  free(my_expression);
}

Test(wexpression, should_not_free_a_NULL_expression) {
  wexpression_t *my_expression = NULL;
  wexpression_free(my_expression);
}
