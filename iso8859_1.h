/*
 * Copyright (c) 2004-2006, Doug Harple.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of author nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: iso8859_1.h,v 1.5 2006/07/03 16:45:54 purgedhalo Exp $
 *
 *
 *
 * Special thanks to: http://www.bbsinc.com/iso8859.html
 *
 */

#define ISO8859_1_OFFSET 0x80
#define ISO8859_1_COUNT 128
#define ISO8859_1_MAXLEN 8

/* starts at 80 */
char iso8859_1_trans[ISO8859_1_COUNT][ISO8859_1_MAXLEN] = {
	"_",			/* 0x0080 */
	"_",			/* 0x0081 */
	"_",			/* 0x0082 */
	"_",			/* 0x0083 */
	"_",			/* 0x0084 */
	"...",			/* 0x0085 */
	"_",			/* 0x0086 */
	"_",			/* 0x0087 */
	"^",			/* 0x0088 */
	"_",			/* 0x0089 */
	"S",			/* 0x008A */
	"_",			/* 0x008B */
	"OE",			/* 0x008C */
	"_",			/* 0x008D */
	"_",			/* 0x008E */
	"_",			/* 0x008F */
	"_",			/* 0x0090 */
	"_",			/* 0x0091 */
	"_",			/* 0x0092 */
	"_",			/* 0x0093 */
	"_",			/* 0x0094 */
	"_",			/* 0x0095 */
	"-",			/* 0x0096 */
	"-",			/* 0x0097 */
	"_",			/* 0x0098 */
	"_tm_",			/* 0x0099 */
	"s",			/* 0x009A */
	"_",			/* 0x009B */
	"oe",			/* 0x009C */
	"_",			/* 0x009D */
	"_",			/* 0x009E */
	"Y",			/* 0x009F */
	"_",			/* 0x00A0 */
	"_",			/* 0x00A1 */
	"_cent_",		/* 0x00A2 */
	"_pound_",		/* 0x00A3 */
	"_",			/* 0x00A4 */
	"_yen_",		/* 0x00A5 */
	"_",			/* 0x00A6 */
	"_",			/* 0x00A7 */
	"_",			/* 0x00A8 */
	"_copy_",		/* 0x00A9 */
	"_",			/* 0x00AA */
	"_",			/* 0x00AB */
	"_",			/* 0x00AC */
	"_",			/* 0x00AD */
	"_reg_",		/* 0x00AE */
	"_",			/* 0x00AF */
	"_",			/* 0x00B0 */
	"_",			/* 0x00B1 */
	"2",			/* 0x00B2 */
	"3",			/* 0x00B3 */
	"_",			/* 0x00B4 */
	"b",			/* 0x00B5 */
	"_pp_",			/* 0x00B6 */
	"_",			/* 0x00B7 */
	"_",			/* 0x00B8 */
	"1",			/* 0x00B9 */
	"_",			/* 0x00BA */
	"_",			/* 0x00BB */
	"_",			/* 0x00BC */
	"_",			/* 0x00BD */
	"_",			/* 0x00BE */
	"_",			/* 0x00BF */
	"A",			/* 0x00C0 */
	"A",			/* 0x00C1 */
	"A",			/* 0x00C2 */
	"A",			/* 0x00C3 */
	"A",			/* 0x00C4 */
	"A",			/* 0x00C5 */
	"AE",			/* 0x00C6 */
	"C",			/* 0x00C7 */
	"E",			/* 0x00C8 */
	"E",			/* 0x00C9 */
	"E",			/* 0x00CA */
	"E",			/* 0x00CB */
	"I",			/* 0x00CC */
	"I",			/* 0x00CD */
	"I",			/* 0x00CE */
	"I",			/* 0x00CF */
	"TH",			/* 0x00D0 */
	"N",			/* 0x00D1 */
	"O",			/* 0x00D2 */
	"O",			/* 0x00D3 */
	"O",			/* 0x00D4 */
	"O",			/* 0x00D5 */
	"O",			/* 0x00D6 */
	"x",			/* 0x00D7 */
	"O",			/* 0x00D8 */
	"U",			/* 0x00D9 */
	"U",			/* 0x00DA */
	"U",			/* 0x00DB */
	"U",			/* 0x00DC */
	"Y",			/* 0x00DD */
	"TH",			/* 0x00DE */
	"ss",			/* 0x00DF */
	"a",			/* 0x00E0 */
	"a",			/* 0x00E1 */
	"a",			/* 0x00E2 */
	"a",			/* 0x00E3 */
	"a",			/* 0x00E4 */
	"a",			/* 0x00E5 */
	"ae",			/* 0x00E6 */
	"c",			/* 0x00E7 */
	"e",			/* 0x00E8 */
	"e",			/* 0x00E9 */
	"e",			/* 0x00EA */
	"e",			/* 0x00EB */
	"i",			/* 0x00EC */
	"i",			/* 0x00ED */
	"i",			/* 0x00EE */
	"i",			/* 0x00EF */
	"th",			/* 0x00F0 */
	"n",			/* 0x00F1 */
	"o",			/* 0x00F2 */
	"o",			/* 0x00F3 */
	"o",			/* 0x00F4 */
	"o",			/* 0x00F5 */
	"o",			/* 0x00F6 */
	"_",			/* 0x00F7 */
	"o",			/* 0x00F8 */
	"u",			/* 0x00F9 */
	"u",			/* 0x00FA */
	"u",			/* 0x00FB */
	"u",			/* 0x00FC */
	"y",			/* 0x00FD */
	"th",			/* 0x00FE */
	"y",			/* 0x00FF */
};
