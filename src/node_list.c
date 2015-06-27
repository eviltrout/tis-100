#include <stdlib.h>
#include "node_list.h"

NodeList * node_list_append(NodeList *list, Node *n) {
  NodeList *head = (NodeList *) malloc(sizeof(NodeList));
  head->node = n;
  head->prev = list;
  return head;
}

void node_list_clean(NodeList *list, int free_nodes) {
  while (list) {
    if (free_nodes) { free(list->node); }

    NodeList *ref = list;
    list = list->prev;
    free(ref);
  }
}
