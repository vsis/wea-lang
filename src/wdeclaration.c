#include <stdlib.h>
#include <string.h>

#include "wexpression.h"
#include "wdeclaration.h"


wdeclaration_t * insert_declaration(char *name, wexpression_t *expression, wdeclaration_t *declaration_list) {
  if ((!name) || (!expression)) {
    return NULL;
  }
  size_t name_length = strlen(name) + 1;
  wdeclaration_t *new_declaration = (wdeclaration_t *) malloc(sizeof(wdeclaration_t));
  if (!new_declaration) {
    return NULL;
  }
  new_declaration->name = (char *) malloc(sizeof(char) * name_length);
  if (!new_declaration->name) {
    free(new_declaration);
    return NULL;
  }
  strncpy(new_declaration->name, name, name_length);
  new_declaration->name[name_length - 1] = '\0';
  new_declaration->wexpression = expression;
  new_declaration->next_declaration = NULL;
  // If there is a declaration list, we put this declaration at the end
  if (declaration_list) {
    wdeclaration_t *last_element = declaration_list;
    // Iterate the list until the end
    while(last_element->next_declaration) {
      last_element = last_element->next_declaration;
    }
    // Put new declaration in the end
    last_element->next_declaration = new_declaration;
  }
  return new_declaration;
}

void wdeclaration_free(wdeclaration_t *declaration_list) {
  if (declaration_list) {
    wdeclaration_free(declaration_list->next_declaration);
    free(declaration_list->name);
    wexpression_free(declaration_list->wexpression);
    free(declaration_list);
  }
}

void wdeclaration_free_scope(char *scope, wdeclaration_t *declaration_list) {
  if ((!scope) || (!declaration_list)) {
    return;
  }
  char *in_scope;
  wdeclaration_t *declaration = declaration_list;
  wdeclaration_t *previous = NULL;
  while (declaration) { // Iterate declarations until we find a name with our scope
    in_scope = strstr(declaration->name, scope);
    if (in_scope) {     // If we foud the scope, we delete all list elements from here
      wdeclaration_free(declaration);
      declaration = NULL;
      if (previous) {   // If there is a previous declaration, it's next pointer should point to NULL
        previous->next_declaration = NULL;
      }
    } else { // If scope is not found, keep iterating
      previous = declaration;
      declaration = declaration->next_declaration;
    }
  }
}
