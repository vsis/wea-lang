#ifndef WDECLARATION_H
#define WDECLARATION_H

// A declaration is a named expression
typedef struct wdeclaration {
  char *name;
  wexpression_t *wexpression;
  struct wdeclaration *next_declaration;
} wdeclaration_t;

// Add a new declaration to a declaration_list
// If declaration_list is NULL, it creates the list
// Always return a pointer to the newest declaration
wdeclaration_t  *insert_declaration(char *name, wexpression_t *expression, wdeclaration_t *declaration_list);

// Free() every element of the declaration_list
void wdeclaration_free(wdeclaration_t *declaration_list);

// Free() every element of the declaration_list of the given scope
void wdeclaration_free_scope(char *scope, wdeclaration_t *declaration_list);

#endif
