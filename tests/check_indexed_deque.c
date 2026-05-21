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
#include <assert.h>
#include <gc/gc.h>
#include <libhiha/libhiha.h>

HIHA_VISIBLE const char version_etc_copyright[] =
  "Copyright %s %d Barry Schwartz";

int
main (void)
{
  GC_INIT ();

  indexed_deque_t dq = NULL;
  assert (indexed_deque_size (dq) == 0);
  assert (indexed_deque_get_first (dq) == NULL);
  assert (indexed_deque_get_last (dq) == NULL);

  dq = indexed_deque_put_before_first (dq, make_string_t ("one"));
  assert (indexed_deque_size (dq) == 1);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 0),
                             make_string_t ("one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_first (dq),
                             make_string_t ("one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_last (dq),
                             make_string_t ("one")));

  dq = indexed_deque_put_after_last (dq, make_string_t ("two"));
  assert (indexed_deque_size (dq) == 2);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 0),
                             make_string_t ("one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 1),
                             make_string_t ("two")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_first (dq),
                             make_string_t ("one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_last (dq),
                             make_string_t ("two")));

  dq = indexed_deque_put (dq, 0, make_string_t ("zero"));
  assert (indexed_deque_size (dq) == 2);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 0),
                             make_string_t ("zero")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 1),
                             make_string_t ("two")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_first (dq),
                             make_string_t ("zero")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_last (dq),
                             make_string_t ("two")));

  dq = indexed_deque_put (dq, 1, make_string_t ("one"));
  assert (indexed_deque_size (dq) == 2);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 0),
                             make_string_t ("zero")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 1),
                             make_string_t ("one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_first (dq),
                             make_string_t ("zero")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_last (dq),
                             make_string_t ("one")));

  dq = indexed_deque_put_before_first (dq, make_string_t ("-one"));
  dq = indexed_deque_put_before_first (dq, make_string_t ("-two"));
  dq = indexed_deque_put_before_first (dq, make_string_t ("-three"));
  dq = indexed_deque_put_after_last (dq, make_string_t ("two"));
  dq = indexed_deque_put_after_last (dq, make_string_t ("three"));
  assert (indexed_deque_size (dq) == 7);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 0),
                             make_string_t ("-three")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 1),
                             make_string_t ("-two")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 2),
                             make_string_t ("-one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 3),
                             make_string_t ("zero")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 4),
                             make_string_t ("one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 5),
                             make_string_t ("two")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 6),
                             make_string_t ("three")));

  dq = indexed_deque_delete_first (dq);
  assert (indexed_deque_size (dq) == 6);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 0),
                             make_string_t ("-two")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 1),
                             make_string_t ("-one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 2),
                             make_string_t ("zero")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 3),
                             make_string_t ("one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 4),
                             make_string_t ("two")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 5),
                             make_string_t ("three")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_first (dq),
                             make_string_t ("-two")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_last (dq),
                             make_string_t ("three")));

  dq = indexed_deque_delete_last (dq);
  assert (indexed_deque_size (dq) == 5);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 0),
                             make_string_t ("-two")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 1),
                             make_string_t ("-one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 2),
                             make_string_t ("zero")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 3),
                             make_string_t ("one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get (dq, 4),
                             make_string_t ("two")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_first (dq),
                             make_string_t ("-two")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_last (dq),
                             make_string_t ("two")));

  dq = indexed_deque_delete_first (dq);
  assert (indexed_deque_size (dq) == 4);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_first (dq),
                             make_string_t ("-one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_last (dq),
                             make_string_t ("two")));

  dq = indexed_deque_delete_last (dq);
  assert (indexed_deque_size (dq) == 3);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_first (dq),
                             make_string_t ("-one")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_last (dq),
                             make_string_t ("one")));

  dq = indexed_deque_delete_first (dq);
  assert (indexed_deque_size (dq) == 2);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_first (dq),
                             make_string_t ("zero")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_last (dq),
                             make_string_t ("one")));

  dq = indexed_deque_delete_last (dq);
  assert (indexed_deque_size (dq) == 1);
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_first (dq),
                             make_string_t ("zero")));
  assert (0 == string_t_cmp ((string_t) indexed_deque_get_last (dq),
                             make_string_t ("zero")));

  dq = indexed_deque_delete_first (dq);
  assert (indexed_deque_size (dq) == 0);
  assert (dq == NULL);

  return 0;
}
