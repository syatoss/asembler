#ifndef STR_LINKED_LIST_H
#define STR_LINKED_LIST_H

typedef struct StrNode {
  char *string;
  struct StrNode *next;
} StrNode;

typedef struct {
  StrNode *head;
  StrNode *last_node;
} StrList;

/* allocates memory for StrList and assings default values, returns pointer to
 * allocated memory */
StrList *new_str_linked_list();
/* retuns pointer to the next node in the linked list */
StrNode *get_next_node(StrNode *);
/* retuns pointer to the nth next node in the linked list */
StrNode *get_nth_node(StrList, int);
/* assinges the node to the lists head */
void init_list_with_node(StrList *, StrNode *);
void add_node(StrList *, StrNode *);
/* allocates memory for StrNode and assings default values, returns pointer to
 * allocated memory */
StrNode *get_new_node(char *);
/* frees the pointer and all assosiated allocated memory */
void free_node(StrNode *);
/* frees the pointer and all assosiated allocated memory */
void clear_linked_list(StrList *);

#endif
