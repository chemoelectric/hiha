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
#include <libhiha/token_t.h>
#include <libhiha/string_t.h>
#include <libhiha/string_literal.h>

#define _(msgid) HIHA_GETTEXT (msgid)

/*
static void
skip_blanks (buffered_token_getter_t getter, token_t_vector_t *toks,
             const char **error_message)
{
  
}
*/

//static void
//get_next_digit (buffered_token_getter_t getter, uint32_t *separator,
//                uint32_t *digit, const char **error_message)
//{
//  *separator = 0;
//  *digit = 0;
//  size_t num_to_consume = 0;
//
//  token_t t;
//  getter->look_at_token (getter, 0, &t, error_message);
//  if (*error_message == NULL
//      && string_t_cmp (t->token_kind, string_t_CP ()) == 0)
//    {
//      check_code_point_token (t);
//      if (is_ascii_digit (t->token_value->s[0]))
//        {
//          *digit = t->token_value->s[0];
//          num_to_consume = 1;
//        }
//      else
//        if (uc_is_general_category
//            (t->token_value->s[0], UC_CATEGORY_Pc))
//        {
//          token_t u;
//          getter->look_at_token (getter, 1, &u, error_message);
//          if (*error_message == NULL
//              && string_t_cmp (u->token_kind, string_t_CP ()) == 0)
//            {
//              check_code_point_token (u);
//              if (is_ascii_digit (u->token_value->s[0]))
//                {
//                  *separator = t->token_value->s[0];
//                  *digit = u->token_value->s[0];
//                  num_to_consume = 2;
//                }
//            }
//        }
//    }
//
//  consume_tokens (getter, num_to_consume, error_message);
//}

/*
  local variables:
  mode: c
  coding: utf-8
  end:
*/
