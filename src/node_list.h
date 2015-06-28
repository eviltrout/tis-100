#ifndef _NODE_LIST_H
#define _NODE_LIST_H

#include "node.h"

typedef struct _NodeList {
  Node *node;
  struct _NodeList *next;
} NodeList;

NodeList * node_list_append(NodeList *list, Node *n);
void node_list_clean(NodeList *list, int free_nodes);

#define for_each_list(item, list) \
        for(NodeList *item = list; item != NULL; item = item->next)

#endif
