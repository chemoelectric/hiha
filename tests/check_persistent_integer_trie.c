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
#include <gc/gc.h>
#include <libhiha/libhiha.h>

HIHA_VISIBLE const char version_etc_copyright[] =
  "Copyright %s %d Barry Schwartz";

#define BUFSIZE 10000

static int buf[BUFSIZE];

static void
fisher_yates (void)
{
  for (int i = 0; i != (BUFSIZE); i += 1)
    buf[i] = i;
  for (int i = (BUFSIZE) - 1; i != 0; i -= 1)
    {
      /* Dividing the big random integer is good enough. */
      int j = (int) (random () % ((long) i) + 1);

      int _tmp = buf[i];
      buf[i] = buf[j];
      buf[j] = _tmp;
    }
}

HIHA_INT_TRIE_NODES_DECL (uint_trie_int_node, unsigned int, int);
HIHA_INT_TRIE_INSERT_DEFN (uint_trie_int_insert,
                           uint_trie_int_node, unsigned int, int);
HIHA_INT_TRIE_SEARCH_DEFN (uint_trie_int_search,
                           uint_trie_int_node, unsigned int);
HIHA_INT_TRIE_DELETE_DEFN (uint_trie_int_delete,
                           uint_trie_int_node, unsigned int);
HIHA_INT_TRIE_WALK_DEFN (uint_trie_int_walk, uint_trie_int_node);

uint_trie_int_node_t tree;
uint_trie_int_node_t tree1;
uint_trie_int_node_t tree2;
uint_trie_int_node_t tree3;
int i;
int j;

static void
delete_leaf (uint_trie_int_node_leaf_t leaf, void *data)
{
  tree2 = (uint_trie_int_delete) (tree2, leaf->key);
  uint_trie_int_node_t tree3 = *((uint_trie_int_node_t *) data);
  tree3 = (uint_trie_int_insert) (tree3, leaf->key, leaf->value);
  *((uint_trie_int_node_t *) data) = tree3;
}

static void
trie_test (void)
{
  /* Insert in random order. */
  fisher_yates ();
  tree = NULL;
  for (i = 0; i != (BUFSIZE); i += 1)
    tree = (uint_trie_int_insert) (tree, buf[i], -buf[i]);

  /* Search in random order. */
  fisher_yates ();
  for (i = 0; i != (BUFSIZE); i += 1)
    {
      uint_trie_int_node_leaf_t leaf =
        (uint_trie_int_search) (tree, buf[i]);
      assert (leaf != NULL);
      assert (leaf->key == buf[i]);
      assert (leaf->value == -buf[i]);
    }

  /* Delete in random order. */
  fisher_yates ();
  for (i = 0; i != (BUFSIZE); i += 1)
    {
      tree = (uint_trie_int_delete) (tree, buf[i]);
      for (j = 0; j != i + 1; j += 1)
        {
          uint_trie_int_node_leaf_t leaf =
            (uint_trie_int_search) (tree, buf[j]);
          assert (leaf == NULL);
        }
      for (j = i + 1; j != (BUFSIZE); j += 1)
        {
          uint_trie_int_node_leaf_t leaf =
            (uint_trie_int_search) (tree, buf[j]);
          assert (leaf != NULL);
          assert (leaf->key == buf[j]);
          assert (leaf->value == -buf[j]);
        }
    }
  assert (tree == NULL);

  /* Fill two trees. Walk through one and use the walk to delete */
  /* the contents of the other tree. Simultaneously use the walk */
  /* to build another tree.                                      */
  fisher_yates ();
  tree1 = NULL;
  tree2 = NULL;
  tree3 = NULL;
  for (i = 0; i != (BUFSIZE); i += 1)
    {
      tree1 = (uint_trie_int_insert) (tree1, buf[i], -buf[i]);
      tree2 = (uint_trie_int_insert) (tree2, buf[i], -buf[i]);
    }

  (uint_trie_int_walk) (tree1, delete_leaf, &tree3);
  assert (tree2 == NULL);
  for (i = 0; i != (BUFSIZE); i += 1)
    {
      assert (tree3 != NULL);
      tree3 = (uint_trie_int_delete) (tree3, buf[i]);
    }
  assert (tree3 == NULL);
}

int
main (void)
{
  GC_INIT ();
  trie_test ();
  return 0;
}
