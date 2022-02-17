#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    element_t *elm, *tmp;
    if (l) {
        list_for_each_entry_safe (elm, tmp, l, list) {
            list_del(&elm->list);
            free(elm->value);
            free(elm);
        }
        free(l);
    }
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *elm = malloc(sizeof(element_t));
    if (!elm) {
        return false;
    }
    size_t len = strlen(s) + 1;
    char *val = malloc(sizeof(char) * len);
    if (!val) {
        free(elm);
        return false;
    }
    memcpy(val, s, len);
    elm->value = val;
    list_add(&elm->list, head);
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *elm = malloc(sizeof(element_t));
    if (!elm) {
        return false;
    }
    size_t len = strlen(s) + 1;
    char *val = malloc(sizeof(char) * len);
    if (!val) {
        free(elm);
        return false;
    }
    memcpy(val, s, len);
    elm->value = val;
    list_add_tail(&elm->list, head);
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return target element.
 * Return NULL if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * REF:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *elm = list_first_entry(head, element_t, list);
    head->next->next->prev = head;
    head->next = head->next->next;
    if (sp) {
        memcpy(sp, elm->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    return elm;
}

/*
 * Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *elm = list_last_entry(head, element_t, list);
    head->prev->prev->next = head;
    head->prev = head->prev->prev;
    if (sp) {
        memcpy(sp, elm->value, bufsize);
        sp[bufsize - 1] = '\0';
    }
    return elm;
}

/*
 * WARN: This is for external usage, don't modify it
 * Attempt to release element.
 */
void q_release_element(element_t *e)
{
    free(e->value);
    free(e);
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(struct list_head *head)
{
    return -1;
}

/*
 * Delete the middle node in list.
 * The middle node of a linked list of size n is the
 * ⌊n / 2⌋th node from the start using 0-based indexing.
 * If there're six element, the third member should be return.
 * Return true if successful.
 * Return false if list is NULL or empty.
 */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head)) {
        return false;
    }
    struct list_head *slow = head->next;
    for (struct list_head *fast = head->next;
         fast != head && fast->next != head; fast = fast->next->next) {
        slow = slow->next;
    }
    slow->prev->next = slow->next;
    slow->next->prev = slow->prev;
    element_t *del = list_entry(slow, element_t, list);
    list_del(slow);
    free(del->value);
    free(del);
    return true;
}

/*
 * Delete all nodes that have duplicate string,
 * leaving only distinct strings from the original list.
 * Return true if successful.
 * Return false if list is NULL.
 *
 * Note: this function always be called after sorting, in other words,
 * list is guaranteed to be sorted in ascending order.
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head) {
        return false;
    }
    struct list_head *node, *tmp;
    bool has_dup = false;
    list_for_each_safe (node, tmp, head) {
        list_del(node);
        if (node->next != head &&
            !strcmp(list_entry(node, element_t, list)->value,
                    list_entry(node->next, element_t, list)->value)) {
            has_dup = true;
            element_t *del = list_entry(node, element_t, list);
            list_del(node);
            free(del->value);
            free(del);
        }
        if (has_dup) {
            has_dup = false;
            element_t *del = list_entry(node, element_t, list);
            list_del(node);
            free(del->value);
            free(del);
        }
    }
    return true;
}

/*
 * Attempt to swap every two adjacent nodes.
 */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    struct list_head *node;
    list_for_each (node, head) {
        if (node->next == head) {
            break;
        }
        struct list_head *tmp = node->next;
        list_del(node);
        list_add(node, tmp);
    }
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head)
{
    struct list_head *node, *tmp;
    list_for_each_safe (node, tmp, head) {
        struct list_head *prev = node->prev;
        node->prev = node->next;
        node->next = prev;
    }
    tmp = head->next;
    head->next = head->prev;
    head->prev = tmp;
}

void merge(struct list_head *head_a,
           struct list_head *head_b,
           struct list_head *sorted)
{
    if (list_empty(head_a)) {
        list_splice_tail(head_b, sorted);
        return;
    }
    if (list_empty(head_b)) {
        list_splice_tail(head_a, sorted);
        return;
    }

    while (!list_empty(head_a) && !list_empty(head_b)) {
        char *lv = list_entry(head_a->next, element_t, list)->value;
        char *rv = list_entry(head_b->next, element_t, list)->value;
        struct list_head *tmp =
            strcmp(lv, rv) <= 0 ? head_a->next : head_b->next;
        list_del(tmp);
        list_add_tail(tmp, sorted);
    }
    list_splice_tail(list_empty(head_a) ? head_b : head_a, sorted);
}

void merge_sort_list(struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head)) {
        return;
    }
    struct list_head *fast = head->next, *slow;
    list_for_each (slow, head) {
        if (fast->next == head || fast->next->next == head) {
            break;
        }
        fast = fast->next->next;
    }
    LIST_HEAD(sorted);
    LIST_HEAD(left);
    list_cut_position(&left, head, slow);
    merge_sort_list(&left);
    merge_sort_list(head);
    merge(&left, head, &sorted);
    INIT_LIST_HEAD(head);
    list_splice_tail(&sorted, head);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(struct list_head *head)
{
    if (!head || q_size(head) < 2) {
        return;
    }
    merge_sort_list(head);
}
