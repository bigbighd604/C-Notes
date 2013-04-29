#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  const char *data;
  struct node *prev;
  struct node *next;
} node_t;


typedef struct list {
  node_t *first;
  node_t *last;
} list_t;


list_t *create_double_linked_list() {
  list_t *list;
  list = (list_t *)malloc(sizeof(list_t) * 1);
  list->first = NULL;
  list->last = NULL;
  return list;

}


void destroy_double_linked_list(list_t *list) {
  node_t *cursor_node;
  node_t *data_node;
  cursor_node = list->first;
  while (cursor_node) {
    data_node = cursor_node;
    cursor_node = cursor_node->next;
    printf("Destroyed Node: %s\n", data_node->data);
    free(data_node);
  }
  free(list);
}


void append(list_t *list, const char *str, int preappend) {
  node_t *cursor_node;
  node_t *new_node;
  new_node = malloc(sizeof(node_t) * 1);
  new_node->data = str;

  if (list->last == NULL) {
    new_node->prev = NULL;
    new_node->next = NULL;
    list->first = new_node;
    list->last = new_node;
  } else {
    if (preappend) {
      cursor_node = list->first;
      new_node->next = cursor_node;
      cursor_node->prev = new_node;
      new_node->prev = NULL;
      list->first = new_node;
    } else {
      cursor_node = list->last;
      new_node->prev = cursor_node;
      cursor_node->next = new_node;
      new_node->next = NULL;
      list->last = new_node;
    }
  }
}


int delete_node(list_t *list, node_t *node) {
  int status = -1;
  node_t *temp_node = list->first;
  while (temp_node != NULL) {
    node_t *next_node = temp_node->next;
    node_t *prev_node = temp_node->prev;
    if (strcmp(node->data, temp_node->data) == 0) {
      if (next_node == NULL) {
        if (prev_node != NULL) {
          prev_node->next = NULL;
          list->last = prev_node;
        } else {
          list->first = list->last = NULL;
        }
      } else {
        if (prev_node != NULL) {
          prev_node->next = next_node;
          next_node->prev = prev_node;
        } else {
          next_node->prev = NULL;
          list->first = next_node;
        }
      }
      free(temp_node);
      status = 1;
    }
    temp_node = next_node;
  }
  return status;
}

void print_list(list_t *list) {
  int counter = 1;
  node_t *temp_node = list->first;

  if (temp_node == NULL) {
    printf("Empty double linked list, exit...\n");
  }

  for (temp_node;
       temp_node != NULL;
       temp_node = temp_node->next) {
    printf("Node %d: %s\n", counter, temp_node->data);
    counter++;
  }
  printf("\n");
}


int main(int argc, char *argv[]) {
  char *nodes[10] = {"Node 1",
    "Node 2",
    "Node 3",
    "Node 4",
    "Node 5",
    "Node 6",
    "Node 7",
    "Node 8",
    "Node 9",
    "Node 10"};
  int i;

  list_t *list = create_double_linked_list();
  print_list(list);
  // append
  for (i=0; i < 10; i++) {
    append(list, nodes[i], 0);
  }
  print_list(list);
  // preappend
  for (i=0; i < 10; i++) {
    append(list, nodes[i], 1);
  }
  print_list(list);


  // delete a node
  node_t node = {"Node 1", NULL, NULL};
  delete_node(list, &node);
  print_list(list);

  // destroy a list
  destroy_double_linked_list(list);

}
