#include "list.h"
#include "stdio.h"
#include <assert.h>
static bool firstTime = true;
static int numLists = 0;
static int freeListIdx = 0;
static int newFreeListIdx = 1;
static int numNodes = 0;
List lists[LIST_MAX_NUM_HEADS] = {};
Node nodes[LIST_MAX_NUM_NODES+LIST_MAX_NUM_HEADS*2] = {};

/**
 * initializer function, initializes both static arrays
 * including nodes and lists with nulls except for indexes
 * 
 */
static void init(){
    // printf("Initializing lists and nodes\n");
    for (int j = 0; j < LIST_MAX_NUM_NODES; j++)
    {
        nodes[j] = (Node){ //initializer for regular nodes
                    NULL, //item
                    NULL, //prev
                    NULL, //next
                    j}; //index
    }
    for (int k = LIST_MAX_NUM_NODES; k < LIST_MAX_NUM_HEADS + LIST_MAX_NUM_NODES; k++) //initializer for head sentinels
    {
        nodes[k] = (Node){ 
                    NULL, 
                    &nodes[LIST_MAX_NUM_HEADS * (k % LIST_MAX_NUM_NODES)], 
                    //points to LIST_MAX_NUM_HEADS increment nodes starting from 0
                    &nodes[k], //points to itself as a prev
                    -1}; //special negative index
    }
    for (int l = LIST_MAX_NUM_NODES + LIST_MAX_NUM_HEADS; l < 2 * LIST_MAX_NUM_HEADS + LIST_MAX_NUM_NODES; l++) // tail sentinels
    {
        nodes[l] = (Node){
                    NULL,
                    &nodes[l], //itself as a next
                    //points to LIST_MAX_NUM_NODES/LIST_MAX_NUM_HEADS node starting from 0
                    &nodes[LIST_MAX_NUM_HEADS * l % (LIST_MAX_NUM_NODES + LIST_MAX_NUM_HEADS) + LIST_MAX_NUM_HEADS],
                    -5}; //special negative index
    }
    for (int i = 0; i < LIST_MAX_NUM_HEADS; i++)
    {
        lists[i] = (List){
                    0, // size
                    NULL, // current
                    NULL, // head
                    NULL, // tail
                    &nodes[i + LIST_MAX_NUM_NODES], // head sentinel
                    &nodes[i + LIST_MAX_NUM_HEADS + LIST_MAX_NUM_NODES], // tail sentinel
                    i, //index
                    i * LIST_MAX_NUM_HEADS, //freeNodeIndex
                    i * LIST_MAX_NUM_HEADS + 1}; //nextFreeNodeIndex
    }
    firstTime = false;
}

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create(){
    if (firstTime)
    {
        init();
    }
    else if (numLists > LIST_MAX_NUM_HEADS)
    {
        // printf("Number of lists exceeded LIST_MAX_NUM_HEADS\n");
        return NULL;
    }
    int t_listIdx = freeListIdx; //using global list index and storing it temporarily
    freeListIdx = newFreeListIdx;
    if (t_listIdx < LIST_MAX_NUM_HEADS)
    {
        newFreeListIdx = lists[freeListIdx].index + 1;
        lists[t_listIdx].h_sentinel->next = lists[t_listIdx].head; //set up sentinel 
        lists[t_listIdx].t_sentinel->prev = lists[t_listIdx].tail; //set up tail
        ++numLists;
    }
    else
    {
        // printf("Number of lists exceeded LIST_MAX_NUM_HEADS\n");
        return NULL;
    }
    return &lists[t_listIdx];
}

// Returns the number of items in pList.
int List_count(List* pList){
    assert(pList!=NULL);
    return pList->size; //return node count
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){
    if(List_count(pList) == 0){
        //if list is empty
        pList->curr = NULL;
    }
    else{
        //point to head
        pList->curr = pList->head;
    }
    return pList->curr->item;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    assert(pList!=NULL);
    if(List_count(pList) == 0){
        //if list is empty
        pList->curr = NULL;
    }
    else{
        // point to tail
        pList->curr = pList->tail;
    }
    return pList->curr->item;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
    assert(pList!=NULL);
    if(pList->curr->index == -5) 
    //if the current pointer is pointing to the tail sentinel (beyond)
    {
        return NULL;
    }
    pList->curr = pList->curr->next; 
    return pList->curr->item;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
    assert(pList!=NULL);
    if(pList->curr->index == -1) 
    // if current pointer is pointing to the head sentinel, before start
    {
        return NULL;
    }
    pList->curr = pList->curr->prev;
    return pList->curr->item;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList){
    return pList->curr->item;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem){
    assert(pList!=NULL);
    int t_nodeIdx = pList->freeNodeIdx;
    if(numNodes>=LIST_MAX_NUM_NODES){ //too many nodes
        // printf("NUM_NODES exceeded max number of nodes allowed\n");
        return -1;
    }
    else if((pList->curr == pList->t_sentinel || pList->curr == pList->tail) && pList->size>0){
        // if current at the end of list pr beyond
        // printf("Now appending through add\n");
        return List_append(pList, pItem);
    }
    else if(pList->curr == pList->h_sentinel && pList->size>0){
        // printf("Now prepending through add\n");
        return List_prepend(pList,pItem);
    }
    pList->freeNodeIdx = pList->nextFreeNodeIdx; //retrieving previous new free node index to add node to
    if(pList->size == 0){
        pList->nextFreeNodeIdx++;
        nodes[t_nodeIdx].item = pItem; //assign item
        // printf("Now adding to new list\n");
        pList->head = pList->tail = &nodes[t_nodeIdx]; //setting new head and tail to new node
        pList->tail->next = pList->t_sentinel; 
        pList->head->prev = pList->h_sentinel;
    }
    else{
        nodes[t_nodeIdx].item = pItem; //assign item
        pList->nextFreeNodeIdx = pList->freeNodeIdx+1;
        // printf("Now adding\n");
        nodes[t_nodeIdx].prev = pList->curr; //setting new node prev and next
        nodes[t_nodeIdx].next = pList->curr->next;
        pList->curr->next = pList->curr->next->prev =  &nodes[t_nodeIdx]; // join the two nodes around new item
    } 
    ++pList->size; //increase size of list and node count
    ++numNodes;   
    pList->curr = &nodes[t_nodeIdx]; //set current node
    return 0;
}
// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem){
    assert(pList!=NULL);
    int t_freeIdx = pList->freeNodeIdx;
    if(numNodes>LIST_MAX_NUM_NODES){//too many nodes
        // printf("NUM_NODES exceeded max number of nodes allowed\n");
        return -1;
    }
    else if(pList->curr == pList->t_sentinel && pList->size>0){
        // printf("Now appending through insert\n");
        return List_append(pList, pItem);
    }
    else if((pList->curr == pList->head || pList->curr == pList->h_sentinel) && pList->size>0){
        // printf("Now prepending through insert\n");
        return List_prepend(pList, pItem);
    }
    
    pList->freeNodeIdx = pList->nextFreeNodeIdx; //retrieving previous new free node index to add node to
     // increment new free node index
    nodes[t_freeIdx].item = pItem; //assign item
    if (pList->size == 0){
        pList->nextFreeNodeIdx++;
        // printf("Now inserting to new list\n");
        pList->head = pList->tail = &nodes[t_freeIdx];
        pList->tail->next = pList->t_sentinel; //New list so tail and head are the same, setting sentinels
        pList->head->prev = pList->h_sentinel;
    } 
    // else if(pList->curr == pList->tail){ //TODO: do I need this?
    //     pList->nextFreeNodeIdx = List_count(pList) + 2;
    //     nodes[t_freeIdx].next = pList->curr;
    //     nodes[t_freeIdx].prev = pList->curr->prev;
    //     pList->tail->prev = &nodes[t_freeIdx];
    // }
    else{
        // printf("Now inserting\n");
        pList->nextFreeNodeIdx = pList->freeNodeIdx+1;
        nodes[t_freeIdx].next = pList->curr;
        nodes[t_freeIdx].prev = pList->curr->prev; //inserting between two nodes
        pList->curr->next->prev = pList->curr->prev = &nodes[t_freeIdx]; // join surrounding nodes
        // pList->curr->prev  = &nodes[t_freeIdx];
    } 
    ++pList->size; //increase size of list and node count
    ++numNodes;   
    pList->curr = &nodes[t_freeIdx]; //set current node
    return 0;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){
    assert(pList!=NULL);
    if(numNodes >= LIST_MAX_NUM_NODES){//too many nodes
        // printf("NUM_NODES exceeded max number of nodes allowed\n");
        return -1;
    }
    else if(pList->size<1){ //if new list, add.
        return List_add(pList, pItem); 
    }
    int t_freeIdx = numNodes; //same as add/insert code, retrieving index and assigning item
    pList->freeNodeIdx = pList->nextFreeNodeIdx;
    pList->nextFreeNodeIdx = pList->freeNodeIdx+1;
    nodes[t_freeIdx].item = pItem;

    pList->tail->next = &nodes[t_freeIdx]; //adding new tail
    nodes[t_freeIdx].prev = pList->tail;
    pList->tail = &nodes[t_freeIdx];

    pList->tail->next = pList->t_sentinel; //assigning tail to sentinel
    pList->t_sentinel->prev = &nodes[t_freeIdx];

    pList->curr = &nodes[t_freeIdx]; //new current 
    pList->size++;
    numNodes++;
    return 0;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){
    assert(pList!=NULL);
    
    if(numNodes >= LIST_MAX_NUM_NODES){//too many nodes
        // printf("NUM_NODES exceeded max number of nodes allowed\n");
        return -1;
    }
    else if(pList->size<1){ //new list, insert
        return List_insert(pList, pItem);
    }
    int t_freeIdx = numNodes; //same as add/insert code, retrieving index and assigning item
    pList->freeNodeIdx = pList->nextFreeNodeIdx;
    pList->nextFreeNodeIdx = pList->freeNodeIdx+1;
    nodes[t_freeIdx].item = pItem;

    pList->head->prev = &nodes[t_freeIdx]; //adding new head
    nodes[t_freeIdx].next = pList->head;
    pList->head = &nodes[t_freeIdx];

    pList->head->prev = pList->h_sentinel; //assigning head to sentinel
    pList->h_sentinel->next = &nodes[t_freeIdx];

    pList->curr = &nodes[t_freeIdx];
    pList->size++;
    numNodes++;
    return 0;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){
    assert(pList!=NULL);
    void* itemRemoved;
    if(pList->curr == pList->h_sentinel || pList->curr == pList->t_sentinel){
        // printf("Pointer beyond end or before start of pList\n");
        return NULL;
    }
    else if(List_count(pList)<=0){
        // printf("No nodes left to remove");
        return NULL;
    }
    else if(pList->curr == pList->head){ //if removing from head
        itemRemoved = pList->curr->item;
        pList->head = pList->curr->next;
        pList->curr->item = NULL;
        pList->curr->prev->next = pList->head;
        pList->curr->prev = pList->h_sentinel;
        pList->curr = pList->head;
        // printf("Item removed from start of list\n");
    }
    else if(pList->curr == pList->tail){ // removing from tail
        return List_trim(pList);
    }
    else{
        //removing regularly, in between two nodes        
        Node* newCurr = NULL;
        itemRemoved = pList->curr->item;
        newCurr = pList->curr->prev->next = pList->curr->next; //new current is the next node
        pList->curr->next->prev = pList->curr->prev;
        pList->curr->item = pList->curr->next = pList->curr->prev = NULL; 
        pList->curr = newCurr;
        // printf("Item removed from list\n");
    }
    --pList->size;
    --numNodes;
    return itemRemoved;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2){
    assert(pList1!=NULL && pList2!=NULL);
    pList1->tail->next = pList2->head; //point head and tail to each other 
    pList1->head->prev = pList1->head;
    pList2->h_sentinel->next = NULL; //nullify sentinel
   // pList2->head = NULL;
    pList1->size += pList2->size; //counts added
    pList1->tail = pList2->tail;
    pList1->t_sentinel->prev = pList1->tail; //sentinel points to new tail
    pList2->size = 0;
    pList2->freeNodeIdx = 0;
    pList2->nextFreeNodeIdx = 1;
    pList2->tail = pList2->head = &nodes[pList2->index*LIST_MAX_NUM_HEADS]; // make them point back to the start

}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
// UPDATED: Changed function pointer type, May 19
void List_free(List* pList, FREE_FN pItemFreeFn){
    assert(pList!=NULL);
    Node* t;
    pList-> curr = pList->head;
    void* removeReturn;
    t = pList->curr; //make temporary current 
    while(t!=pList->t_sentinel && t->item!=NULL){
        pItemFreeFn(t->item); // use function pointer to free
        removeReturn = List_remove(pList); // remove from list
        if(removeReturn == NULL){ // if null, we're out of things to remove, break out of the loop
            break;
        }
        t  = t->next;
    }
    pList->tail = pList->head = &nodes[pList->index*LIST_MAX_NUM_HEADS]; 
    --numLists; 
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
    assert(pList!=NULL);
    void* itemRemoved;
    pList->curr = pList->tail;
    if(pList->curr == pList->h_sentinel || pList->curr == pList->t_sentinel){
        // printf("Pointer beyond end or before start of pList\n");
        return NULL;
    }
    else if(numNodes<=0){
        // printf("No nodes left to remove");
        return NULL;
    }
    else{ //carefully remove from tail and preserve sentinel
        itemRemoved = pList->curr->item;
        pList->tail = pList->curr->prev;
        pList->curr->item = pList->curr->prev = NULL;
        pList->curr->next->prev = pList->tail;
        pList->curr->next = pList->t_sentinel;
        pList->curr = pList->tail;
        // printf("Item removed from end of list\n");
    }
     --pList->size;
    --numNodes;
    return itemRemoved;
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// UPDATED: Added clarification of behaviour May 19
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    assert(pList->curr!=NULL);
    assert(pComparator!=NULL);
    assert(pComparisonArg!=NULL);
     while(pList->curr->index!=-5){
        if(pComparator(pList->curr->item,pComparisonArg)){
            return pList->curr->item;
        }
        else{
            pList->curr = pList->curr->next;
        }
    }
    return NULL;
} 
