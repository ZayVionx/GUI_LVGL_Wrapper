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

/*============================ BASIC TYPES ===================================*/
/* 用于偏移计算的无符号整型，避免依赖 size_t */
typedef unsigned long emb_size_t;

/*============================ MACROS ========================================*/
/**
 * @brief  计算成员在结构体中的偏移量（类似标准 offsetof）
 */
#define emb_offsetof(TYPE, MEMBER)  ((emb_size_t)&(((TYPE *)0)->MEMBER))

/**
 * @brief  基础节点校验，保证 prev/next 指针非空。
 */
#define EMB_LIST_CHECK_NODE(node)                                              \
    do {                                                                       \
        GUI_LV_ASSERT((node) != NULL);                                         \
        GUI_LV_ASSERT((node)->prev != NULL);                                   \
        GUI_LV_ASSERT((node)->next != NULL);                                   \
    } while (0)

/*============================ TYPEOF SUPPORT ===============================*/
#if defined(__cplusplus)
#define EMB_TYPEOF(expr) decltype(expr)
#elif defined(__GNUC__) || defined(__clang__)
#define EMB_TYPEOF(expr) __typeof__(expr)
#else
#define EMB_TYPEOF(expr) typeof(expr)
#endif

/**
 * @brief  根据成员指针获取外层结构体指针（与 Linux container_of 类似）
 *         同时借助 typeof/decltype 做静态类型校验。
 */
#if defined(__GNUC__) || defined(__clang__)
#define emb_container_of(ptr, type, member)                                    \
    __extension__ ({                                                           \
        const EMB_TYPEOF(((type *)0)->member) *__emb_mptr = (ptr);             \
        (type *)((char *)__emb_mptr - emb_offsetof(type, member));             \
    })
#else
#define emb_container_of(ptr, type, member)                                    \
    ((type *)((char *)(ptr) - emb_offsetof(type, member)))
#endif

/**
 * @brief  根据成员指针获取外层结构体指针（兼容旧接口名）。
 */
#define EMB_LIST_ENTRY(ptr, type, member)  emb_container_of(ptr, type, member)

/**
 * @brief  获取链表中的第一个/最后一个节点对应的外层结构体。
 */
#define emb_list_first_entry(head, type, member)                               \
    EMB_LIST_ENTRY((head)->next, type, member)

#define emb_list_last_entry(head, type, member)                                \
    EMB_LIST_ENTRY((head)->prev, type, member)

#define emb_list_first_entry_or_null(head, type, member)                       \
    (emb_list_is_empty(head) ? NULL : emb_list_first_entry(head, type, member))

/**
 * @brief  通过 typeof 自动推导类型的 next/prev 助手。
 */
#define emb_list_next_entry(pos, member)                                       \
    emb_container_of((pos)->member.next, EMB_TYPEOF(*(pos)), member)

#define emb_list_prev_entry(pos, member)                                       \
    emb_container_of((pos)->member.prev, EMB_TYPEOF(*(pos)), member)

/*============================ TYPES =========================================*/
/**
 * @brief 内嵌循环双向链表节点类型
 */
typedef struct emb_list_t {
    struct emb_list_t *prev;   /* 指向上一个节点 */
    struct emb_list_t *next;   /* 指向下一个节点 */
} emb_list_t;

/*============================ PROTOTYPES ====================================*/
/*!
 * \brief  Determine whether the node is in a self-loop state 
 *         of "has been disconnected from the linked list".
 * \param[in] node the node to check
 * 
 * \return true  if the node is detached (in self-loop), 
 *         false if it is linked in a list
 */
static inline 
bool emb_list_is_detached(const emb_list_t *node)
{
    GUI_LV_ASSERT(node != NULL);
    return (node->next == node) && (node->prev == node);
}

/*!
 * \brief  Determine whether the node is linked in a list.
 * \param[in] node the node to check
 * 
 * \return true  if the node is linked in a list, 
 *         false if it is detached
 */
static inline 
bool emb_list_is_linked(const emb_list_t *node)
{
    return !emb_list_is_detached(node);
}

/*!
 * \brief  Initialize a list head or node, making it a self-loop.
 * \param[in] list the list head or node to initialize
 */
static inline 
void emb_list_init(emb_list_t *list)
{
    GUI_LV_ASSERT(list != NULL);
    list->prev = list;
    list->next = list;
}

/*!
 * \brief  Determine whether the list is empty (i.e., head points to itself).
 * \param[in] head the list head to check
 */
static inline 
bool emb_list_is_empty(const emb_list_t *head)
{
    GUI_LV_ASSERT(head != NULL);
    return (head->next == head);
}

/*!
 * \brief  Insert a node after the head (head insertion).
 * \param[in] list the node to insert
 * \param[in] head the list head
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

/*!
 * \brief  Insert a node before the head (tail insertion).
 * \param[in] list the node to insert
 * \param[in] head the list head
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

/*!
 * \brief  Get the last node in the list.
 * \param[in] head the list head
 * \return the last node, or NULL if the list is empty
 */
static inline 
emb_list_t *emb_list_get_tail(const emb_list_t *head)
{
    if(emb_list_is_empty(head)) return NULL;
    return head->prev;
}

/**
 * \brief  Insert a node before the specified position.
 * \param[in] list the node to insert
 * \param[in] position the position before which to insert the node
 */
static inline 
void emb_list_insert_before(emb_list_t *list, emb_list_t *position)
{
    EMB_LIST_CHECK_NODE(position);
    GUI_LV_ASSERT(list != NULL);
    GUI_LV_ASSERT(emb_list_is_detached(list));
    list->prev            = position->prev;
    list->next            = position;
    position->prev->next  = list;
    position->prev        = list;
}

/**
 * \brief  Remove a node from the list and reset it to a self-loop.
 * \param[in] list the node to remove
 */
static inline 
void emb_list_del(emb_list_t *list)
{
    EMB_LIST_CHECK_NODE(list);
    if (emb_list_is_detached(list)) {
        return;
    }
    list->prev->next = list->next;
    list->next->prev = list->prev;
    list->next = list;
    list->prev = list;
}

/**
 * \brief  Move a node to the position after the head.
 * \param[in] list the node to move
 * \param[in] head the list head
 */
static inline 
void emb_list_move(emb_list_t *list, emb_list_t *head)
{
    GUI_LV_ASSERT(head != NULL);
    GUI_LV_ASSERT(emb_list_is_linked(list));
    emb_list_del(list);
    emb_list_add(list, head);
}

/**
 * \brief  Move a node to the position before the head (tail).
 * \param[in] list the node to move
 * \param[in] head the list head
 */
static inline 
void emb_list_move_tail(emb_list_t *list, emb_list_t *head)
{
    GUI_LV_ASSERT(head != NULL);
    GUI_LV_ASSERT(emb_list_is_linked(list));
    emb_list_del(list);
    emb_list_add_tail(list, head);
}

/**
 * \brief  Replace an old node with a new node in place.
 * \param[in] old_list the node to be replaced
 * \param[in] new_list the new node to insert
 */
static inline 
void emb_list_replace(emb_list_t *old_list, emb_list_t *new_list)
{
    EMB_LIST_CHECK_NODE(old_list);
    GUI_LV_ASSERT(new_list != NULL);
    GUI_LV_ASSERT(emb_list_is_linked(old_list));
    GUI_LV_ASSERT(emb_list_is_detached(new_list));
    new_list->next       = old_list->next;
    new_list->prev       = old_list->prev;
    old_list->prev->next = new_list;
    old_list->next->prev = new_list;
    emb_list_init(old_list);
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
 * \brief  Safely iterate over a list 
 *         (allows deletion of the current node during iteration)
 *
 * \param[in,out] pos   current node pointer
 * \param[in,out] n     temporary node pointer to store the next node
 * \param[in]     head  list head
 */
#define emb_list_for_each_safe(pos, n, head)                                   \
    for ((pos) = (head)->next, (n) = (pos)->next;                              \
         (pos) != (head);                                                      \
         (pos) = (n), (n) = (pos)->next)

/**
 * \brief  Iterate over the outer structure containing emb_list_t members 
 *         (requires typeof/decltype support).
 *         Declare pos as a pointer to the outer structure before use.
 */
#define emb_list_for_each_entry(pos, head, member)                             \
    for ((pos) = EMB_LIST_ENTRY((head)->next, EMB_TYPEOF(*(pos)), member);     \
         &((pos)->member) != (head);                                           \
         (pos) = emb_list_next_entry(pos, member))

/**
 * \brief  Safely iterate over the outer structure containing emb_list_t members 
 *         (allows deletion of the current node during iteration, requires typeof/decltype support).
 *         Declare pos as a pointer to the outer structure before use.
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
