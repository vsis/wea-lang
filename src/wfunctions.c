#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "wexpression.h"
#include "wfunctions.h"

// List of built-in functions
wfunction_t wfunctions[] = {
  { WOPERATOR, "+", 2 },
  { WOPERATOR, "-", 2 },
  { WOPERATOR, "*", 2 },
  { WOPERATOR, "/", 2 },
  { WOPERATOR, "=", 2 },
  { WOPERATOR, "%", 2 },
  { WFUNCTION, "wea.if", 3 },
  { WFUNCTION, "wea.echo", 1 },
  { WUNKNOWN, "null", 0}
};

bool is_number(wtype_t typ) {
  switch (typ) {
    case WINTEGER:
    case WCHAR:
    case WBOOLEAN:
      return true;
    default:
      return false;
  }
}

long long wtoken_to_long(char *token, werror_t *error) {
    char *endptr;
    long long number = strtol(token, &endptr, 10);
    if ((errno == ERANGE) || (endptr == token) || (*endptr && *endptr != '\n')) {
        *error = WERROR_NOT_A_NUMBER;
        return 0;
    }
    *error = WOK;
    return number;
}

wfunction_t *wget_function(char *token) {
  wfunction_t *current = wfunctions;
  while (current->wtype != WUNKNOWN) {
    if (strcmp(current->token, token) == 0) {
      return current;
    }
    current++;
  }
  return NULL;
}

char *weval_operator_int(char *operator, char *a, char *b, werror_t *error) {
    long long num_a, num_b;
    char *result;
    wfunction_t *op = wget_function(operator);
    *error = WERROR;
    // Check if it's a binary operator
    if ((! op) || (op->wtype != WOPERATOR) || (op->wargc != 2)) {
      return NULL;
    }
    // Get both parameters
    num_a = wtoken_to_long(a, error);
    if (*error != WOK) {
      return NULL;
    }
    num_b = wtoken_to_long(b, error);
    if (*error != WOK) {
      return NULL;
    }
    // Apply operation
    switch (*(op->token)) { // We assume here, operator have only one char
      case '+':
        num_a += num_b;
        break;
      case '-':
        num_a -= num_b;
        break;
      case '*':
        num_a *= num_b;
        break;
      case '/':
        if (num_b == 0) {
          *error = WERROR_DIVISION_BY_ZERO;
          return NULL;
        }
        num_a /= num_b;
        break;
      case '%':
        if (num_b == 0) {
          *error = WERROR_DIVISION_BY_ZERO;
          return NULL;
        }
        num_a %= num_b;
        break;
      case '=':
        if (num_a == num_b) {
          num_a = 1;
        } else {
          num_a = 0;
        }
        break;
      default:
        return NULL;
    }
    result = (char *)malloc(sizeof(char) * STR_SIZE);
    if (!result) {
      return NULL;
    }
    *error = WOK;
    (void) snprintf(result, STR_SIZE, "%lld" , num_a);
    return result;
}

wtype_t wget_operation_wtype(wtype_t a, wtype_t b) {
  if ((is_number(a)) && (is_number(b))) {
    if ((a == WINTEGER) || (b == WINTEGER)) {
      return WINTEGER;
    } else if ((a == WCHAR) || (b == WCHAR)) {
      return WCHAR;
    } else { // types are both number. So, the only option left is both are boolean
      return WBOOLEAN;
    }
  }
  return WUNKNOWN;
}
