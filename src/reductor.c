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
  bool expression_is_literal = false;
  *error = WERROR; // if error is not especified when this returns, it's WERROR
  if (!expression) {
    return false;
  }
  // if expression is literal, return false
  expression_is_literal = is_literal(expression, error);
  if ((expression_is_literal) || (*error != WOK)) {
    return false;
  }
  *error = WERROR; // again, default error is WERROR
  // if not a literal, keep looking
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
      *error = WOK;
      return false; // It is not delta-reductible if it has a nested expression
    } else {
      arg = arg->arg_wexpression;
    }
  }
  *error = WOK;
  return true;
}

bool is_function_definition(wexpression_t *expression, werror_t *error) {
  int argc = 0;
  bool nested_expression_is_function = false;
  wexpression_t *current = expression;
  *error = WERROR;
  if (! expression) {
    return false;
  }
  // Check there is no WEA or dot (.) expression in args
  current = current->arg_wexpression;
  while (current) {
    if (current->wtype == WRESERVED) {
      return false;
    }
    if (current->wtype == WARGUMENT) {
      argc++;
    }
    // Check if nested expression is a function too
    if (is_function_definition(current->nested_wexpression, error)) {
      nested_expression_is_function = true;
    }
    current = current->arg_wexpression;
  }
  // At this point, expression is valid. But we do not know if nested expression are.
  // This expression is function if it has WARGUMENT elements or a nested espression is function
  *error = WOK;
  return (nested_expression_is_function || (argc > 0));
}

bool is_wea_convertible(wexpression_t *expression, werror_t *error) {
  unsigned int number_of_dots = 0;
  unsigned int argc_after_dot = 0;
  *error = WERROR_TOO_FEW_ARGS; // if not especified before return, error is too few args
  wexpression_t *current;
  if (! expression) {
    *error = WERROR;
    return false;
  }
  // Get the number of dots (.) in expression
  // And args after last dot
  current = expression->arg_wexpression;
  while (current) {
    if (strcmp(current->token, WDOTTOKEN) == 0) {
      number_of_dots++;
      argc_after_dot = current->wargc;
    }
    current = current->arg_wexpression;
  }
  if (strcmp(WEATOKEN, expression->token) != 0) { // if token is not "wea", it's not a function
    *error = WOK;                                 // but it may be a valid expression
    return false;
  }
  if (expression->wargc < 2) {    // expression should be at least (wea var . exp). Function with no args (wea . exp) is valid too
    return false;                 // that is, 2 args
  }
  // it should contain a single dot (.)
  if (number_of_dots > 1) {
    *error = WERROR_TOO_MANY_DOTS;
    return false;
  }
  if (number_of_dots < 1) {
    return false;
  }
  // it should have at least a expression after dot
  if (argc_after_dot < 1) {
    return false;
  }
  // if here, it's a valid wea-reductible expression
  *error = WOK;
  return true;
}

void weta_convertion_parenthesis(wexpression_t *expression) {
  if (! expression) {
    return;
  }
  if (expression->nested_wexpression) {
    weta_convertion_parenthesis(expression->nested_wexpression);
    if (expression->nested_wexpression->wargc == 0) { // Inner expression is only one token
      strncpy(expression->token, expression->nested_wexpression->token, STR_SIZE - 1);
      expression->wtype = expression->nested_wexpression->wtype;
      wexpression_free(expression->nested_wexpression);
      expression->nested_wexpression = NULL;
    }
  }
  weta_convertion_parenthesis(expression->arg_wexpression);
}


void wdelta_reduction(wexpression_t *expression, werror_t *error) {
  *error = WOK; // if no error is especified, there is no error
  wtype_t result_type = WUNKNOWN;
  wexpression_t *current = NULL;
  char *new_token = NULL;
  bool expression_is_delta_reductible = false;
  if (! expression) {
    return;
  }
  expression_is_delta_reductible = is_delta_reductible(expression, error);
  if (*error != WOK) { // if there is an error, transmit it
    return;
  }
  if (expression_is_delta_reductible) {
    if ((expression->wargc == 2) && (expression->wtype == WOPERATOR)) { // operator with 2 args
      result_type = wget_operation_wtype(expression->arg_wexpression->wtype, expression->arg_wexpression->arg_wexpression->wtype);
      switch (result_type) {
        case WINTEGER:
          new_token = weval_operator_int(expression->token, expression->arg_wexpression->token, expression->arg_wexpression->arg_wexpression->token, error);
          break;
        default:
          *error = WERROR;
          return;
      }
    } else { // expression is delta-reductible, but reduction is not implemented yet
      *error = WERROR;
      return;
    }
    // if evaluation gave error or there is no new token, return error.
    if (*error != WOK) {
      return;
    } else if (! new_token) {
      *error = WERROR;
      return;
    }
    // Replace expression with delta-reduction
    free(expression->token);
    expression->token = new_token;
    expression->wtype = result_type;
    wexpression_free(expression->arg_wexpression);
    expression->arg_wexpression = NULL;
    expression->wargc = 0;
  } else { // If expression is not delta-reductible, search in nested expressions
    current = expression->arg_wexpression;
    while (current) {
      wdelta_reduction(current->nested_wexpression, error);
      // if an error is found, return it
      if (*error != WOK) {
        return;
      }
      current = current->arg_wexpression;
    }
  }
}
