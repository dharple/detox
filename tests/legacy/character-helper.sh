#!/usr/bin/env bash

# Unicode: 0x0007 / Hex: \x07 / BELL
UTF8_0007=$(printf "\007")
export UTF8_0007;

# Unicode: 0x0009 / Hex: \x09 / CHARACTER TABULATION
UTF8_0009=$(printf "\011")
export UTF8_0009;

# Unicode: 0x003f / Hex: \x3f / QUESTION MARK
UTF8_003F=$(printf "\077")
export UTF8_003F;

# Unicode: 0x007e / Hex: \x7e / TILDE
UTF8_007E=$(printf "\176")
export UTF8_007E;

# Unicode: 0x007f / Hex: \x7f / DELETE
UTF8_007F=$(printf "\177")
export UTF8_007F;

# Unicode: 0x0080 / Hex: \xc2\x80 / undefined
UTF8_0080=$(printf "\302\200")
export UTF8_0080;

# Unicode: 0x00a0 / Hex: \xc2\xa0 / NO-BREAK SPACE
UTF8_00A0=$(printf "\302\240")
export UTF8_00A0;

# Unicode: 0x00a9 / Hex: \xc2\xa9 / COPYRIGHT SIGN
UTF8_00A9=$(printf "\302\251")
export UTF8_00A9;

# Unicode: 0x00ad / Hex: \xc2\xad / SOFT HYPHEN
UTF8_00AD=$(printf "\302\255")
export UTF8_00AD;

# Unicode: 0x00ae / Hex: \xc2\xae / REGISTERED SIGN
UTF8_00AE=$(printf "\302\256")
export UTF8_00AE;

# Unicode: 0x00b0 / Hex: \xc2\xb0 / DEGREE SIGN
UTF8_00B0=$(printf "\302\260")
export UTF8_00B0;

# Unicode: 0x00be / Hex: \xc2\xbe / VULGAR FRACTION THREE QUARTERS
UTF8_00BE=$(printf "\302\276")
export UTF8_00BE;

# Unicode: 0x00bf / Hex: \xc2\xbf / INVERTED QUESTION MARK
UTF8_00BF=$(printf "\302\277")
export UTF8_00BF;

# Unicode: 0x00c0 / Hex: \xc3\x80 / LATIN CAPITAL LETTER A WITH GRAVE
UTF8_00C0=$(printf "\303\200")
export UTF8_00C0;

# Unicode: 0x00c1 / Hex: \xc3\x81 / LATIN CAPITAL LETTER A WITH ACUTE
UTF8_00C1=$(printf "\303\201")
export UTF8_00C1;

# Unicode: 0x00c6 / Hex: \xc3\x86 / LATIN CAPITAL LETTER AE
UTF8_00C6=$(printf "\303\206")
export UTF8_00C6;

# Unicode: 0x00de / Hex: \xc3\x9e / LATIN CAPITAL LETTER THORN
UTF8_00DE=$(printf "\303\236")
export UTF8_00DE;

# Unicode: 0x014a / Hex: \xc5\x8a / LATIN CAPITAL LETTER ENG
UTF8_014A=$(printf "\305\212")
export UTF8_014A;

# Unicode: 0x0172 / Hex: \xc5\xb2 / LATIN CAPITAL LETTER U WITH OGONEK
UTF8_0172=$(printf "\305\262")
export UTF8_0172;

# Unicode: 0x4000 / Hex: \xe4\x80\x80
UTF8_4000=$(printf "\344\200\200")
export UTF8_4000;

# Unicode: 0x10348 / Hex: \xf0\x90\x8d\x88 / GOTHIC LETTER HWAIR
UTF8_10348=$(printf "\360\220\215\210")
export UTF8_10348;

# Unicode: 0x1f37a / Hex: \xf0\x9f\x8d\xba
UTF8_1F37A=$(printf "\360\237\215\272")
export UTF8_1F37A;
