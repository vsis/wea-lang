#ifndef REDUCTOR_H
#define REDUCTOR_H

/*
 * 'Alpha convertion' is the procedure to change variable names
 * 'Beta reduction' is the procedure to apply a function by replacing argument variables with input variables
 * 'Delta reduction' is the procedure to apply built-in functions
 * 'Eta conversion' is the procedur to replace an expression with an equivalent one
 * These concepts are defined in Lambda calculus.
 *
 * 'Wea conversion' is the process to convert a function expression to an expression more easy to handle
 * For example: (wea x y . + x y) is wea-converted to (+ @1 @2), then it's easier to replace input variables there
 *
 * In this example ((+ @1 @2) 5 6) can be beta-reduced to (+ 5 6) then delta-reduced to (11)
 */


// True if expression is a literal
bool is_literal(wexpression_t *expression, werror_t * error);

// Determine if an expression is delta-reductible
// delta-reductible means is reductible by applying a built-in function
// also true if it's a literal
bool is_delta_reductible(wexpression_t *expression, werror_t *error);

// True if expression is a function
// A expression is a function if it's like wea <args> . <expression>
// Or an expression with variable names like @1, @2, @3, etc.
// The first should be reduced to the former with wea-conversion()
bool is_function_definition(wexpression_t *expression, werror_t *error);

// True if expression is a function described by: wea <args> . <expression>
// if True, wfunction_create may be applied
bool is_wea_convertible(wexpression_t *expression, werror_t *error);

// Apply a built-in function
void wdelta_reduction(wexpression_t *expression, werror_t *error);

// Rename free variables of an expression
// Creating function scopes to avoid variable name collision
// Name declarations are in declarations variable
void walfa_convertion_scope(wdeclaration_t *declarations,  wexpression_t *expression);

// Convert an expresion like ((a)) into (a)
void weta_convertion_parenthesis(wexpression_t *expression);

// Create a function
// A function is an expression with some variable names like @1, @2, @3...
// Identifying them as arguments
// To create a function, the input expression is required to be: wea <args> . <expression>
void wea_conversion(wexpression_t *expression);

// Replace bound variables (function arguments) of an expression
// Function is the expression to be replaced
// Args is the expression with the arguments
void wbeta_reduction(wexpression_t *function, wexpression_t *args);

// Replace variables with equivalent expressions given in declarations
void weta_convertion(wdeclaration_t *declarations, wexpression_t *expression);

#endif
