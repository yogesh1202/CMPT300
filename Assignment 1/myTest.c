/**
 * Sample test routine for executing each function at least once.
 * Copyright Brian Fraser, 2020
 */

#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// Macro for custom testing; does exit(1) on failure.
#define CHECK(condition) do{ \
    if (!(condition)) { \
        printf("ERROR: %s (@%d): failed condition \"%s\"\n", __func__, __LINE__, #condition); \
        exit(1);\
    }\
} while(0)



// For checking the "free" function called
static int complexTestFreeCounter = 0;
static void complexTestFreeFn(void* pItem) 
{
    CHECK(pItem != NULL);
    complexTestFreeCounter++;
}

// For searching
static bool itemEquals(void* pItem, void* pArg) 
{
    return (pItem == pArg);
}

static void testComplex()
{
    // Empty list
    List* pList = List_create();
    CHECK(pList != NULL);
    CHECK(List_count(pList) == 0);

    // Add 
    int added = 41;
    CHECK(List_add(pList, &added) == 0);
    CHECK(List_count(pList) == 1);
    CHECK(List_curr(pList) == &added);
    // Insert
    int inserted = 42;
    CHECK(List_insert(pList, &inserted) == 0);
    CHECK(List_count(pList) == 2);
    CHECK(List_curr(pList) == &inserted);
    
    // // Prepend
    int prepended = 43;
    CHECK(List_prepend(pList, &prepended) == 0);
    CHECK(List_count(pList) == 3);
    CHECK(List_curr(pList) == &prepended);
    
    // Append
    int appended = 44;
    CHECK(List_append(pList, &appended) == 0);
    CHECK(List_count(pList) == 4);
    CHECK(List_curr(pList) == &appended);
    
    // Next through it all (from before list)
   CHECK(List_first(pList) == &prepended);
   CHECK(List_prev(pList) == NULL);
   CHECK(List_next(pList) == &prepended);
   CHECK(List_next(pList) == &inserted);
   CHECK(List_next(pList) == &added);
    
    CHECK(List_next(pList) == &appended);
    CHECK(List_next(pList) == NULL);
    CHECK(List_next(pList) == NULL);

    // Prev through it all starting from past end
    CHECK(List_last(pList) == &appended);
    CHECK(List_next(pList) == NULL);
    CHECK(List_prev(pList) == &appended);
    CHECK(List_prev(pList) == &added);
    CHECK(List_prev(pList) == &inserted);
    CHECK(List_prev(pList) == &prepended);
    CHECK(List_prev(pList) == NULL);
    CHECK(List_prev(pList) == NULL);

    // Remove first
    CHECK(List_first(pList) == &prepended);
    CHECK(List_remove(pList) == &prepended);
    CHECK(List_curr(pList) == &inserted);

    // Trim last
    CHECK(List_trim(pList) == &appended);
    CHECK(List_curr(pList) == &added);

    // // Free remaining 2 elements
    complexTestFreeCounter = 0;
    List_free(pList, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 2);


    // // Concat
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    List* pList1 = List_create();
    List_add(pList1, &one);
    List_add(pList1, &two);
    List* pList2 = List_create();
    List_add(pList2, &three);
    List_add(pList2, &four);
    
    List_concat(pList1, pList2);
    CHECK(List_count(pList1) == 4);
    CHECK(List_first(pList1) == &one);
    CHECK(List_last(pList1) == &four);


    // // Search
    List_first(pList1);
    CHECK(List_search(pList1, itemEquals, &two) == &two);
    CHECK(List_search(pList1, itemEquals, &two) == &two);
    CHECK(List_search(pList1, itemEquals, &one) == NULL); 

    // free a concatenated list
    complexTestFreeCounter = 0;
    List_free(pList1, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 4);

    //adding,inserting, appending and removing to a freed list
    List* pList3 = List_create();
    char a = 'a';
    CHECK(List_add(pList3, &a) == 0);
    CHECK(List_search(pList3, itemEquals, &a) == &a);
    CHECK(List_remove(pList3) == &a);
    CHECK(List_remove(pList3) == NULL);
    CHECK(List_curr(pList3) == NULL);
    CHECK(List_search(pList3, itemEquals, &a) == NULL);
    complexTestFreeCounter = 0;
    List_free(pList1, complexTestFreeFn);
    CHECK(complexTestFreeCounter == 0);
    
    CHECK(List_prepend(pList1, &a) == 0);
    CHECK(List_append(pList2, &a) == 0);
    CHECK(List_insert(pList, &a) == 0);
    CHECK(List_remove(pList) == &a);

    List *pList4 = List_create();
    CHECK(&pList4 != NULL);
    char name[5] = "tpony";
    CHECK(List_prepend(pList4, &name[0]) == 0);
    CHECK(List_append(pList4, &name[4]) == 0);
    CHECK(List_first(pList4)==&name[0]);
    CHECK(List_add(pList4, &name[1]) == 0);
    CHECK(List_next(pList4) == &name[4]);
    CHECK(List_insert(pList4, &name[2]) == 0);
    CHECK(List_curr(pList4) == &name[2]);
    CHECK(List_add(pList4, &name[3]) == 0);
    CHECK(List_prev(pList4)==&name[2]);

    List *pList5 = List_create();
    CHECK(pList5 != NULL);
    CHECK(List_count(pList5) == 0);
    List *pList6 = List_create();
    CHECK(pList6 != NULL);
    CHECK(List_count(pList6) == 0);
    List *pList7 = List_create();
    CHECK(pList7 != NULL);
    CHECK(List_count(pList7) == 0);
    List *pList8 = List_create();
    CHECK(pList8 != NULL);
    CHECK(List_count(pList8) == 0);
    List *pList9 = List_create();
    CHECK(pList9 != NULL);
    CHECK(List_count(pList9) == 0);
    List *pList10 = List_create();
    CHECK(pList10 == NULL);
}

int main(int argCount, char *args[]) 
{
    testComplex();

    // We got here?!? PASSED!
    printf("********************************\n");
    printf("           PASSED\n");
    printf("********************************\n");
    return 0;
}