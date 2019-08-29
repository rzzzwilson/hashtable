/******************************************************************************\
 *                             hashtable.h                                    *
 *                            =============                                   *
 *                                                                            *
 *  This file advertises the hashtable system interface.                      *
 *                                                                            *
 *  This abstracts (as much as possible) the hashtable mechanism and gets     *
 *  the user to provide only the hash algorithm and a record comparison       *
 *  function.  The functions provided are:                                    *
 *                                                                            *
 *      HASHTABLE hash_create(int numentries, HHASH hash, HCMP compare);      *
 *                                                                            *
 *          Creates a hashtable with 'numentries' slots. 'hash' is a function *
 *          used to create a hash value from the user data record. 'compare'  *
 *          is a function used to compare two data records. The function      *
 *          returns a token to be used when manipulating the new hashtable.   *
 *                                                                            *
 *      void hash_insert(HASHTABLE htable, void *record, int recsize);        *
 *                                                                            *
 *          Inserts a record into the hash table.  'htable' is the hashtable  *
 *          token returned from a hash_create() call.  'record' is a pointer  *
 *          to the data record to be inserted. 'recsize' is record size.      *
 *                                                                            *
 *      void *hash_lookup(HASHTABLE htable, void *record);                    *
 *                                                                            *
 *          Performs a lookup on the hashtable for a record matching the      *
 *          supplied lookup record 'record'.  Returns a pointer to the found  *
 *          record or NULL if the record isn't found.                         *
 *                                                                            *
 *      void hash_destroy(HASHTABLE htable);                                  *
 *                                                                            *
 *          Destroy a hashtable.                                              *
 *                                                                            *
 *  The records inserted into each hashtable must be the same.                *
 *                                                                            *
\******************************************************************************/

#ifndef HASHTABLE_H
#define HASHTABLE_H

/******
 * Typedefs for user.
 ******/

typedef void     *HASHTABLE;
typedef unsigned (*HHASH)(void *record);
typedef int      (*HCMP)(void *a, void *b);
typedef void     (*HDUMP)(void *record);


/******
 * Function prototypes.
 ******/

HASHTABLE hash_create(int numentries, HHASH hash, HCMP compare);
void      hash_insert(HASHTABLE htable, void *record, int recsize);
void      *hash_lookup(HASHTABLE htable, void *record);
void      hash_destroy(HASHTABLE htable);
void      hash_dump(HASHTABLE htable, HDUMP dump);


#endif
