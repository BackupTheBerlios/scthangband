/* ANSI Libraries */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "externfunc.h"

char output_file[1024],
     input_file[1024];

bool scan_vars;

char *output_lineending = "\n";

void output_help()
{
    printf("externgen 0.5.0 (08/12/01)\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Usage: externfunc [-options] <filename>\n");
    printf("Options:\n");
    printf("      -o        Specify the output filename.\n");
    printf("      -v        Create a list of variables.\n");
    printf("      -f        Create a list of functions.\n");
/*    printf("Putting ~ before an option turns it off where applicable.\n"); */

    return;
}

void scan_for_options(int argc, char *argv[])
{
    int count = 1,
        inc = 1;
/*    int ch_count = 0;
    unsigned int choice = TRUE; */

    argc--;

    while (count < argc)
    {
        inc = 1;

        if ( argv[count][0] == '-' )
        {
            if (argv[count][1] == 'o')
            {
                strcpy(output_file, argv[count+1]);
                inc = 2;
            }

            else if (argv[count][1] == 'v')
            {
                scan_vars = TRUE;
            }

            else if (argv[count][1] == 'f')
            {
                scan_vars = FALSE;
            }

            else if (argv[count][1] == 'h')
            {
                output_help();
                exit(0);
            }
        }
        else
        {
            fprintf(stderr,"%s is not a valid option, and will be ignored.\n",argv[count]);
        }
        
        count += inc;
    }

    return;
}

int main (int argc, char *argv[])
{
    FILE *outf_ptr;
    FILE *fptr;
    char buffer[1024];
    int bool = 1;

    /* Just externfunc */
    if (argc == 1)
    {
        output_help();
        return(FALSE);
    }

	strcpy(output_file, "stdout");

    if ( argc > 2 )
    {
        scan_for_options(argc, argv);
    }

	strcpy(input_file, argv[argc-1]);

    fptr = fopen(input_file,"rb+");
    outf_ptr = fopen(output_file,"w");    

    while (fgets(buffer, 1024, fptr) != 0)
    {
        if (bool == 0)
        {
			if (strchr(buffer, '\n')) *strchr(buffer, '\n') = '\0';
            if (scan_vars)
                analyse_variables(buffer, outf_ptr);
            else
                analyse_functions(buffer, outf_ptr);
            fputc('\n', outf_ptr);

            bool = !bool;
        }
        else if (bool == 1)
        {
            buffer[strlen(buffer)-1] = '\0';

            /* Write the source file name to this file. */
            fprintf(outf_ptr, "/* %s */%s", buffer, output_lineending);

            bool = !bool;
        }
    }

    fclose(fptr);
    fclose(outf_ptr);

    return (1);
}
