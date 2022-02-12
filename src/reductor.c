#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "wexpression.h"
#include "wdeclaration.h"
#include "reductor.h"

bool is_literal(wexpression_t *expression) {
  if (!expression) {
    return false;
  }
  if (expression->wargc != 0) {
    return false;
  }
  switch(expression->wtype){
    case WINTEGER:
    case WBOOLEAN:
    case WCHAR:
      return true;
    default:
      return false;
  }
}


