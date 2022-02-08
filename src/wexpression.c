#include <stdlib.h>

#include "wvariable.h"
#include "wexpression.h"


wexpression_t *wexpression_create(woperator_t woperator) {
  wexpression_t *new_expression = (wexpression_t *) malloc(sizeof(wexpression_t));
  if (new_expression == NULL) {
    return NULL;
  }
  new_expression->woperator = woperator;
  new_expression->wargv = NULL;
  new_expression->wargc = 0;
  return new_expression;
}


void wexpression_free(wexpression_t *wexpression) {
  if (wexpression != NULL) {
    free(wexpression);
  }
}
