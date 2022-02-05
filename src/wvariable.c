#include <stdlib.h>
#include <string.h>

#include "wvariable.h"


wvariable_t *wvariable_create(wdata_type_t data_type, char *name, void *data){
  wvariable_t *new_variable;
  char * new_name;
  size_t name_length = strlen(name) + 1;

  // Return NULL if wrong name size
  if ((name_length < 1 ) || (name_length > WVAR_NAME_MAX_LENGTH )) {
    return NULL;
  }

  // Allocate memory
  // Free() everything and return NULL if malloc() fails
  new_variable  = (wvariable_t *) malloc(sizeof(wvariable_t));
  if (new_variable == NULL) {
    return NULL;
  }
  new_name = (char *) malloc(sizeof(char) * name_length);
  if (new_name == NULL){
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
  // It depends on data type
  switch (data_type) {
    case WTYPE_CHAR:
      char *new_char = (char *) data;
      new_variable->wdata.wchar = *new_char;
      break;
    case WTYPE_INT:
      int *new_int = (int *) data;
      new_variable->wdata.wint = *new_int;
      break;
  }
  
  return new_variable;
}
