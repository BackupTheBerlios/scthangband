/* Utility-type program  */
/* Version ?.00          */
/* 25 July 1999          */

/* ANSI Libraries */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* Header files for this program */
#include "externfunc.h"

static bool lookback(char **ptr,char *start_ptr, char searchfor, bool whitespace)
{
    bool fault;

    while ( ( *ptr > start_ptr ) && ( *ptr[0] != searchfor ) )
    {
        fault = FALSE;
        
        if ( whitespace )
        {
            if (!( *ptr[0] == ' ' || *ptr[0] == '\n' || *ptr[0] =='\r' || *ptr[0]=='\t' ))                
            {
                fault = TRUE;

                if ( ( *ptr[0] == '\\' ) && ( *ptr[1] == '\n' || *ptr[1] == '\r' ) )
                    fault = FALSE;
            }
        }

        /* Skip comments */
        if ( *ptr[0] == '/' )
        {
            *ptr -= 1;
            if ( *ptr[0] == '*' )
            { 
                while ( ( *ptr > start_ptr ) && !( *ptr[0]=='/' && *ptr[1]=='*') ) 
                    *ptr -= 1;
        
                if ( *ptr == start_ptr )
                    return(FALSE);
                else
                    fault = FALSE;
            }
        }
         
        if ( fault && whitespace )
        {
            return(FALSE);
        }
         
        *ptr -= 1;
    }
    
    if ( *ptr == start_ptr ) return(FALSE);

    return(TRUE);
}

/* -------------------------------------------------- ajps, 30/11/01 ---
 * This searches between two points in the text for a function
 * definition - we call this knowing that there was a top-level { in
 * the text at the end of the block, but now there is a zero-terminator.
 * --------------------------------------------------------------------- */
static void do_search (char **in,FILE *out,int start,int end)
{ 
    char *start_ptr = (*in + start );
    char *end_ptr = (*in + end - 1);
    char *ptr;
    char *temp_ptr;
              
    /* First thing - find the last ')' before the end_ptr */
    ptr = end_ptr;
    if ( !lookback(&ptr,start_ptr,')',TRUE) ) return;

    /* As everything is successful up to now, move the end_ptr */
    end_ptr = ptr + 1;
    end_ptr[0]=0;
    
    if ( !lookback(&ptr,start_ptr,'(',FALSE) ) return;

    temp_ptr = ptr;
    if ( !lookback(&ptr,start_ptr,'\n',FALSE) )
    {
        ptr = temp_ptr;
        if ( !lookback(&ptr,start_ptr,'\r',FALSE) ) return;
    }

    ptr += 1;
    while ( ptr[0] == ' ' || ptr[0] == '\n' || ptr[0] =='\r' || ptr[0]=='\t' )
        ptr += 1;

    if ( strncmp(ptr,"static",6) == 0 ) return;

    if ( strncmp(ptr,"extern",6) == 0 ) ptr+=6;
    
    while ( ptr[0] == ' ' || ptr[0] == '\n' || ptr[0] =='\r' || ptr[0]=='\t' )
        ptr += 1;

    if ( strstr(ptr,"main") != NULL ) return;

    /* Print the lovely thing to file here */
    fprintf(out,"extern %s;%s",ptr,output_lineending); 
    return;
}

/* -------------------------------------------------- ajps, 30/11/01 ---
 * This does the actual business of scanning the source file for
 * function definitions, and outputting appropriate output.
 * --------------------------------------------------------------------- */
static void do_file (char **in, FILE *out,int file_size)
{
    char *start_ptr;
    int offset=0;
    int search_from=0,
        search_to=0;

    int level=0;
    bool comment=FALSE;

    start_ptr = *in;
    while ( offset < file_size )
    {
        if ( comment )
        {
            if ( start_ptr[offset]=='*' && start_ptr[offset+1]=='/' )
                comment = FALSE;
        }
        else
        {
            if ( start_ptr[offset]=='{' )
            {
                if ( level == 0 )
                {
                    start_ptr[offset] = 0;
                    search_to = offset;
                    do_search(in, out, search_from,search_to);
                }
                    
                level++;
            }
            
            if ( start_ptr[offset]=='}' )
            {
                level--;
                    
                if ( level == 0 )
                {
                    start_ptr[offset] = 0;
                    search_from = offset+1;
                }
            }
            
            if ( start_ptr[offset]=='/' && start_ptr[offset+1]=='*' )
                comment = TRUE;
        }
           

        offset += 1;
    }

    return;
}

/* -------------------------------------------------- ajps, 30/11/01 ---
 * This loads the file into memory, and finds the function definitions
 * which aren't declared as 'static'.  These are appended to the end of
 * the file pointed to by outfile.
 * --------------------------------------------------------------------- */
bool analyse_variables(char *filename, FILE *outfile)
{           
    FILE *fptr;

    char *infile;

    int file_size;
    
    /* Open source file */
    fptr = fopen(filename,"rb");
    if ( fptr == NULL )  
    {
        fprintf(stderr,"Could not open file '%s'.\n",filename);
        return(FALSE);
    }
    /* Get length of file, and go back to the start. */
    fseek(fptr,0,SEEK_END);
    file_size = (int) ftell(fptr);
    rewind(fptr);

    /* Allocate memory to hold the file in memory */
    infile = (char *) malloc(file_size+1);
    if ( infile == NULL )
    {
        fprintf(stderr,"Could not claim memory to hold file '%s'.\n",filename);
        fclose(fptr);
        return(FALSE);
    }
    infile[0] = 0;

    /* Load the file into memory & close the file */
    fread((void *)(infile + 1),file_size,1,fptr);
    fclose(fptr);
                                
    do_file(&infile,outfile,file_size);
            
    free(infile);
    return(TRUE);          
}
