#include <stdlib.h>
#include <criterion/criterion.h>

#include "wexpression.h"
#include "wdeclaration.h"

Test(wdeclaration, should_create_a_declaration) {
  wexpression_t *expression = wexpression_create(WINTEGER, "50");
  wdeclaration_t *declaration = insert_declaration("a", expression, NULL);
  cr_assert(declaration != NULL);
  cr_assert_str_eq(declaration->name, "a");
  cr_assert(declaration->wexpression == expression);
  wdeclaration_free(declaration);
}

Test(wdeclaration, should_not_create_a_declaration_with_NULL_name) {
  wexpression_t *my_expression = wexpression_create(WINTEGER, "42");
  wdeclaration_t *declaration = insert_declaration(NULL, my_expression, NULL);
  cr_assert(declaration == NULL);
}

Test(wdeclaration, should_not_create_a_declaration_with_NULL_expression) {
  wdeclaration_t *declaration = insert_declaration("a", NULL, NULL);
  cr_assert(declaration == NULL);
}

Test(wdeclaration, should_append_new_declarations) {
  wexpression_t *a_expression = wexpression_create(WINTEGER, "50");
  wexpression_t *b_expression = wexpression_create(WINTEGER, "51");
  wexpression_t *c_expression = wexpression_create(WINTEGER, "52");
  wexpression_t *d_expression = wexpression_create(WINTEGER, "53");
  wdeclaration_t *a_declaration = insert_declaration("a", a_expression, NULL);
  wdeclaration_t *b_declaration = insert_declaration("b", b_expression, a_declaration);
  wdeclaration_t *c_declaration = insert_declaration("c", c_expression, a_declaration);
  wdeclaration_t *d_declaration = insert_declaration("d", d_expression, a_declaration);
  
  // Check right declaration order
  cr_assert(b_declaration == a_declaration->next_declaration);
  cr_assert(c_declaration == a_declaration->next_declaration->next_declaration);
  cr_assert(d_declaration == a_declaration->next_declaration->next_declaration->next_declaration);
  cr_assert(NULL == a_declaration->next_declaration->next_declaration->next_declaration->next_declaration);

  // Check right expression order
  cr_assert(a_expression == a_declaration->wexpression);
  cr_assert(b_expression == a_declaration->next_declaration->wexpression);
  cr_assert(c_expression == a_declaration->next_declaration->next_declaration->wexpression);
  cr_assert(d_expression == a_declaration->next_declaration->next_declaration->next_declaration->wexpression);

  // Check right token order
  cr_assert_str_eq("50", a_declaration->wexpression->token);
  cr_assert_str_eq("51", a_declaration->next_declaration->wexpression->token);
  cr_assert_str_eq("52", a_declaration->next_declaration->next_declaration->wexpression->token);
  cr_assert_str_eq("53", a_declaration->next_declaration->next_declaration->next_declaration->wexpression->token);
  // Free everything
  wdeclaration_free(a_declaration);
}

Test(wdeclaration, should_not_free_a_NULL_declaration) {
  wdeclaration_free(NULL);
}

Test(wdeclaration, shoud_free_a_scope) {
  wexpression_t *a_expression = wexpression_create(WINTEGER, "50");
  wexpression_t *b_expression = wexpression_create(WINTEGER, "51");
  wexpression_t *c_expression = wexpression_create(WINTEGER, "52");
  wexpression_t *d_expression = wexpression_create(WINTEGER, "53");
  wdeclaration_t *a_declaration = insert_declaration("a", a_expression, NULL);
  wdeclaration_t *b_declaration = insert_declaration("b", b_expression, a_declaration);
  wdeclaration_t *c_declaration = insert_declaration("1.c", c_expression, a_declaration);
  wdeclaration_t *d_declaration = insert_declaration("1.d", d_expression, a_declaration);
  
  // Check right declaration order
  cr_assert(b_declaration == a_declaration->next_declaration);
  cr_assert(c_declaration == a_declaration->next_declaration->next_declaration);
  cr_assert(d_declaration == a_declaration->next_declaration->next_declaration->next_declaration);
  cr_assert(NULL == a_declaration->next_declaration->next_declaration->next_declaration->next_declaration);

  // Free scope 1
  wdeclaration_free_scope("1.", a_declaration);

  //Check right declaration order
  cr_assert(b_declaration == a_declaration->next_declaration);
  cr_assert(NULL == a_declaration->next_declaration->next_declaration);

  // Check right expression order
  cr_assert(a_expression == a_declaration->wexpression);
  cr_assert(b_expression == a_declaration->next_declaration->wexpression);

  // Free everything
  wdeclaration_free(a_declaration);
}

Test(wdeclaration, should_free_nothing_if_there_is_no_scope) {
  wexpression_t *a_expression = wexpression_create(WINTEGER, "50");
  wexpression_t *b_expression = wexpression_create(WINTEGER, "51");
  wexpression_t *c_expression = wexpression_create(WINTEGER, "52");
  wexpression_t *d_expression = wexpression_create(WINTEGER, "53");
  wdeclaration_t *a_declaration = insert_declaration("a", a_expression, NULL);
  wdeclaration_t *b_declaration = insert_declaration("b", b_expression, a_declaration);
  wdeclaration_t *c_declaration = insert_declaration("c", c_expression, a_declaration);
  wdeclaration_t *d_declaration = insert_declaration("d", d_expression, a_declaration);
  
  // Free unexisting scope
  wdeclaration_free_scope("1.", a_declaration);

  // Everyting should be intact  
  // Check right declaration order
  cr_assert(b_declaration == a_declaration->next_declaration);
  cr_assert(c_declaration == a_declaration->next_declaration->next_declaration);
  cr_assert(d_declaration == a_declaration->next_declaration->next_declaration->next_declaration);
  cr_assert(NULL == a_declaration->next_declaration->next_declaration->next_declaration->next_declaration);

  // Check right expression order
  cr_assert(a_expression == a_declaration->wexpression);
  cr_assert(b_expression == a_declaration->next_declaration->wexpression);
  cr_assert(c_expression == a_declaration->next_declaration->next_declaration->wexpression);
  cr_assert(d_expression == a_declaration->next_declaration->next_declaration->next_declaration->wexpression);

  // Check right token order
  cr_assert_str_eq("50", a_declaration->wexpression->token);
  cr_assert_str_eq("51", a_declaration->next_declaration->wexpression->token);
  cr_assert_str_eq("52", a_declaration->next_declaration->next_declaration->wexpression->token);
  cr_assert_str_eq("53", a_declaration->next_declaration->next_declaration->next_declaration->wexpression->token);
  // Free everything
  wdeclaration_free(a_declaration);
}

Test(wdeclaration, should_not_free_scope_a_NULL_list) {
  wdeclaration_free_scope("1.", NULL);
}

Test(wdeclaration, should_free_last_scope_only) {
  wexpression_t *a_expression = wexpression_create(WINTEGER, "50");
  wexpression_t *b_expression = wexpression_create(WINTEGER, "51");
  wexpression_t *c_expression = wexpression_create(WINTEGER, "52");
  wexpression_t *d_expression = wexpression_create(WINTEGER, "53");
  wdeclaration_t *a_declaration = insert_declaration("a", a_expression, NULL);
  wdeclaration_t *b_declaration = insert_declaration("1.b", b_expression, a_declaration);
  wdeclaration_t *c_declaration = insert_declaration("1.c", c_expression, a_declaration);
  wdeclaration_t *d_declaration = insert_declaration("2.d", d_expression, a_declaration);
  
  // Free last scope
  wdeclaration_free_scope("2.", a_declaration);

  // Check right declaration order
  cr_assert(b_declaration == a_declaration->next_declaration);
  cr_assert(c_declaration == a_declaration->next_declaration->next_declaration);
  cr_assert(NULL == a_declaration->next_declaration->next_declaration->next_declaration);

  // Check right expression order
  cr_assert(a_expression == a_declaration->wexpression);
  cr_assert(b_expression == a_declaration->next_declaration->wexpression);
  cr_assert(c_expression == a_declaration->next_declaration->next_declaration->wexpression);

  // Check right token order
  cr_assert_str_eq("50", a_declaration->wexpression->token);
  cr_assert_str_eq("51", a_declaration->next_declaration->wexpression->token);
  cr_assert_str_eq("52", a_declaration->next_declaration->next_declaration->wexpression->token);
  
  // Free everything
  wdeclaration_free(a_declaration);
}

Test(wdeclaration, should_free_nothing_if_there_scope_is_NULL) {
  wexpression_t *a_expression = wexpression_create(WINTEGER, "50");
  wexpression_t *b_expression = wexpression_create(WINTEGER, "51");
  wexpression_t *c_expression = wexpression_create(WINTEGER, "52");
  wexpression_t *d_expression = wexpression_create(WINTEGER, "53");
  wdeclaration_t *a_declaration = insert_declaration("a", a_expression, NULL);
  wdeclaration_t *b_declaration = insert_declaration("1.b", b_expression, a_declaration);
  wdeclaration_t *c_declaration = insert_declaration("1.c", c_expression, a_declaration);
  wdeclaration_t *d_declaration = insert_declaration("1.d", d_expression, a_declaration);
  
  // Free NULL scope
  wdeclaration_free_scope(NULL, a_declaration);

  // Everyting should be intact  
  // Check right declaration order
  cr_assert(b_declaration == a_declaration->next_declaration);
  cr_assert(c_declaration == a_declaration->next_declaration->next_declaration);
  cr_assert(d_declaration == a_declaration->next_declaration->next_declaration->next_declaration);
  cr_assert(NULL == a_declaration->next_declaration->next_declaration->next_declaration->next_declaration);

  // Check right expression order
  cr_assert(a_expression == a_declaration->wexpression);
  cr_assert(b_expression == a_declaration->next_declaration->wexpression);
  cr_assert(c_expression == a_declaration->next_declaration->next_declaration->wexpression);
  cr_assert(d_expression == a_declaration->next_declaration->next_declaration->next_declaration->wexpression);

  // Check right token order
  cr_assert_str_eq("50", a_declaration->wexpression->token);
  cr_assert_str_eq("51", a_declaration->next_declaration->wexpression->token);
  cr_assert_str_eq("52", a_declaration->next_declaration->next_declaration->wexpression->token);
  cr_assert_str_eq("53", a_declaration->next_declaration->next_declaration->next_declaration->wexpression->token);
  // Free everything
  wdeclaration_free(a_declaration);
}
