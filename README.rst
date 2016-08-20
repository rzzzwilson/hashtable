hashtable
=========

This directory contains a library for the use of a hashtable.
The **hashtable.h** file describes the interface to the library.

To build the library, do:

::

    $ make clean
    $ make

This creates the **hashtable.o** deliverable and runs simple tests.

Usage
-----

Before using a hashtable you must create it:

::

    HASHTABLE h = HashCreate(64, hash, compare);

You supply the size of the hashtable vector (64 elements in this case)
as well as the two user-supplied functions, **hash()** and
**compare()**.  You have to do this as the hashtable code knows
nothing about your records as they are passed in as **void**
pointers.

The compare function has the following signature:

::

    int compare(void *old, void *new);

where **old** and **new** are void pointers to two of your records.
Your code must cast these two pointers to pointers to your records
and somehow compare them.  The return value must be one of:

+---------+------------+
| Returns | When       |
+=========+============+
|   -1    | new < old  |
+---------+------------+
|    0    | new == old |
+---------+------------+
|   +1    | new > old  |
+---------+------------+

For example, we could do this:

::

    typedef struct
    {
        char string[128];
        int  count;
    } MYREC;

    int
    my_compare(void *old, void *new)
    {
        char *str_old = (MYREC *) old->string;
        char *str_new = (MYREC *) new->string;

        return strcmp(str_new, str_old);
    }

The hash function signature is:

::

    unsigned int hash(void *voidrecord)

where **voidrecord** is a void pointer to one of your records.  Your
code must generate a hash value (unsigned int) and return it.

Here's one possible hash function:
::

    unsigned
    hash(void *rec)
    {
        MYREC    *record = (MYREC *) rec;
        unsigned result = strlen(record->string);
        char     *chptr = record->string;
    
        while (*chptr)
            result *= *(chptr++);
    
        return result;
    }

Once the hashtable has been created you can populate it so:

::

    HASHTABLE h = HashCreate(64, hash, compare);

    typedef struct
    {
        char string[128];
        int  count;
    } MYREC;

    MYREC myrec;

    HashInsert(h, &myrec, sizeof(myrec));

To find a record in a hashtable do something like this:

::

    HASHTABLE h = HashCreate(64, hash, compare);

    /* insert some records here */

    char *record = "abc";
    void *result = HashLookup(h, (void *) record):

    if (result == NULL)
        /* not found */
    else
        /* found */

Look in **test.c** for some sample code.
