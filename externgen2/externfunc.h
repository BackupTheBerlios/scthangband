typedef unsigned int bool;
#define TRUE 1
#define FALSE 0

extern char *output_lineending;

extern bool analyse_functions(char *filename, FILE *outfile);
extern bool analyse_variables(char *filename, FILE *outfile);
