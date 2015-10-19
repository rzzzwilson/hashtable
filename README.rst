hashtable
=========

This directory contains a library for the use of a hashtable.
The **hashtable.h** file describes the interface to the library.

To build the library, do:

    $ make clean
    $ make

This creates the **hashtable.o** deliverable and runs simple tests.

Usage
-----

Before using a hashtable you must create it:

::

    HASHTABLE h = HashCreate(64, hash, compare);

You supply the size of the hashtable vector (64 elements in this case)
as well as the to user-supplied functions, **hash()** and
**compare()**.  You have to do this as the hashtable code knows
nothing about your records as they are passed in as **void**
pointers.

The auxiliary functions have the following signatures:

::

    int compare(void *old, void *new);

where **old** and **new** are void pointers to two of your records.
Your code must cast these two pointers to pointers to your records
and somehow compare them.  The return value is one of:

+---------+------------+
| Returns | When       |
+=========+============+
|   -1    | new < old  |
+---------+------------+
|    0    | new == old |
+---------+------------+
|   +1    | new > old  |
+---------+------------+

The hash function is:

    unsigned int hash(void *voidrecord)

where **voidrecord** is a void pointer to one of your records.  Your
code must generate a hash value (unsigned int) and return it.

Once the hashtable has been created you can populate it so:

::

    typedef struct
    {
        char string[128];
        int  count;
    } MYREC;

    MYREC     myrec;

    void *rec = HashLookup(h, &myrec);

Look in **test.c** for some sample code.
