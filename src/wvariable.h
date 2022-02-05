#ifndef WVARIABLE_H
#define WVARIABLE_H

#define WVAR_NAME_MAX_LENGTH 64

typedef enum wdata_type {
  WTYPE_CHAR,
  WTYPE_INT} wdata_type_t;

typedef union wdata {
  int wint;
  char wchar;
} wdata_t;

typedef struct wvariable {
  char *name;
  wdata_type_t wdata_type;
  wdata_t wdata;
} wvariable_t;

// Create a new variable
// It allocates memory, so ti will need to be freed by wvariable_free()
wvariable_t *wvariable_create(wdata_type_t data_type, char *name, void *data);

// Updates the value of a variable
void wvariable_update(wvariable_t *wvar, void *data);

// Free variable from memory
void wvariable_free(wvariable_t *wvar);

#endif
