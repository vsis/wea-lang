#ifndef WEXPRESSION_H
#define WEXPRESSION_H

#define WARGV_SIZE 4

// The operator of an expression
typedef enum woperator {
  WOP_FUNCTION,
  WOP_ADD,
  WOP_SUB,
  WOP_MUL,
  WOP_DIV
} woperator_t;

// Arguments of an operator
// It could be variable, literal (unnamed variables) or expression
typedef union warg {
  wvariable_t *wvariable;
  struct wexpression *wexpression;
} warg_t;

// Definition of expression
typedef struct wexpression {
  woperator_t woperator;
  warg_t *wargv;
  int wargc;
} wexpression_t;


// Create a new expression
wexpression_t *wexpression_create(woperator_t woperator);

// Add an argument to an expression
void wexpression_add_arg(wexpression_t *wexpression, warg_t warg);

// Replace an argument with a variable
// It will be used post-evaluation of a nested expression
void wexpression_update(wexpression_t *wexpression, int arg_pos, wvariable_t *wvar);

// Free expression
void wexpression_free(wexpression_t *wexpression);

#endif
