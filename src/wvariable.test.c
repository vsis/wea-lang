#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>

#include "wvariable.h"

Test(create_variable, should_create_int_variable) {
  int test_data = 42;
  wvariable_t *my_variable = wvariable_create(WTYPE_INT, "some_name", &test_data);
  cr_expect(my_variable != NULL, "Created variable is not null");
  cr_expect(my_variable->wdata_type == WTYPE_INT);
  cr_expect(my_variable->wdata.wint == (int) 42);
}

Test(create_variable, should_create_char_variable) {
  char test_data = 'A';
  wvariable_t *my_variable = wvariable_create(WTYPE_CHAR, "some_name", &test_data);
  cr_expect(my_variable != NULL, "Created variable is not null");
  cr_expect(my_variable->wdata_type == WTYPE_CHAR);
  cr_expect(my_variable->wdata.wchar == 'A');
}
