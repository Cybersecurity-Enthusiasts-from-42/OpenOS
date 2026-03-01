/*
 * OpenOS - Shell Scripting Support
 * 
 * Provides enhanced shell with scripting capabilities
 */

#ifndef OPENOS_SCRIPT_H
#define OPENOS_SCRIPT_H

#include <stdint.h>

/* Maximum script size */
#define MAX_SCRIPT_SIZE 4096

/* Maximum variable name length */
#define MAX_VAR_NAME 32

/* Maximum variable value length */
#define MAX_VAR_VALUE 256

/* Maximum number of variables */
#define MAX_VARIABLES 64

/* Variable structure */
typedef struct script_var {
    char name[MAX_VAR_NAME];
    char value[MAX_VAR_VALUE];
    int is_set;
} script_var_t;

/* Script context */
typedef struct script_context {
    script_var_t variables[MAX_VARIABLES];
    int in_if_block;
    int if_condition_result;
    int loop_depth;
} script_context_t;

/* Initialize scripting subsystem */
void script_init(void);

/* Execute a script file or string */
int script_execute(const char* script);

/* Variable operations */
int script_set_var(const char* name, const char* value);
const char* script_get_var(const char* name);
void script_unset_var(const char* name);

/* Control flow parsing */
int script_parse_if(const char* condition);
int script_parse_while(const char* condition);
int script_parse_for(const char* statement);

/* I/O redirection */
int script_redirect_output(const char* filename);
int script_redirect_input(const char* filename);

/* Pipe support */
int script_create_pipe(const char* cmd1, const char* cmd2);

#endif /* OPENOS_SCRIPT_H */
