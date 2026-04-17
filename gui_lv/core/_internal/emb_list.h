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
/** 
 * @brief  Initialize a list head (points to itself = empty). 
 * @param[in] list  The list head to initialize
 */
static inline 
void emb_list_init(emb_list_t *list)
{
    GUI_LV_ASSERT(list != NULL);
    list->prev = list;
    list->next = list;
}

/** 
 * @brief  Check if list is empty. 
 * @param[in] head  The list head
 * @return true if the list is empty, false otherwise
 */
static inline 
bool emb_list_is_empty(const emb_list_t *head)
{
    GUI_LV_ASSERT(head != NULL);
    return (head->next == head);
}

/**
 * @brief  Check if a node is detached (not in any list).
 * @param[in] node  The node to check
 */
static inline 
bool emb_list_is_detached(const emb_list_t *node)
{
    GUI_LV_ASSERT(node != NULL);
    return (node->next == node) && (node->prev == node);
}

/**
 * @brief  Add a new node after the specified head (add to front).
 * @param[in] list  The new node to add (must be detached)
 */
static inline 
void emb_list_add(emb_list_t *list, emb_list_t *head)
{
    EMB_LIST_CHECK_NODE(head);
    GUI_LV_ASSERT(list != NULL);
    GUI_LV_ASSERT(emb_list_is_detached(list));
    list->next       = head->next;
    list->prev       = head;
    head->next->prev = list;
    head->next       = list;
}

/** 
 * @brief  Insert ptNew before ptAt (add_tail: ptAt = ptHead).
 * @param[in] list  The new node to add (must be detached)
 * @param[in] head  The list head
 */
static inline 
void emb_list_add_tail(emb_list_t *list, emb_list_t *head)
{
    EMB_LIST_CHECK_NODE(head);
    GUI_LV_ASSERT(list != NULL);
    GUI_LV_ASSERT(emb_list_is_detached(list));
    list->next         = head;
    list->prev         = head->prev;
    head->prev->next   = list;
    head->prev         = list;
}

/** 
 * @brief  Remove ptNode from its list. 
 * @param[in] list  The node to remove
 */
static inline 
void emb_list_del(emb_list_t *list)
{
    EMB_LIST_CHECK_NODE(list);
    list->prev->next = list->next;
    list->next->prev = list->prev;
    list->next       = list;
    list->prev       = list;
}

/** 
 * @brief  Get the last (tail) node, or NULL if empty.
 * @param[in] head  The list head
 * @return The last node, or NULL if the list is empty
 */
static inline 
emb_list_t *emb_list_get_tail(const emb_list_t *head)
{
    if(emb_list_is_empty(head)) return NULL;
    return head->prev;
}


/*============================ ITERATION HELPERS =============================*/
/**
 * @brief  遍历链表
 *
 * @param[in,out] pos   遍历时用于保存当前位置的指针（类型为 emb_list_t *）
 * @param[in]     head  链表头
 */
#define emb_list_for_each(pos, head)                                           \
    for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

/**
 * @brief  安全遍历链表（遍历过程中允许删除当前节点）
 *
 * @param[in,out] pos   当前节点指针
 * @param[in,out] n     临时节点指针，用于保存下一个节点
 * @param[in]     head  链表头
 */
#define emb_list_for_each_safe(pos, n, head)                                   \
    for ((pos) = (head)->next, (n) = (pos)->next;                              \
         (pos) != (head);                                                      \
         (pos) = (n), (n) = (pos)->next)

/**
 * @brief  遍历包含 emb_list_t 成员的外层结构体（需要 typeof/decltype 支持）。
 *         使用前请声明 pos 为指向外层结构体的指针。
 */
#define emb_list_for_each_entry(pos, head, member)                             \
    for ((pos) = EMB_LIST_ENTRY((head)->next, EMB_TYPEOF(*(pos)), member);     \
         &((pos)->member) != (head);                                           \
         (pos) = emb_list_next_entry(pos, member))

/**
 * @brief  支持遍历过程中安全删除节点的版本。
 */
#define emb_list_for_each_entry_safe(pos, n, head, member)                     \
    for ((pos) = EMB_LIST_ENTRY((head)->next, EMB_TYPEOF(*(pos)), member),     \
         (n) = emb_list_next_entry(pos, member);                               \
         &((pos)->member) != (head);                                           \
         (pos) = (n), (n) = emb_list_next_entry(n, member))

/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __EMB_LIST_H__ */
