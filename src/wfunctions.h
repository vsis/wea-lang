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

// List of built-in functions
extern wfunction_t wfunctions[];

// Evaluate a sum
char *weval_plus_operator_int(char *a, char *b, werror_t *error);

#endif
