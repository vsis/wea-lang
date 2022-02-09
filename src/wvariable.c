#include <stdlib.h>
#include <string.h>

#include "wvariable.h"

// This function add data to a wvariable
// the wvariable should be already created
void wvariable_insert_data(wvariable_t *wvar, void *data) {
  if (!data) {
    return;
  }
  // Copy data
  // It depends on data type
  switch (wvar->wdata_type) {
    case WTYPE_CHAR:
      char *new_char = (char *) data;
      wvar->wdata.wchar = *new_char;
      break;
    case WTYPE_INT:
      int *new_int = (int *) data;
      wvar->wdata.wint = *new_int;
      break;
  }
}

wvariable_t *wvariable_create(wdata_type_t data_type, char *name, void *data){
  wvariable_t *new_variable;
  char * new_name;
  size_t name_length;

  // Check if name is NULL
  if (!name) {
    return NULL;
  } else {
    name_length = strlen(name) + 1;
  }

  // Return NULL if wrong name size
  // We allow an empty string as a valid name for anonymous variables
  if ((name_length < 1 ) || (name_length > WVAR_NAME_MAX_LENGTH )) {
    return NULL;
  }

  // Allocate memory
  // Free() everything and return NULL if malloc() fails
  new_variable  = (wvariable_t *) malloc(sizeof(wvariable_t));
  if (!new_variable) {
    return NULL;
  }
  new_name = (char *) malloc(sizeof(char) * name_length);
  if (!new_name) {
    free(new_variable);
    return NULL;
  }

  // Copy name
  new_variable->name = new_name;
  strncpy(new_variable->name, name, name_length);
  new_variable->name[name_length - 1] = '\0';

  // Copy data type
  new_variable->wdata_type = data_type;

  // Copy data
  wvariable_insert_data(new_variable, data);
  return new_variable;
}

void wvariable_update(wvariable_t *wvar, void *data) {
  if (wvar) {         // This function does the same as wvariable_insert_data(),
                      // but it checks for a NULL input
    wvariable_insert_data(wvar, data);
  }
}

void wvariable_free(wvariable_t *wvar){
  if (wvar) {
    free(wvar->name);
    wvar->name = NULL;
    free(wvar);
  }
}
