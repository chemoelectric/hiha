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

#include <config.h>
#include <xalloc.h>
#include <error.h>
#include <exitfail.h>
#include <libhiha/persistent_integer_trie.h>
#include <libhiha/indexed_deque.h>

#define _(msgid) HIHA_GETTEXT (msgid)

HIHA_INT_TRIE_NODES_DECL (indexed_deque_node, size_t, const void *);
HIHA_INT_TRIE_INSERT_DEFN (indexed_deque_node_insert,
                           indexed_deque_node, size_t, const void *);
HIHA_INT_TRIE_DELETE_DEFN (indexed_deque_node_delete,
                           indexed_deque_node, size_t);
HIHA_INT_TRIE_SEARCH_DEFN (indexed_deque_node_search,
                           indexed_deque_node, size_t);

struct indexed_deque
{
  indexed_deque_node_t _trie;
  size_t _i1;
  size_t _i2;
};

static inline size_t
deque_size (indexed_deque_t dq)
{
  return (dq == NULL) ? 0 : (dq->_i2 - dq->_i1);
}

HIHA_NORETURN static void
deque_index_error (size_t i)
{
  error (exit_failure, 0, "indexed_deque_t index out of range: %zu", i);
  abort ();
}

static inline size_t
deque_index (indexed_deque_t dq, size_t i)
{
  if (deque_size (dq) <= i)
    deque_index_error (i);
  return dq->_i1 + i;
}

static indexed_deque_t
allocate_deque (indexed_deque_node_t trie, size_t i1, size_t i2)
{
  indexed_deque_t result = XMALLOC (struct indexed_deque);
  result->_trie = trie;
  result->_i1 = i1;
  result->_i2 = i2;
  return result;
}

static inline indexed_deque_t
make_deque (indexed_deque_node_t trie, size_t i1, size_t i2)
{
  return (trie == NULL) ? NULL : allocate_deque (trie, i1, i2);
}

static inline indexed_deque_node_t
deque_trie (indexed_deque_t dq)
{
  return (dq == NULL) ? NULL : dq->_trie;
}

static inline size_t
deque_i1 (indexed_deque_t dq)
{
  return (dq == NULL) ? 0 : dq->_i1;
}

static inline size_t
deque_i2 (indexed_deque_t dq)
{
  return (dq == NULL) ? 0 : dq->_i2;
}

HIHA_VISIBLE size_t
indexed_deque_size (indexed_deque_t dq)
{
  return deque_size (dq);
}

HIHA_VISIBLE indexed_deque_t
indexed_deque_put_before_first (indexed_deque_t dq, const void *element)
{
  indexed_deque_node_t trie = deque_trie (dq);
  size_t i1 = deque_i1 (dq);
  size_t i2 = deque_i2 (dq);
  i1 -= 1;
  trie = indexed_deque_node_insert (trie, i1, element);
  return make_deque (trie, i1, i2);
}

HIHA_VISIBLE indexed_deque_t
indexed_deque_put_after_last (indexed_deque_t dq, const void *element)
{
  indexed_deque_node_t trie = deque_trie (dq);
  size_t i1 = deque_i1 (dq);
  size_t i2 = deque_i2 (dq);
  trie = indexed_deque_node_insert (trie, i2, element);
  i2 += 1;
  return make_deque (trie, i1, i2);
}

HIHA_VISIBLE indexed_deque_t
indexed_deque_put (indexed_deque_t dq, size_t i, const void *element)
{
  indexed_deque_node_t trie = deque_trie (dq);
  size_t i1 = deque_i1 (dq);
  size_t i2 = deque_i2 (dq);
  const size_t j = deque_index (dq, i);
  trie = indexed_deque_node_insert (trie, j, element);
  return make_deque (trie, i1, i2);
}

HIHA_VISIBLE const void *
indexed_deque_get (indexed_deque_t dq, size_t i)
{
  const size_t j = deque_index (dq, i);
  return indexed_deque_node_search (dq->_trie, j)->value;
}

HIHA_VISIBLE const void *
indexed_deque_get_first (indexed_deque_t dq)
{
  return ((deque_size (dq) == 0)
          ? NULL
          : indexed_deque_node_search (dq->_trie, dq->_i1)->value);
}

HIHA_VISIBLE const void *
indexed_deque_get_last (indexed_deque_t dq)
{
  return ((deque_size (dq) == 0)
          ? NULL
          : indexed_deque_node_search (dq->_trie, dq->_i2 - 1)->value);
}

HIHA_VISIBLE indexed_deque_t
indexed_deque_delete_first (indexed_deque_t dq)
{
  indexed_deque_node_t trie = deque_trie (dq);
  size_t i1 = deque_i1 (dq);
  size_t i2 = deque_i2 (dq);
  trie = indexed_deque_node_delete (trie, i1);
  i1 += 1;
  return make_deque (trie, i1, i2);
}

HIHA_VISIBLE indexed_deque_t
indexed_deque_delete_last (indexed_deque_t dq)
{
  indexed_deque_node_t trie = deque_trie (dq);
  size_t i1 = deque_i1 (dq);
  size_t i2 = deque_i2 (dq);
  i2 -= 1;
  trie = indexed_deque_node_delete (trie, i2);
  return make_deque (trie, i1, i2);
}

/*
  local variables:
  mode: c
  coding: utf-8
  end:
*/
