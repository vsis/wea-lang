#ifndef WFUNCTIONS_H
#define WFUNCTIONS_H

#define STR_SIZE 512

// Definition of built-in functions
typedef struct wfunction {
  wtype_t wtype;
  char *token;
  unsigned int wargc;
} wfunction_t;

// Definition of weaerror
typedef enum werror {
  WOK,
  WERROR_NOT_A_NUMBER
} werror_t;

// Returns the definition of the function of the given token
wfunction_t *wget_function(char *token);

// Evaluate a sum
char *weval_plus_operator_int(char *a, char *b, werror_t *error);

#endif
