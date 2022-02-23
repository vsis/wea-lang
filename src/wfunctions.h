#ifndef WFUNCTIONS_H
#define WFUNCTIONS_H

#define STR_SIZE 512
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
  WERROR
} werror_t;

// Returns the definition of the function of the given token
wfunction_t *wget_function(char *token);

// Evaluate a sum
char *weval_plus_operator_int(char *a, char *b, werror_t *error);

#endif
