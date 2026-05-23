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
#include <inttypes.h>
#include <gc/gc.h>
#include <libhiha/libhiha.h>

/*
  A simple check of str_nul_hash() to at least gain some assurance
  that it does not crash and returns reproducible numbers. The number
  also must differ, depending on index.
*/

HIHA_VISIBLE const char version_etc_copyright[] =
  "Copyright %s %d Barry Schwartz";

/* Do not be afraid of a barking dog. Be afraid of a silent dog. */
static const char *const proverb =
  "Ne timu hundon bojantan. Timu hundon silentan.";

const uint64_t nominal[] = {
  0xA9671C71A5D7C2D0,
  0x4F62F348878E1CD5,
  0x3F2662FD4A6C276C,
  0x8830C0E9E2A603D2,
  0x9C0E37126F1F49D1,
  0x0E890F3AE1618F32,
  0xDF5F7F0B6E7D7816,
  0xF57D57AB26327340,
  0x6902CA00DEF6AB47,
  0x5EFDCFFA758AD9B2,
  0x5F9CCDEA74683E49,
  0xB7909CE0DBB48090,
  0x5EDA90919A96B08E,
  0x085F7DDD7DD0A2F9,
  0x85DE3A3331CF1B95,
  0x5FCA7CC884A094C6,
  0xB66DDF771D260B78,
  0x99E3599EE3ECC459,
  0x6F710416581DB7C8,
  0xC4C89021942DACC6,
  0xDB6BA69332A89903,
  0x773D4460A27728B7,
  0xCE657D121FE846C1,
  0xF7896B64E9B97B24,
  0x97627897C37FCE17,
  0x5E379F29248EB3A9,
  0xEAEF97A5AF62AA99,
  0xD66401B1571FE2B1,
  0x1A46F387627415EC,
  0x40D3033060EA6E19,
  0x798EFCB574A394C2,
  0xB202E81B7AA9ADE0,
  0x69021D8759BE7143,
  0xCE0802E590AC42D0,
  0xAD3816C7B79E879E,
  0x09B815699A2887D5,
  0x25BEA9FB704FD77E,
  0x6A0CB0862C1E5F53,
  0xCC227CADBCDBD0C8,
  0x8ECFC58EFA89625A,
  0x7C65BDE20E8084CA,
  0x4833A2BDE97BF81C,
  0x20D0BF45C149501B,
  0xF5A0747E48346594,
  0x105F98E5666B67B6,
  0x6739BBECAF775829,
  0x81DAE51881452198,
  0x0D0A8F4738C7DBB1,
  0x1CC9542318A70F52,
  0x68D05B1610C6C676,
  0x71490084B0D0E913,
  0x59FCA666A28C6166,
  0x4C521B04AE77F3E3,
  0x9F463C717DBD506C,
  0xACBD8C61774B3FA8,
  0x343BF05630ECF8B9,
  0x71836A50D4D28734,
  0xF95BFEC727994C7D,
  0x317D7C18127F2AE8,
  0x01FA7BAC00B60F3B,
  0xE35AB420E45A127C,
  0x6BA62F4D1E0111F9,
  0xC4FF83F159E8B624,
  0xDAE52FA133A3A539,
  0x91DCC08A3BEAB4F5,
  0xFB1CE0EAAAB0A557,
  0x1A59D09F51F0FD5F,
  0xD9E342D44AD1ED92,
  0x828849DC789D98D0,
  0x2D4E19AFCF5E0FE1,
  0x7A475DFC5FA5B71D,
  0xFE2C02CD3F2205F1,
  0x6A30DB8D30C6F3F2,
  0x38E64F7FF7E15FEB,
  0x9DEEF64AE7FEFE8E,
  0x0A438F1C65BFE1A3,
  0xF112A7B9956707DB,
  0x6410C33922B40569,
  0x7D22026A87A5989F,
  0xEAA7E89828C64BE2,
  0xEAA264F3C5595DBF,
  0x5BFFC2563D1B8792,
  0xBEC65E0BA4D4F5D0,
  0xF99180CD0AA657C8,
  0x8EE0F4008B2E9FAB,
  0xFD23714B61BF244D,
  0x3E89765F6CB3DCAB,
  0x7D0E389C777C8837,
  0x6CE54010C4F63418,
  0x0B5E06678C1FCFC6,
  0xFFDD07528A775271,
  0xCF6C71C186A285C8,
  0x9E2A4D2E02171F57,
  0xBC974F497D402BDB,
  0x746D6EE308832D54,
  0xE5A5DC8B8BCA57DC,
  0x51541AC725954BCB,
  0xC7CFE6A72FACFBFF,
  0x3D078468E4C1CB30,
  0x1937BF76BEAED1C1
};

int
main (void)
{
  GC_INIT ();

  str_nul_hash_context_t context = str_nul_hash_init (proverb);
  for (size_t i = 0; i != 100; i += 1)
    {
      //printf ("0x%016" PRIX64 ",\n", str_nul_hash (context, i));
      if (str_nul_hash (context, i) != nominal[i])
        abort ();
    }

  for (size_t i = 100; i != 0; i -= 1)
    if (str_nul_hash (context, i - 1) != nominal[i - 1])
      abort ();

  return 0;
}
