#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>

#include "wvariable.h"

Test(wvariable, should_create_int_variable) {
  int test_data = 42;
  wvariable_t *my_variable = wvariable_create(WTYPE_INT, "some_name", &test_data);
  cr_expect(my_variable != NULL, "Created variable is not null");
  cr_expect(my_variable->wdata_type == WTYPE_INT);
  cr_expect(my_variable->wdata.wint == (int) 42);
  wvariable_free(my_variable);
}

Test(wvariable, should_create_char_variable) {
  char test_data = 'A';
  wvariable_t *my_variable = wvariable_create(WTYPE_CHAR, "some_name", &test_data);
  cr_expect(my_variable != NULL, "Created variable is not null");
  cr_expect(my_variable->wdata_type == WTYPE_CHAR);
  cr_expect(my_variable->wdata.wchar == 'A');
  wvariable_free(my_variable);
}

Test(wvariable, should_save_variable_name) {
  char test_data = 'A';
  wvariable_t *my_variable = wvariable_create(WTYPE_CHAR, "some_name", &test_data);
  cr_assert_str_eq(my_variable->name, "some_name");
  wvariable_free(my_variable);
}

Test(wvariable, should_not_update_a_NULL_wvariable) {
  int data = 42;
  wvariable_t *my_variable = NULL;
  wvariable_update(my_variable, &data);
}

Test(wvariable, should_update_an_existing_variable) {
  char old_data = 'A';
  char new_data = 'z';
  wvariable_t *my_variable = wvariable_create(WTYPE_CHAR, "some_name", &old_data);
  cr_expect(my_variable->wdata.wchar == 'A');
  wvariable_update(my_variable, &new_data);
  cr_expect(my_variable->wdata.wchar == 'z');
  wvariable_free(my_variable);
}

Test(wvariable, should_not_free_a_NULL_wvariable) {
  wvariable_t *my_variable = NULL;
  wvariable_free(my_variable);
}

Test(wvariable, should_not_fail_to_create_a_variable_with_NULL_name) {
  wvariable_t *my_variable = wvariable_create(WTYPE_CHAR, NULL, NULL);
  cr_expect(my_variable == NULL);
  wvariable_free(my_variable);
}

Test(wvariable, should_not_fail_to_create_a_variable_with_empty_name) {
  wvariable_t *my_variable = wvariable_create(WTYPE_CHAR, "", NULL);
  cr_assert_str_eq(my_variable->name, "");
  wvariable_free(my_variable);
}
