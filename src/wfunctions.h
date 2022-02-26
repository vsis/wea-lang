#ifndef WFUNCTIONS_H
#define WFUNCTIONS_H

#define WEATOKEN "wea"
#define WDOTTOKEN "."

// Definition of built-in functions
typedef struct wfunction {
  wtype_t wtype;
  char *token;
  unsigned int wargc;
} wfunction_t;

// Definition of weaerror
typedef enum werror {
  WOK,
  WERROR_TOO_MANY_ARGS,
  WERROR_TOO_FEW_ARGS,
  WERROR_NOT_A_NUMBER,
  WERROR_NOT_EXECUTABLE,
  WERROR_TOO_MANY_DOTS,
  WERROR_DIVISION_BY_ZERO,
  WERROR
} werror_t;

// Returns the definition of the function of the given token
wfunction_t *wget_function(char *token);

// Transform a token into a number (long long)
long long wtoken_to_long(char *token, werror_t *error);

// Given an operation with different data types, return the datatype that must be used
wtype_t wget_operation_wtype(wtype_t a, wtype_t b);

// Evaluate a sum
char *weval_operator_int(char *operator, char *a, char *b, werror_t *error);

#endif
