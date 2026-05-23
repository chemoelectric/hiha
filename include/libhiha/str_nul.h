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

#ifndef __LIBHAHA__STR_NUL_H__INCLUDED__
#define __LIBHAHA__STR_NUL_H__INCLUDED__

/* NUL-terminated strings of char. */

#include <string.h>
#include <libhiha/persistent_vector.h>

/*--------------------------------------------------------------------*/

inline bool
str_nul_equal (const char *a, const char *b)
{
  const bool a_is_null = (a == NULL);
  const bool b_is_null = (b == NULL);
  return ((a_is_null && b_is_null)
          || (!a_is_null && !b_is_null && strcmp (a, b) == 0));
}

/*--------------------------------------------------------------------*/

struct str_nul_keyval
{
  const char *key;
  const void *value;
};
typedef const struct str_nul_keyval *str_nul_keyval_t;

DECLARE_HIHA_PERSISTENT_VECTOR_DATATYPE (, str_nul_vector,
                                         const char *, 5);
DECLARE_HIHA_PERSISTENT_VECTOR_DATATYPE (, str_nul_keyval_vector,
                                         str_nul_keyval_t, 5);

/*--------------------------------------------------------------------*/

struct str_nul_hash_context;
typedef struct str_nul_hash_context *str_nul_hash_context_t;

str_nul_hash_context_t str_nul_hash_init (const char *);
uint64_t str_nul_hash (str_nul_hash_context_t context, unsigned int i);

/*--------------------------------------------------------------------*/
/* Persistent unordered maps. */

struct str_nul_map;
typedef const struct str_nul_map *str_nul_map_t;

size_t str_nul_map_size (str_nul_map_t map);

const void *str_nul_map_search (str_nul_map_t map, const char *key);

str_nul_map_t str_nul_map_insert_or_replace (str_nul_map_t map,
                                             const char *key,
                                             const void *value);
str_nul_map_t str_nul_map_insert_only (str_nul_map_t map,
                                       const char *key,
                                       const void *value);
str_nul_map_t str_nul_map_replace_only (str_nul_map_t map,
                                        const char *key,
                                        const void *value);

str_nul_map_t str_nul_map_delete (str_nul_map_t map, const char *key);

str_nul_vector_t str_nul_map_keys (str_nul_map_t map);
voidp_vector_t str_nul_map_values (str_nul_map_t map);
str_nul_keyval_vector_t str_nul_map_associations (str_nul_map_t map);

/*--------------------------------------------------------------------*/

#endif /* __LIBHAHA__STR_NUL_H__INCLUDED__ */

/*
  local variables:
  mode: c
  coding: utf-8
  end:
*/
