#ifndef WEXPRESSION_H
#define WEXPRESSION_H

#define STR_SIZE 512

// Data type
typedef enum wtype {
  WUNKNOWN,
  WOPERATOR,
  WFUNCTION,
  WINTEGER,
  WBOOLEAN,
  WCHAR,
  WRESERVED,
  WARGUMENT
} wtype_t;

// Definition of expression
typedef struct wexpression {
  char *token;                            // Name or literal of this expression
  wtype_t wtype;                          // Datatype of this expression
  struct wexpression *arg_wexpression;    // Argument of this expression
                                          // NULL if it's the last part of the expresion
  struct wexpression *nested_wexpression; // Not NULL if this expression is a nested one
                                          // example: + a (+ b c)
                                          //          | | |--> arg_expression = NULL
                                          //          | | |--> wvar->name = "("
                                          //          | | |--> nested_wexpression = + b c
                                          //          | |--> arg_expression = (
                                          //          | |--> wvar-name = "a"
                                          //          |--> wvar->name = "+"
                                          //          |--> arg_wexpression = a
  unsigned int wargc; // Number of arguments of this expression
} wexpression_t;


// Create a new literal expression
wexpression_t *wexpression_create(wtype_t type, char *symbol);

// Add an argument to an expresison
void wexpression_append(wexpression_t *parent, wexpression_t *child);

// Add a nested expression
void wexpression_nest(wexpression_t *parent, wexpression_t *child);

// Free expression
void wexpression_free(wexpression_t *wexpression);

#endif
