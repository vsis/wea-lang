#ifndef WEXPRESSION_H
#define WEXPRESSION_H


// Definition of expression
typedef struct wexpression {
  wvariable_t *wvar;                      // Data and name of this expression
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
wexpression_t *wexpression_create(wdata_type_t wdata_type, char *symbol, void *value);

// Add an argument to an expresison
void wexpression_append(wexpression_t *parent, wexpression_t *child);

// Add a nested expression
void wexpression_nest(wexpression_t *parent, wexpression_t *child);

// Free expression
void wexpression_free(wexpression_t *wexpression);

#endif
