typedef struct StrNode{
    char* string;
    struct StrNode* next;
} StrNode;

typedef struct {
    StrNode* head;
    StrNode* last_node;
} StrList;

StrList* new_str_linked_list();
StrNode* get_next_node(StrNode*);
StrNode* get_nth_node(StrList, int);
void init_list_with_node(StrList* , StrNode* );
void add_node(StrList*, StrNode*);
StrNode* get_new_node(char*);
void free_node(StrNode*);
void clear_linked_list(StrList*);
