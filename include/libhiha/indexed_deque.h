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

#ifndef __LIBHAHA__INDEXED_DEQUE_H__INCLUDED__
#define __LIBHAHA__INDEXED_DEQUE_H__INCLUDED__

/*
  Persistent deques that can be searched by index in constant time.
  Elements can be replaced by index in constant time, as well.
*/

#include <stddef.h>

struct indexed_deque;
typedef struct indexed_deque *indexed_deque_t;

size_t indexed_deque_size (indexed_deque_t dq);

indexed_deque_t indexed_deque_put_before_first (indexed_deque_t dq,
                                                const void *element);
indexed_deque_t indexed_deque_put_after_last (indexed_deque_t dq,
                                              const void *element);

indexed_deque_t indexed_deque_put (indexed_deque_t dq, size_t i,
                                   const void *element);
const void *indexed_deque_get (indexed_deque_t dq, size_t i);

/* The next two return NULL rather than treat it as an error, if there
   is no such entry. */
const void *indexed_deque_get_first (indexed_deque_t dq);
const void *indexed_deque_get_last (indexed_deque_t dq);

indexed_deque_t indexed_deque_delete_first (indexed_deque_t dq);
indexed_deque_t indexed_deque_delete_last (indexed_deque_t dq);

#endif /* __LIBHAHA__INDEXED_DEQUE_H__INCLUDED__ */

/*
  local variables:
  mode: c
  coding: utf-8
  end:
*/
