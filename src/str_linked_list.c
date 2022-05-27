#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../headers/str_linked_list.h"
#include "../headers/utils.h"

StrList* new_str_linked_list() {
    StrNode* head;
    StrList* list = (StrList*)malloc(sizeof(StrList));
    if(!list) SYS_MEM_FAIL_EXIT(1)
    head = NULL;
    list->head = head;
    list->last_node = head;
    return list;
}

StrNode* get_next_node(StrNode* node) {
    return (*node).next;
}

StrNode* get_nth_node(StrList ll, int n) {
    StrNode* head_proxy = ll.head;
    while(head_proxy && n--)
        head_proxy = get_next_node(head_proxy);
    return head_proxy;
}

void add_node(StrList* ll, StrNode* node) {
    if(ll->head == NULL) {
        init_list_with_node(ll,node);
        return;
    }
    ll->last_node->next = node;
    ll->last_node = node;
}

void init_list_with_node(StrList* ll, StrNode* node) {
    ll->head = node;
    ll->last_node = node;
}

StrNode* get_new_node(char* str) {
    StrNode* node = (StrNode*)malloc(sizeof(StrNode));
    if(!node) SYS_MEM_FAIL_EXIT(1);
    node->string = (char*)malloc(sizeof(char) * (strlen(str) + 1));
    node->string = strcpy(node->string,str);
    node->next = NULL;
    return node;
}

void free_node(StrNode* node) {
    if(!node) return;
    free(node->string);
    free(node);
}

void clear_linked_list(StrList* ll) {
    StrList ll_proxy;
    StrNode* next;
    if(!ll) return;
    ll_proxy = (*ll);
    while(ll_proxy.head) {
        next = ll_proxy.head->next;
        free_node(ll_proxy.head);
        ll_proxy.head = next;
    }
    free(ll);
}
