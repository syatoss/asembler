#include <stdlib.h>
#include <stdio.h>
#include "../headers/str_linked_list.h"
#include "../headers/utils.h"

str_linked_list* new_str_linked_list() {
    str_linked_list* list = malloc(sizeof(str_linked_list));
    str_node* head;
    if(!list) {
        SYS_MEM_FAIL_EXIT(1)
    }
    head = NULL;
    list->head = head;
    list->last_node = head;
    return list;
}

str_node* get_next_node(str_node* node) {
    return (*node).next;
}

str_node* get_nth_node(str_linked_list ll, int n) {
    str_node* head_proxy = ll.head;
    while(head_proxy && n--)
        head_proxy = get_next_node(head_proxy);
    return head_proxy;
}

void add_node(str_linked_list* ll, str_node* node) {
    str_linked_list ll_proxy = (*ll);
    ll_proxy.last_node->next = node;
    ll_proxy.last_node = node;
}

str_node* get_new_node(char* str) {
    str_node* node = malloc(sizeof(str_node));
    if(!node) {
        printf("Memmory allocation failure");
        exit(1);
    }
    node->string = str;
    node->next = NULL;
    return node;
}

void free_node(str_node* node) {
    if(!node) return;
    free(node->string);
    free(node);
}

void clear_linked_list(str_linked_list* ll) {
    str_linked_list ll_proxy = (*ll);
    str_node* next = (*ll).head;
    if(!ll) return;
    while(ll_proxy.head) {
        next = ll_proxy.head->next;
        free_node(ll_proxy.head);
        ll_proxy.head = next;
    }
    ll_proxy.head = NULL;
    ll_proxy.last_node = NULL;
    free(ll);
}
