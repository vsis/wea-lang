#include <stdlib.h>

#include "wvariable.h"
#include "wexpression.h"

wexpression_t *wexpression_create(wdata_type_t wdata_type, char *symbol, void *value) {
  if (symbol == NULL) {
    return NULL;
  }
  wvariable_t *wvar = wvariable_create(wdata_type, symbol, value);
  if (wvar == NULL) {
    return NULL;
  }
  wexpression_t *new_expression = (wexpression_t *) malloc(sizeof(wexpression_t));
  if (new_expression == NULL) {
    wvariable_free(wvar);
    return NULL;
  }
  new_expression->wvar = wvar;
  new_expression->arg_wexpression = NULL;
  new_expression->nested_wexpression = NULL;
  new_expression->wargc = 0;
  return new_expression;
}

void wexpression_append(wexpression_t *parent, wexpression_t *child) {
  if ((parent == NULL) || (child == NULL)) {
    return;
  }
  parent->wargc++;
  if (parent->arg_wexpression != NULL) {
    wexpression_append(parent->arg_wexpression, child);
  } else {
    parent->arg_wexpression = child;
  }
}

void wexpression_nest(wexpression_t *parent, wexpression_t *child) {
  if ((parent == NULL) || (parent->nested_wexpression != NULL) || (child == NULL)) {
    return;
  }
  parent->nested_wexpression = child;
}

void wexpression_free(wexpression_t *wexpression) {
  if (wexpression != NULL) {
    wexpression_free(wexpression->nested_wexpression);
    wexpression_free(wexpression->arg_wexpression);
    wvariable_free(wexpression->wvar);
    free(wexpression);
  }
}
