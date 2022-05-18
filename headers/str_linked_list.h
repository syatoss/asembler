typedef struct str_node{
    char* string;
    struct str_node* next;
} str_node;

typedef struct {
    str_node* head;
    str_node* last_node;
} str_linked_list;

str_linked_list* new_str_linked_list();
str_node* get_next_node(str_node*);
str_node* get_nth_node(str_linked_list, int);
void init_list_with_node(str_linked_list* , str_node* );
void add_node(str_linked_list*, str_node*);
str_node* get_new_node(char*);
void free_node(str_node*);
void clear_linked_list(str_linked_list*);
