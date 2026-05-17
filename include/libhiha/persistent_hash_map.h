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

#include <stdint.h>
#include <libhiha/persistent_integer_trie.h>

#ifndef HIHA_HASH_MAP_ALLOC
#include <xalloc.h>
#define HIHA_HASH_MAP_ALLOC(T) XMALLOC (T)
#endif

#define HIHA_HASH_MAP_DECL(NAME, VALTYPE)                       \
                                                                \
  struct NAME##_node;                                           \
                                                                \
  union NAME##_node_union                                       \
  {                                                             \
    VALTYPE value;                                              \
    struct NAME##_node *trie;                                   \
  };                                                            \
                                                                \
  struct NAME##_node_contents                                   \
  {                                                             \
    union NAME##_node_union u;                                  \
    bool is_trie;                                               \
  };                                                            \
                                                                \
  HIHA_INT_TRIE_NODES_DECL (NAME##_node, uint64_t,              \
                            struct NAME##_node_contents);       \
                                                                \
  typedef struct NAME                                           \
  {                                                             \
    struct NAME##_node *trie;                                   \
    size_t size;                                                \
  } *NAME##_t

#define HIHA_HASH_MAP_TRIE_DEFN(NAME)                                   \
  HIHA_INT_TRIE_INSERT_DEFN (NAME##_node_insert, NAME##_node,           \
                             uint64_t, struct NAME##_node_contents);    \
  HIHA_INT_TRIE_SEARCH_DEFN (NAME##_node_search, NAME##_node,           \
                             uint64_t);                                 \
  HIHA_INT_TRIE_DELETE_DEFN (NAME##_node_delete, NAME##_node,           \
                             uint64_t);                                 \
  HIHA_INT_TRIE_WALK_DEFN (NAME##_node_walk, NAME##_node)

#define HIHA_HASH_MAP_SIZE_DEFN(FUNC, NAME)     \
  size_t                                        \
  FUNC (NAME##_t hmap)                          \
  {                                             \
    return (hmap == NULL) ? 0 : hmap->size;     \
  }

#define HIHA_HASH_MAP_SEARCH_DEFN(FUNC, NAME, VALTYPE,                  \
                                  HASHINIT, HASHFUNC, EQUALS)           \
                                                                        \
  const VALTYPE *                                                       \
  FUNC##_9c336bbf_7aba_427b_a598_1b0686a45eb3                           \
  (NAME##_node_t trie, const VALTYPE *key,                              \
   void *context, unsigned int index)                                   \
  {                                                                     \
    const VALTYPE *result = NULL;                                       \
                                                                        \
    uint64_t hash = (HASHFUNC) (context, index);                        \
    NAME##_node_leaf_t leaf = (NAME##_node_search) (trie, hash);        \
    if (leaf != NULL)                                                   \
      {                                                                 \
        if (leaf->value.is_trie)                                        \
          (FUNC##_9c336bbf_7aba_427b_a598_1b0686a45eb3)                 \
            (leaf->value.u.trie, key, context, index + 1);              \
        else if ((EQUALS) (key, &leaf->value.u.value))                  \
          result = &leaf->value.u.value;                                \
      }                                                                 \
                                                                        \
    return result;                                                      \
  }                                                                     \
                                                                        \
  const VALTYPE *                                                       \
  FUNC (NAME##_t hmap, const VALTYPE *key)                              \
  {                                                                     \
    const VALTYPE *result = NULL;                                       \
    if (hmap != NULL)                                                   \
      result = ((FUNC##_9c336bbf_7aba_427b_a598_1b0686a45eb3)           \
                (hmap->trie, key, ((HASHINIT) ((string_t) key)), 0));   \
    return result;                                                      \
  }

#endif /* __LIBHAHA__PERSISTENT_HASH_MAP_H__INCLUDED__ */

/*
  local variables:
  mode: c
  coding: utf-8
  end:
*/
