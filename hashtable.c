/******************************************************************************\
 *                             hashtable.c                                    *
 *                            =============                                   *
 *                                                                            *
 *  This file implements the hashtable system.                                *
 *                                                                            *
 *  This abstracts (as much as possible) the hashtable mechanism and gets     *
 *  the user to provide only the hash algorithm and a record comparison       *
 *  function.  The functions provided are:                                    *
 *                                                                            *
 *      HASHTABLE HashCreate(int numentries, HHASH hash, HCMP compare);       *
 *                                                                            *
 *          Creates a hashtable with 'numentries' slots. 'hash' is a function *
 *          used to create a hash value from the user data record. 'compare'  *
 *          is a function used to compare two data records. The function      *
 *          returns a token to be used when manipulating the new hashtable.   *
 *                                                                            *
 *      void HashInsert(HASHTABLE htable, void *record, int recsize);         *
 *                                                                            *
 *          Inserts a record into the hash table.  'htable' is the hashtable  *
 *          token returned from a HashCreate() call.  'record' is a pointer   *
 *          to the data record to be inserted. 'recsize' is record size.      *
 *                                                                            *
 *      void *HashLookup(HASHTABLE htable, void *record);                     *
 *                                                                            *
 *          Performs a lookup on the hashtable for a record matching the      *
 *          supplied lookup record 'record'.  Returns a pointer to the found  *
 *          record or NULL if the record isn't found.                         *
 *                                                                            *
 *      void HashDestroy(HASHTABLE htable);                                   *
 *                                                                            *
 *          Destroy a hashtable.                                              *
 *                                                                            *
 *      void HashDump(HASHTABLE htable, HDUMP dump);                          *
 *                                                                            *
 *          Calls dump() for each record stored in hashtable.                 *
 *          (DEBUG purposes only, not defined if DEBUG not defined)           *
 *                                                                            *
 *  The records inserted into each hashtable must be the same.                *
 *                                                                            *
\******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include "hashtable.h"


/******
 * Define the hashtable header structure and data types.
 ******/

typedef void    *BUCKET;		/* each slot in hash table */

typedef struct
{
    int		numentries;		/* number of slots in the hash table */
    HHASH	hash;			/* the hash function for this table */
    HCMP	compare;		/* function to compare records */
    BUCKET	*table;			/* pointer to base of hash array */
} HTAB;


/******
 * Now define the bucket header.
 ******/

typedef struct bhdr
{
    struct bhdr *next;
    void        *data;
} BHDR;


/******
 * Local constants.
 ******/


/******
 * Static globals.
 ******/


/******
 * Forward prototype definitions.
 ******/


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

    exit(1);
}


/******************************************************************************
       Name : HashCreate()
Description : Routine to create a hashtable.
 Parameters : numentries - number of slots in the new hash table
            : hash       - address of user hash function
            : compare    - address of user record compare function
    Returns : Address (void *) of hashtable header.
   Comments : 
 ******************************************************************************/
HASHTABLE
HashCreate(int numentries, HHASH hash, HCMP compare)
{
    HTAB *htab = malloc(sizeof(HTAB));
    int  i;

    if (htab == NULL)
        error("Out of memory.");

    htab->numentries = numentries;
    htab->hash = hash;
    htab->compare = compare;

    htab->table = malloc(numentries * sizeof(BUCKET));
    if (htab->table == NULL)
        error("Out of memory.");

    for (i = 0; i < numentries; ++i)
        htab->table[i] = NULL;

    return (HASHTABLE) htab;
}


/******************************************************************************
       Name : HashInsert()
Description : Function to insert new record into the hash table.
 Parameters : htable  - address (void *) of hashtable header
            : record  - address (void *) of record to insert
            : recsize - size of record (bytes)
    Returns : 
   Comments : This routine takes a copy of the user record.
            : New record placed in front of any existing equal hash record.
 ******************************************************************************/
void
HashInsert(HASHTABLE htable, void *record, int recsize)
{
    HTAB *h = (HTAB *) htable;
    int  hash = h->hash(record) % h->numentries;
    BHDR *newbhdr = malloc(sizeof(BHDR));
    void *newrec = malloc(recsize);

    if (newbhdr == NULL || newrec == NULL)
        error("Out of memory");

    memcpy(newrec, record, recsize);

    newbhdr->data = newrec;
    newbhdr->next = h->table[hash];
    h->table[hash] = newbhdr;
}


/******************************************************************************
       Name : HashLookup()
Description : Look for a record in a hashtable.
 Parameters : htable - hashtable to look in
            : record - the record to look for
    Returns : Returns found data or NULL if not found.
   Comments : 
 ******************************************************************************/
void *
HashLookup(HASHTABLE htable, void *record)
{
    void *result = NULL;
    HTAB *h = (HTAB *) htable;
    int  hash = h->hash(record) % h->numentries;
    BHDR *bptr;

    bptr = h->table[hash];

    while (bptr != NULL)
    {
        if (h->compare(record, bptr->data) == 0)
        {
            result = bptr->data;
            break;
        }

        bptr = bptr->next;
    }

    return result;
}


/******************************************************************************
       Name : HashDestroy()
Description : Destroy a hash table.
 Parameters : htable - hash table to destroy
    Returns : 
   Comments : 
 ******************************************************************************/
void
HashDestroy(HASHTABLE htable)
{
    HTAB *h = (HTAB *) htable;
    int  i;

    for (i = 0; i < h->numentries; ++i)
    {
        BHDR *bptr = h->table[i];

        while (bptr != NULL)
        {
            BHDR *next = bptr->next;

            free(bptr->data);
            free(bptr);

            bptr = next;
        }
    }

    free(h->table);
    free(h);
}


/******************************************************************************
       Name : 
Description : 
 Parameters : 
    Returns : 
   Comments : 
 ******************************************************************************/
void
HashDump(HASHTABLE htable, HDUMP dump)
{
    HTAB *htab = (HTAB *) htable;
    int  i;

    for (i = 0; i < htab->numentries; ++i)
    {
        BHDR *bhdr = htab->table[i];

        while (bhdr != NULL)
        {
            dump(bhdr->data);
            bhdr = bhdr->next;
        }
    }
}
