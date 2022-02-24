#include <stdlib.h>
#include <string.h>

#include "wexpression.h"

wexpression_t *wexpression_create(wtype_t type, char *symbol) {
  if (!symbol) {
    return NULL;
  }
  wexpression_t *new_expression = (wexpression_t *) malloc(sizeof(wexpression_t));
  if (!new_expression) {
    return NULL;
  }
  size_t symbol_length = strlen(symbol) + 1;
  new_expression->token = (char *)malloc(sizeof(char) * symbol_length);
  if (! new_expression->token) {
    free(new_expression);
    return NULL;
  }
  strncpy(new_expression->token, symbol, symbol_length);
  new_expression->wtype = type;
  new_expression->arg_wexpression = NULL;
  new_expression->nested_wexpression = NULL;
  new_expression->wargc = 0;
  return new_expression;
}

void wexpression_append(wexpression_t *parent, wexpression_t *child) {
  if ((!parent) || (!child)) {
    return;
  }
  parent->wargc++;
  if (parent->arg_wexpression) {
    wexpression_append(parent->arg_wexpression, child);
  } else {
    parent->arg_wexpression = child;
  }
}

void wexpression_nest(wexpression_t *parent, wexpression_t *child) {
  if ((!parent) || (parent->nested_wexpression) || (!child)) {
    return;
  }
  parent->nested_wexpression = child;
}

void wexpression_free(wexpression_t *wexpression) {
  if (wexpression) {
    wexpression_free(wexpression->nested_wexpression);
    wexpression_free(wexpression->arg_wexpression);
    free(wexpression->token);
    free(wexpression);
  }
}
