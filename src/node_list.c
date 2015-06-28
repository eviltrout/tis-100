#include <stdlib.h>
#include "node_list.h"

NodeList * node_list_append(NodeList *list, Node *n) {
  NodeList *tail = (NodeList *) malloc(sizeof(NodeList));
  tail->node = n;
  tail->next = NULL;

  if (!list) { return tail; }

  NodeList *head = list;
  while (head->next != NULL) {
    head = head->next;
  }
  head->next = tail;
  return list;
}

void node_list_clean(NodeList *list, int free_nodes) {
  while (list) {
    if (free_nodes) { free(list->node); }

    NodeList *ref = list;
    list = list->next;
    free(ref);
  }
}
