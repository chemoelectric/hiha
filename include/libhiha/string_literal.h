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

#ifndef __LIBHAHA__STRING_LITERAL_H__INCLUDED__
#define __LIBHAHA__STRING_LITERAL_H__INCLUDED__

void scan_string_literal (buffered_token_getter_t getter, token_t *tok,
                          string_t *string, const char **error_message);
void dequote_string_literal (string_t literal, token_t *tok,
                             string_t *result,
                             const char **error_message);

#endif /* __LIBHAHA__STRING_LITERAL_H__INCLUDED__ */

/*
  local variables:
  mode: c
  coding: utf-8
  end:
*/
