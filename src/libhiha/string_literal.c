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
#include <stdint.h>
#include <inttypes.h>
#include <xalloc.h>
#include <unistr.h>
#include <unictype.h>
#include <libhiha/token_t.h>
#include <libhiha/string_t.h>
#include <libhiha/string_literal.h>

#define _(msgid) HIHA_GETTEXT (msgid)

static const char *
multiple_code_points (text_location_t loc)
{
  char buf[1000];
  snprintf (buf, 1000,
            _
            ("%s: lexer problem: multiple code-point token in string literal"),
            text_location_string (loc));
  return xstrdup (buf);
}

static void
look_at_token (buffered_token_getter_t getter, size_t i,
               token_t *tok, const char **error_message)
{
  getter->look_at_token (getter, i, tok, error_message);
  if (*error_message == NULL)
    if (string_t_cmp ((*tok)->token_kind, string_t_EOF ()) != 0
        && (*tok)->token_value->n != 1)
      *error_message = multiple_code_points ((*tok)->loc);
}

static const char *
eof_in_open_string (text_location_t loc)
{
  char buf[1000];
  snprintf (buf, 1000, _("%s: end of input in open string"),
            text_location_string (loc));
  return xstrdup (buf);
}

static const char *
unrecognized_escape (text_location_t loc)
{
  char buf[1000];
  snprintf (buf, 1000, _("%s: unrecognized string escape"),
            text_location_string (loc));
  return xstrdup (buf);
}

static const char *
bad_newline_escape (text_location_t loc)
{
  char buf[1000];
  snprintf (buf, 1000, _("%s: perhaps a bad newline escape"),
            text_location_string (loc));
  return xstrdup (buf);
}

static const char *
empty_hex_escape (text_location_t loc)
{
  char buf[1000];
  snprintf (buf, 1000, _("%s: empty hex escape in string"),
            text_location_string (loc));
  return xstrdup (buf);
}

static const char *
illegal_char_in_hex_escape (text_location_t loc)
{
  char buf[1000];
  snprintf (buf, 1000, _("%s: illegal character in hex escape"),
            text_location_string (loc));
  return xstrdup (buf);
}

static const char *
not_a_code_point (text_location_t loc, const char *hex)
{
  char buf[1000];
  snprintf (buf, 1000, _("%s: not a Unicode code point: \\x%s;"),
            text_location_string (loc), hex);
  return xstrdup (buf);
}

static bool
token_is_EOF (token_t tok)
{
  return (string_t_cmp (tok->token_kind, string_t_EOF ()) == 0);
}

static bool
token_is_char (token_t tok, int ch)
{
  return (tok->token_value->n == 1 && tok->token_value->s[0] == ch);
}

static bool
token_is_space_separator (token_t tok)
{
  return (tok->token_value->n == 1
          && uc_is_general_category (tok->token_value->s[0],
                                     UC_CATEGORY_Zs));
}

static bool
token_is_ascii_hex_digit (token_t tok)
{
  return (tok->token_value->n == 1
          && uc_is_property (tok->token_value->s[0],
                             UC_PROPERTY_ASCII_HEX_DIGIT));
}

static void
skip_blanks (buffered_token_getter_t getter, token_t_vector_t *toks,
             const char **error_message)
{
  if (*error_message == NULL)
    {
      token_t t;
      look_at_token (getter, 0, &t, error_message);
      while (*error_message == NULL && token_is_space_separator (t))
        {
          getter->get_token (getter, &t, error_message);
          if (*error_message == NULL)
            {
              *toks = token_t_vector_push (*toks, t);
              look_at_token (getter, 0, &t, error_message);
            }
        }
      if (*error_message != NULL && token_is_EOF (t))
        *error_message = eof_in_open_string (t->loc);
    }
}

static void
skip_blanks_newline_blanks (buffered_token_getter_t getter,
                            token_t_vector_t *toks,
                            const char **error_message)
{
  /* Skip blanks until a newline, and then skip more blanks. This is
     how backslash at the end of a line is handled is handled in
     R⁷RS Scheme, and is how we will handle it here. */

  skip_blanks (getter, toks, error_message);
  if (*error_message == NULL)
    {
      token_t t;
      look_at_token (getter, 0, &t, error_message);
      if (!token_is_char (t, '\n'))
        *error_message = bad_newline_escape (t->loc);
      else
        {
          getter->get_token (getter, &t, error_message);
          if (*error_message == NULL)
            {
              *toks = token_t_vector_push (*toks, t);
              skip_blanks (getter, toks, error_message);
            }
        }
    }
}

static void
hex_ends_badly (token_t tok, const char **error_message)
{
  if (token_is_EOF (tok))
    *error_message = eof_in_open_string (tok->loc);
  else
    *error_message = illegal_char_in_hex_escape (tok->loc);
}

static void
scan_the_hex (token_t tok, string_t_vector_t v, uint32_t *code_point,
              const char **error_message)
{
  string_t str = empty_string_t ();
  for (size_t i = 0; i != string_t_vector_length (v); i += 1)
    str = concat_string_t (str, string_t_vector_ref (v, i), NULL);
  const char *s = make_str_nul (str);
  int num_scanned = sscanf (s, "%" SCNx32, code_point);
  if (num_scanned != 1)
    *error_message = not_a_code_point (tok->loc, s);
  else if (u32_check (code_point, 1) != NULL)
    *error_message = not_a_code_point (tok->loc, s);
}

static void
hex_until_semicolon (buffered_token_getter_t getter,
                     token_t_vector_t *toks,
                     uint32_t *code_point, const char **error_message)
{
  if (*error_message == NULL)
    {
      *code_point = 0xFFFD;     /* U+FFFD REPLACEMENT CHARACTER */
      string_t_vector_t v = NULL;
      token_t t;
      look_at_token (getter, 0, &t, error_message);
      while (*error_message == NULL && token_is_ascii_hex_digit (t))
        {
          getter->get_token (getter, &t, error_message);
          if (*error_message == NULL)
            {
              *toks = token_t_vector_push (*toks, t);
              v = string_t_vector_push (v, t->token_value);
              look_at_token (getter, 0, &t, error_message);
            }
        }
      if (*error_message == NULL)
        {
          if (token_is_char (t, ';'))
            {
              if (string_t_vector_length (v) == 0)
                *error_message = empty_hex_escape (t->loc);
              else
                scan_the_hex (t, v, code_point, error_message);
            }
          else
            hex_ends_badly (t, error_message);
        }
    }
}

static void
simple_escape (buffered_token_getter_t getter,
               token_t_vector_t *toks,
               string_t *substring,
               const char **error_message, int new_char)
{
  token_t t;
  getter->get_token (getter, &t, error_message);
  if (*error_message == NULL)
    {
      *toks = token_t_vector_push (*toks, t);
      char buf[2];
      buf[0] = (char) new_char;
      buf[1] = '\0';
      *substring = make_string_t (buf);
    }
}

static void
escape_newline (buffered_token_getter_t getter,
                token_t_vector_t *toks,
                string_t *substring, const char **error_message)
{
  token_t t;
  getter->get_token (getter, &t, error_message);
  if (*error_message == NULL)
    {
      *toks = token_t_vector_push (*toks, t);
      skip_blanks (getter, toks, error_message);
      if (*error_message == NULL)
        *substring = empty_string_t ();
    }
}

static void
escape_blank (buffered_token_getter_t getter,
              token_t_vector_t *toks,
              string_t *substring, const char **error_message)
{
  token_t t;
  getter->get_token (getter, &t, error_message);
  if (*error_message == NULL)
    {
      *toks = token_t_vector_push (*toks, t);
      skip_blanks_newline_blanks (getter, toks, error_message);
      if (*error_message == NULL)
        *substring = empty_string_t ();
    }
}

static void
escape_x (buffered_token_getter_t getter,
          token_t_vector_t *toks,
          string_t *substring, const char **error_message)
{
  token_t t;
  getter->get_token (getter, &t, error_message);
  if (*error_message == NULL)
    {
      *toks = token_t_vector_push (*toks, t);
      uint32_t code_point;
      hex_until_semicolon (getter, toks, &code_point, error_message);
      if (*error_message == NULL)
        {
          struct string *str = XMALLOC (struct string);
          str->n = 1;
          str->s = XNMALLOC (1, uint32_t);
          str->s[0] = code_point;
          *substring = str;
        }
    }
}

static void
scan_escape (buffered_token_getter_t getter,
             token_t_vector_t *toks,
             string_t *substring, const char **error_message)
{
  token_t t;
  getter->get_token (getter, &t, error_message);
  if (*error_message == NULL)
    {
      *toks = token_t_vector_push (*toks, t);
      look_at_token (getter, 0, &t, error_message);
      if (token_is_EOF (t))
        *error_message = eof_in_open_string (t->loc);
      else if (token_is_space_separator (t))
        escape_blank (getter, toks, substring, error_message);
      else
        switch (t->token_value->s[0])
          {
          case 'x':
            escape_x (getter, toks, substring, error_message);
            break;
          case '\n':
            escape_newline (getter, toks, substring, error_message);
            break;
          case '"':
            simple_escape (getter, toks, substring, error_message, '"');
            break;
          case '\\':
            simple_escape (getter, toks, substring, error_message,
                           '\\');
            break;
          case '|':
            simple_escape (getter, toks, substring, error_message, '|');
            break;
          case 'a':
            simple_escape (getter, toks, substring, error_message,
                           '\a');
            break;
          case 'b':
            simple_escape (getter, toks, substring, error_message,
                           '\b');
            break;
          case 't':
            simple_escape (getter, toks, substring, error_message,
                           '\t');
            break;
          case 'n':
            simple_escape (getter, toks, substring, error_message,
                           '\n');
            break;
          case 'r':
            simple_escape (getter, toks, substring, error_message,
                           '\r');
            break;
          case 'v':
            simple_escape (getter, toks, substring, error_message,
                           '\013');
            break;
          case 'f':
            simple_escape (getter, toks, substring, error_message,
                           '\014');
            break;
          default:
            *error_message = unrecognized_escape (t->loc);
            break;
          }
    }
}

static void
scan_string_portion (buffered_token_getter_t getter,
                     token_t_vector_t *toks,
                     string_t *substring, const char **error_message)
{
  if (*error_message == NULL)
    {
      token_t t;
      look_at_token (getter, 0, &t, error_message);
      if (token_is_EOF (t))
        *error_message = eof_in_open_string (t->loc);
      else if (token_is_char (t, '\\'))
        scan_escape (getter, toks, substring, error_message);
      else if (token_is_char (t, '"'))
        {
          getter->get_token (getter, &t, error_message);
          if (*error_message == NULL)
            {
              *toks = token_t_vector_push (*toks, t);
              /* Setting (*substring == NULL) indicates the string is
                 complete. */
              *substring = NULL;
            }
        }
      else
        {
          getter->get_token (getter, &t, error_message);
          if (*error_message == NULL)
            {
              *toks = token_t_vector_push (*toks, t);
              *substring = t->token_value;
            }
        }
    }
}

HIHA_VISIBLE void
scan_string_literal (buffered_token_getter_t getter, token_t *tok,
                     string_t *string, const char **error_message)
{
  *string = empty_string_t ();
  *tok = NULL;
  token_t t;
  getter->get_token (getter, &t, error_message);
  if (*error_message == NULL)
    {
      assert (token_is_char (t, '"'));
      token_t_vector_t toks = token_t_vector_push (NULL, t);
      string_t substring;
      scan_string_portion (getter, &toks, &substring, error_message);
      while (*error_message == NULL && substring != NULL)
        {
          *string = concat_string_t (*string, substring, NULL);
          scan_string_portion (getter, &toks, &substring,
                               error_message);
        }
      if (*error_message == NULL)
        {
          string_t tokval = empty_string_t ();
          size_t n = token_t_vector_length (toks);
          for (size_t i = 0; i != n; i += 1)
            tokval =
              concat_string_t (tokval, token_t_vector_ref (toks, i),
                               NULL);
          *tok = make_token_t (make_string_t ("STR"), tokval, t->loc);
        }
    }
}

/*
  local variables:
  mode: c
  coding: utf-8
  end:
*/
