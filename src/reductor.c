#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "wexpression.h"
#include "wdeclaration.h"
#include "wfunctions.h"
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

bool is_delta_reductible(wexpression_t *expression) {
  wfunction_t *function;
  wexpression_t *arg;
  if (!expression) {
    return false;
  }
  function = wget_function(expression->token);
  if (!function) {
    return false;
  }
  if (expression->wargc != function->wargc) {
    return false;
  }
  // Check if an argument is a nested expression
  arg = expression;
  while(arg) {
    if (arg->nested_wexpression) {
      return false;
    } else {
      arg = arg->arg_wexpression;
    }
  }
  return true;
}
