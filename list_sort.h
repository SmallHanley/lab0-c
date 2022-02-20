#ifndef LIST_SORT_H
#define LIST_SORT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "list.h"

typedef int
    __attribute__((nonnull(2, 3))) (*list_cmp_func_t)(void *,
                                                      const struct list_head *,
                                                      const struct list_head *);

__attribute__((nonnull(2, 3))) void list_sort(void *priv,
                                              struct list_head *head,
                                              list_cmp_func_t cmp);

#endif
