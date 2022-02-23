#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "wexpression.h"
#include "wdeclaration.h"
#include "wfunctions.h"
#include "reductor.h"

bool is_literal(wexpression_t *expression, werror_t *error) {
  *error = WERROR;
  if (!expression) {
    return false;
  }
  *error = WOK; // expression may not be a literal, but it's still a valid one
  if (expression->wargc != 0) {
    return false;
  }
  switch(expression->wtype){
    case WINTEGER:
    case WBOOLEAN:
    case WCHAR:
      *error = WOK;
      return true;
    default:
      return false;
  }
}

bool is_delta_reductible(wexpression_t *expression, werror_t *error) {
  wfunction_t *function;
  wexpression_t *arg;
  *error = WERROR;
  if (!expression) {
    return false;
  }
  function = wget_function(expression->token);
  if (!function) {               // The token is not the name of an operator or in-built function
    switch (expression->wtype) { // Therefore, expression is not delta-reductible...
      case WFUNCTION:
      case WUNKNOWN:
        *error = WOK;           // ... but the use of a user-defined function or expression, that may be reduced to function, is OK
        return false;
      case WOPERATOR:
        return false;           // An unknown operator is an error
      default:                  // Maybe the user tries to use a literal as a function. Then, is not delta-reductible nor valid.
        *error = WERROR_NOT_EXECUTABLE;
        return false;
    }
  }
  if (expression->wargc > function->wargc) {
    *error = WERROR_TOO_MANY_ARGS;
    return false;
  }
  if (expression->wargc < function->wargc) {
    *error = WOK;  // if operator has too few args is not delta-reductible
    return false;  // but is not an error. It's equivalent to a function definition.
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
  *error = WOK;
  return true;
}

bool is_function_definition(wexpression_t *expression, werror_t *error) {
  *error = WERROR_TOO_FEW_ARGS; // if not especified before return, error is too few args
  wexpression_t *current;
  if (! expression) {
    *error = WERROR;
    return false;
  }
  if (strcmp(WEATOKEN, expression->token) != 0) { // if token is not "wea", it's not a function
    *error = WOK;                                 // but it may be a valid expression
    return false;
  }
  if (expression->wargc < 2) {    // expression should be at least (wea var . exp). Function with no args (wea . exp) is valid too
    return false;                 // that is, 2 args
  }
  // it should contain a dot (.) and then at least one expression
  current = expression->arg_wexpression;
  while (current) {
    if (strcmp(WDOTTOKEN, current->token) == 0) { // It's a dot expression
      if (current->wargc > 0) { // and it has at least one arg left
        *error = WOK;
        return true;
      } else {  // if it has no args, there is a malformed expression like (wea var .)
        return false;
      }
    }
   current = current->arg_wexpression;
  }
  // if here, it means there is no dot expression
  return false;
}
