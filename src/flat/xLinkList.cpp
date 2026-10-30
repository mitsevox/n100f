#include "xLinkList.h"

// A list header doubles as its own head and tail nodes: the head node is the list itself
// (whose next is head) and the tail node starts at &tail (whose next, tail, is always NULL).
// A node whose next is NULL is therefore the tail marker.
void NewList(st_ELIST* list)
{
    list->head = (st_ENODE*)&list->tail;
    list->tail = NULL;
    list->tailprev = (st_ENODE*)list;
}

void AddHead(st_ELIST* list, st_ENODE* node)
{
    node->next = list->head;
    node->prev = (st_ENODE*)list;
    list->head->prev = node;
    list->head = node;
}

void AddTail(st_ELIST* list, st_ENODE* node)
{
    st_ENODE* pred = list->tailprev;
    if (pred == NULL)
    {
        pred = (st_ENODE*)list;
    }

    node->next = pred->next;
    node->prev = pred;
    pred->next->prev = node;
    pred->next = node;
}

st_ENODE* RemHead(st_ELIST* list)
{
    st_ENODE* node = list->head;
    st_ENODE* succ = node->next;

    if (succ == NULL)
    {
        return NULL;
    }

    succ->prev = node->prev;
    node->prev->next = node->next;
    return node;
}

// Moves every node of from to to, leaving from empty.
void MoveList(st_ELIST* to, st_ELIST* from)
{
    if (from->head->next == NULL)
    {
        NewList(to);
        return;
    }

    to->head = from->head;
    to->tail = from->tail;
    to->tailprev = from->tailprev;
    to->head->prev = (st_ENODE*)to;
    to->tailprev->next = (st_ENODE*)&to->tail;
    NewList(from);
}
