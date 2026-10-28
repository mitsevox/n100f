#ifndef XLINKLIST_H
#define XLINKLIST_H

#include "types.h"

struct st_ENODE
{
    st_ENODE* next;
    st_ENODE* prev;
};

struct st_ELIST
{
    st_ENODE* head;
    st_ENODE* tail;
    st_ENODE* tailprev;
};

void MoveList(st_ELIST* to, st_ELIST* from);
st_ENODE* RemHead(st_ELIST* list);
void AddTail(st_ELIST* list, st_ENODE* node);
void AddHead(st_ELIST* list, st_ENODE* node);
void NewList(st_ELIST* list);

#endif
