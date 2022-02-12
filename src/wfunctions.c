#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "wexpression.h"
#include "wfunctions.h"

// List of built-in functions
wfunction_t wfunctions[] = {
  { WOPERATOR, "+", 2 },
  { WOPERATOR, "-", 2 },
  { WOPERATOR, "*", 2 },
  { WOPERATOR, "/", 2 },
  { WOPERATOR, "=", 2 },
  { WFUNCTION, "wea.if", 3 },
  { WFUNCTION, "wea.echo", 1 }
};

long long string_to_long(char *strnum, werror_t *error) {
    char *endptr;
    long long number = strtol(strnum, &endptr, 10);
    if ((errno == ERANGE) || (endptr == strnum) || (*endptr && *endptr != '\n')) {
        *error = WERROR_NOT_A_NUMBER;
        return 0;
    }
    return number;
}

char *weval_plus_operator_int(char *a, char *b, werror_t *error) {
    long long num_a, num_b;
    char *result;
    num_a = string_to_long(a, error);
    if (*error != WOK) {
      return NULL;
    }
    num_b = string_to_long(b, error);
    if (*error != WOK) {
      return NULL;
    }
    num_a += num_b;
    result = (char *)malloc(sizeof(char) * STR_SIZE);
    if (!result) {
      return NULL;
    }
    sprintf(result, "%lld" , num_a);
    return result;
}
