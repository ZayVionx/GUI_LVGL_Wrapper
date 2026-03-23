/****************************************************************************
*  Copyright 2025 ZJY (Email:3102196558@qq.com)                             *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

#ifndef __EMB_LIST_H__
#define __EMB_LIST_H__

/*================================= INCLUDES =================================*/
#include "gui_lv_utils.h"

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/
#ifndef NULL
#   define NULL       ((void *)0)
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
typedef unsigned long emb_size_t;
#define emb_offsetof(TYPE, MEMBER)          ((emb_size_t)&(((TYPE *)0)->MEMBER))

/** @brief  Container-of macro for list nodes. */
#define EMB_LIST_ENTRY(ptr, type, member)                                      \
    ((type *)((char *)(ptr) - emb_offsetof(type, member)))

#define EMB_LIST_CHECK_NODE(node)                                              \
    do {                                                                       \
        GUI_LV_ASSERT((node) != NULL);                                         \
        GUI_LV_ASSERT((node)->prev != NULL);                                   \
        GUI_LV_ASSERT((node)->next != NULL);                                   \
    } while (0)

/*================================== TYPES ===================================*/
/**
 * @brief   Intrusive doubly-linked list node (self-contained in .c).
 * @note    Naming follows emb_list convention with emb_list_ prefix.
 */
typedef struct emb_list_t {
    struct emb_list_t *prev; 
    struct emb_list_t *next;
} emb_list_t;


/*================================ PROTOTYPES ================================*/
/** @brief  Initialize a list head (points to itself = empty). */
static inline void emb_list_init(emb_list_t *list)
{
    GUI_LV_ASSERT(list != NULL);
    list->prev = list;
    list->next = list;
}

/** @brief  Check if list is empty. */
static inline bool emb_list_is_empty(const emb_list_t *head)
{
    GUI_LV_ASSERT(head != NULL);
    return (head->next == head);
}

/** @brief  Insert ptNew before ptAt (add_tail: ptAt = ptHead). */
static inline void emb_list_add_tail(emb_list_t *list, emb_list_t *head)
{
    EMB_LIST_CHECK_NODE(head);
    GUI_LV_ASSERT(list != NULL);
    list->next         = head;
    list->prev         = head->prev;
    head->prev->next   = list;
    head->prev         = list;
}

/** @brief  Remove ptNode from its list. */
static inline void emb_list_del(emb_list_t *list)
{
    EMB_LIST_CHECK_NODE(list);
    list->prev->next = list->next;
    list->next->prev = list->prev;
    list->next = list;
    list->prev = list;
}

/** @brief  Get the last (tail) node, or NULL if empty. */
static inline emb_list_t *emb_list_get_tail(const emb_list_t *head)
{
    if(emb_list_is_empty(head)) return NULL;
    return head->prev;
}

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LIST_H__ */
