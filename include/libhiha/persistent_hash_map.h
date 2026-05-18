/*

  The hiha programming language

  Copyright © 2026 Barry Schwartz

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#ifndef __LIBHAHA__PERSISTENT_HASH_MAP_H__INCLUDED__
#define __LIBHAHA__PERSISTENT_HASH_MAP_H__INCLUDED__

/*

  Ideal hash maps.

*/

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#ifndef HIHA_HASH_MAP_ALLOC
#include <xalloc.h>
#define HIHA_HASH_MAP_ALLOC(T) XMALLOC (T)
#endif

#define HIHA_HASH_MAP_NODE_DECL(NAME)           \
  typedef struct NAME                           \
  {                                             \
    bool is_leaf;                               \
  } *NAME##_t

#define HIHA_HASH_MAP_INTERNAL_DECL(NAME)       \
  typedef struct NAME##_internal                \
  {                                             \
    bool is_leaf;                               \
    struct NAME *left;                          \
    struct NAME *right;                         \
  } *NAME##_internal_t

#define HIHA_HASH_MAP_LEAF_DECL(NAME, ELEMTYPE) \
  typedef struct NAME##_leaf                    \
  {                                             \
    bool is_leaf;                               \
    ELEMTYPE element;                           \
  } *NAME##_leaf_t

#define HIHA_HASH_MAP_NODES_DECL(NAME, ELEMTYPE)        \
  HIHA_HASH_MAP_NODE_DECL (NAME);                       \
  HIHA_HASH_MAP_INTERNAL_DECL (NAME);                   \
  HIHA_HASH_MAP_LEAF_DECL (NAME, ELEMTYPE)

#define HIHA_HASH_MAP_MAKE_INTERNAL(NEW_NODE, NAME, LEFT, RIGHT)        \
  do                                                                    \
    {                                                                   \
      struct NAME##_internal *_NEW_ND__ =                               \
        HIHA_HASH_MAP_ALLOC (struct NAME##_internal);                   \
      _NEW_ND__->is_leaf = false;                                       \
      _NEW_ND__->left = (LEFT);                                         \
      _NEW_ND__->right = (RIGHT);                                       \
      NEW_NODE = (NAME##_t) _NEW_ND__;                                  \
    }                                                                   \
  while (0)

#define HIHA_HASH_MAP_MAKE_LEAF(NEW_NODE, NAME, ELEMENT)        \
  do                                                            \
    {                                                           \
      struct NAME##_leaf *_NEW_ND__ =                           \
        HIHA_HASH_MAP_ALLOC (struct NAME##_leaf);               \
      _NEW_ND__->is_leaf = true;                                \
      _NEW_ND__->element = (ELEMENT);                           \
      NEW_NODE = (NAME##_t) _NEW_ND__;                          \
    }                                                           \
  while (0)

#define HIHA_HASH_MAP_SEARCH(SOUGHT_NODE, NAME, ELEMTYPE,       \
                             NODE, KEY, CONTEXT, HASHBIT,       \
                             EQUALS)                            \
  do                                                            \
    {                                                           \
      NAME##_t _SOUGHT_ND__ = (NODE);                           \
      const ELEMTYPE *_KEY__ = (KEY);                           \
      uint64_t _BIT_NUMBER__ = 0;                               \
      while (_SOUGHT_ND__ != NULL && !_SOUGHT_ND__->is_leaf)    \
        {                                                       \
          _SOUGHT_ND__ =                                        \
            (((HASHBIT) (CONTEXT, _BIT_NUMBER__))               \
             ? ((NAME##_internal_t) _SOUGHT_ND__)->left         \
             : ((NAME##_internal_t) _SOUGHT_ND__)->right);      \
          _BIT_NUMBER__ += 1;                                   \
        }                                                       \
      if (_SOUGHT_ND__ != NULL)                                 \
        if (!((EQUALS)                                          \
              (_KEY__,                                          \
               &((NAME##_leaf_t) _SOUGHT_ND__)->element)))      \
          _SOUGHT_ND__ = NULL;                                  \
      SOUGHT_NODE = (NAME##_leaf_t) _SOUGHT_ND__;               \
    }                                                           \
  while (0)

/* Search for a matching element. Return a pointer to it if found,
   NULL if not found. */
#define HIHA_HASH_MAP_SEARCH_DEFN(FUNC, NAME, ELEMTYPE,         \
                                  HASHINIT, HASHBIT, EQUALS)    \
  const ELEMTYPE *                                              \
  FUNC (NAME##_t _Node, const ELEMTYPE *_Key)                   \
  {                                                             \
    void *__context_ = (HASHINIT) (_Key);                       \
    NAME##_leaf_t __sought_node_;                               \
    HIHA_HASH_MAP_SEARCH (__sought_node_, NAME, ELEMTYPE,       \
                          _Node, _Key, __context_,              \
                          (HASHBIT), (EQUALS));                 \
    return ((__sought_node_ == NULL)                            \
            ? NULL                                              \
            : &__sought_node_->element);                        \
  }

#endif /* __LIBHAHA__PERSISTENT_HASH_MAP_H__INCLUDED__ */

/*
  local variables:
  mode: c
  coding: utf-8
  end:
*/
