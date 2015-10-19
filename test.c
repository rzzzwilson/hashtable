/******************************************************************************\
 *                                test.c                                      *
 *                               ========                                     *
 *                                                                            *
 *  This program is used to test the hashtable code library.                  *
 *                                                                            *
\******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <limits.h>

#include "hashtable.h"


/******
 * Local constants.
 ******/

#define PROGNAME                        "test"

#define EXIT_OK                         0
#define EXIT_ERROR                      1


/******
 * Record used in test hashtable.
 ******/

typedef struct
{
    char string[128];
    int  count;
} MYREC;


/******
 * Globals.
 ******/

/* Holders of the program name, input filenames, I/O streams, etc. */
static char *ProgName = NULL;
static FILE *InData = NULL;
static char *InDataFName = "stdin";

/* The output filenames and streams */
static FILE *OutData = NULL;
static char *OutDataFName = "stdout";

HASHTABLE hashtable;

MYREC     myrec;


/******
 * Forward prototype definitions.
 ******/

static void  error(char *fmt, ...);



/******************************************************************************
       Name : compare()
Description : Routine to compare two two data lines.
 Parameters : old - void pointer to old record (already in tree)
            : new - void pointer to new record (to be inserted in tree)
    Returns : An integer describing the relationship between the two records:
            :     -1  new < old
            :      0  new == old
            :     +1  new > old
   Comments : 
 ******************************************************************************/
int
compare(void *old, void *new)
{
    int result = strcmp(new, old);

    return result;
}


/******************************************************************************
       Name : dump()
Description : Routine to dump a record to the output file.
 Parameters : rec - address of record to dump
    Returns : 
   Comments : 
 ******************************************************************************/
void
dump(void *voidrec)
{
    MYREC *rec = (MYREC *) voidrec;

    fprintf(OutData, "Record: '%s', count=%d\n", rec->string, rec->count);
}


/******************************************************************************
       Name : error()
Description : printf()-like error routine.
 Parameters : params like printf()
    Returns : Doesn't, calls exit().
   Comments : 
 ******************************************************************************/
static void
error(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    exit(EXIT_ERROR);
}


/******************************************************************************
       Name : hash()
Description : Routine to generate the hash value for a record.
 Parameters : record - address of record to hash
    Returns : The hash value for the record.
   Comments : 
 ******************************************************************************/
unsigned
hash(void *voidrecord)
{
    MYREC    *record = (MYREC *) voidrecord;
    unsigned result = strlen(record->string);
    char     *chptr = record->string;

    while (*chptr)
        result *= *(chptr++);

    return result;
}


/******************************************************************************
       Name : test()
Description : Production routine for the 'test' system.
 Parameters : in      - input data stream
            : inname  - input stream name
            : out     - output data stream
            : outname - output stream name
    Returns : 
   Comments : 
 ******************************************************************************/
static void
test(FILE *in, char *inname, FILE *out, char *outname)
{
    char      buff[128];
    HASHTABLE h;

/******
 * Create the hashtable
 ******/

    h = HashCreate(64, hash, compare);

/******
 * Read data from input and process it.
 ******/

    while (fgets(myrec.string, sizeof(myrec.string), in) != NULL)
    {
        char *chptr;
        void *rec;

        chptr = strchr(myrec.string, '\n');
        if (chptr != NULL)
            *chptr = '\0';

        myrec.count = 1;

        rec = HashLookup(h, &myrec);
        if (rec == NULL)
        {
            fprintf(out, "Inserted new record '%s'\n", myrec.string);
            HashInsert(h, &myrec, sizeof(myrec));
        }
        else
        {
            MYREC *oldrec = (MYREC *) rec;

            fprintf(out, "Found old record '%s', count=%d\n",
                    oldrec->string, ++oldrec->count);
        }
    }

/******
 * Now dump data to output stream.
 ******/

    HashDump(h, dump);

/******
 * Delete the hashtable.
 ******/

    HashDestroy(h);
}


/******************************************************************************
       Name : usage()
Description : Function to provide much needed help to the user.
 Parameters : 
    Returns : Doesn't, calls error().
   Comments : Expects global 'ProgName' to point to the program name.
 ******************************************************************************/
static void
usage(void)
{
    error("usage: %s [ <in> [ <out> ] ]\n"
          "\n"
          "where <in>  is the (optional) input filename,\n"
          "      <out> is the (optional) output filename.\n",
          ProgName);
}


/******************************************************************************
       Name : main()
Description : 
 Parameters : 
    Returns : 
   Comments : 
 ******************************************************************************/
int
main(int argc, char *argv[])
{
/*    MCinit(0); */

/******
 * Initialize globals.
 ******/

    if (argv[0] != NULL)
    {
        char *chptr;

        ProgName = argv[0]; 
        chptr = strrchr(argv[0], '/');
        if (chptr != NULL)
            ProgName = ++chptr;
    }
    else
        ProgName = PROGNAME;

    InData = stdin;
    OutData = stdout;
 
/******
 * Check the input files now.
 ******/

    switch (argc)
    {
        case 3:
            OutDataFName = argv[2];
            OutData = fopen(OutDataFName, "w");
            if (OutData == NULL)
                error("Can't open '%s' for output", OutDataFName);
        case 2:
            InDataFName = argv[1];
            InData = fopen(InDataFName, "r");
            if (InData == NULL)
            {
                if (OutData != stdout)
                    fclose(OutData);
                error("Can't open '%s' for input", InDataFName);
            }
        case 1:
            break;

        default:
            usage();
    }

/******
 * Start the 'test' routine.
 ******/

    test(InData, InDataFName, OutData, OutDataFName);

/******
 * Close down.
 ******/

/*    MCterm(); */

    return EXIT_OK;
}
