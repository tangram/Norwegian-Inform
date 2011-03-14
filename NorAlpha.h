! Z-machine alphabet optimised for Norwegian. Must be included before
! any text in encoded, even before declaring the constants Story and
! Headline.
!
! It is not absolutely necessary to include this file, but without it,
! dictionary words containing "���" may be reduced to the 2-6 first
! characters, rather than 9. This file is only relevant for Zcode games,
! not Glulx.

#ifdef TARGET_ZCODE;
Zcharacter "abcdefghijklmnop�rstuv�xy�"
           "ABCDEFGHIJKLMNOP�RSTUV�XY�"
              "012345.,!?'/-:()wqzWQZ�";
#endif;

