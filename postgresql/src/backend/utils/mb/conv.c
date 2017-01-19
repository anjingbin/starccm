/*
 * conversion between client encoding and server internal encoding
 * (currently mule internal code (mic) is used)
 * Tatsuo Ishii
 *
 * WIN1250 client encoding support contributed by Pavel Behal
 * SJIS UDC (NEC selection IBM kanji) support contributed by Eiji Tokuya
 *
 * $Id: conv.c,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
 *
 *
 */
#include "postgres.h"
#include "mb/pg_wchar.h"

#ifdef UNICODE_CONVERSION

/*
 * for Unicode (UTF-8) support
 */
#include "Unicode/utf8_to_iso8859_2.map"
#include "Unicode/utf8_to_iso8859_3.map"
#include "Unicode/utf8_to_iso8859_4.map"
#include "Unicode/utf8_to_iso8859_5.map"
#include "Unicode/utf8_to_iso8859_6.map"
#include "Unicode/utf8_to_iso8859_7.map"
#include "Unicode/utf8_to_iso8859_8.map"
#include "Unicode/utf8_to_iso8859_9.map"
#include "Unicode/utf8_to_iso8859_10.map"
#include "Unicode/utf8_to_iso8859_13.map"
#include "Unicode/utf8_to_iso8859_14.map"
#include "Unicode/utf8_to_iso8859_15.map"
#include "Unicode/utf8_to_iso8859_16.map"
#include "Unicode/iso8859_2_to_utf8.map"
#include "Unicode/iso8859_3_to_utf8.map"
#include "Unicode/iso8859_4_to_utf8.map"
#include "Unicode/iso8859_5_to_utf8.map"
#include "Unicode/iso8859_6_to_utf8.map"
#include "Unicode/iso8859_7_to_utf8.map"
#include "Unicode/iso8859_8_to_utf8.map"
#include "Unicode/iso8859_9_to_utf8.map"
#include "Unicode/iso8859_10_to_utf8.map"
#include "Unicode/iso8859_13_to_utf8.map"
#include "Unicode/iso8859_14_to_utf8.map"
#include "Unicode/iso8859_15_to_utf8.map"
#include "Unicode/iso8859_16_to_utf8.map"
#include "Unicode/utf8_to_euc_jp.map"
#include "Unicode/euc_jp_to_utf8.map"
#include "Unicode/utf8_to_euc_cn.map"
#include "Unicode/euc_cn_to_utf8.map"
#include "Unicode/utf8_to_euc_kr.map"
#include "Unicode/euc_kr_to_utf8.map"
#include "Unicode/utf8_to_euc_tw.map"
#include "Unicode/euc_tw_to_utf8.map"
#include "Unicode/utf8_to_sjis.map"
#include "Unicode/sjis_to_utf8.map"
#include "Unicode/utf8_to_big5.map"
#include "Unicode/big5_to_utf8.map"
/* Cyrillic charset conversion */
#include "Unicode/alt_to_utf8.map"
#include "Unicode/koi8r_to_utf8.map"
#include "Unicode/win1251_to_utf8.map"
#include "Unicode/utf8_to_alt.map"
#include "Unicode/utf8_to_koi8r.map"
#include "Unicode/utf8_to_win1251.map"
#endif   /* UNICODE_CONVERSION */

/*
 * SJIS alternative code.
 * this code is used if a mapping EUC -> SJIS is not defined.
 */
#define PGSJISALTCODE 0x81ac
#define PGEUCALTCODE 0xa2ae

/*
 * conversion table between SJIS UDC (IBM kanji) and EUC_JP
 */
#include "sjis.map"

/*
 * convert bogus chars that cannot be represented in the current
 * encoding system.
 */
static void
printBogusChar(unsigned char **mic, unsigned char **p)
{
	char		strbuf[16];
	int			l = pg_mic_mblen(*mic);

	*(*p)++ = '(';
	while (l--)
	{
		sprintf(strbuf, "%02x", *(*mic)++);
		*(*p)++ = strbuf[0];
		*(*p)++ = strbuf[1];
	}
	*(*p)++ = ')';
}

/*
 * SJIS ---> MIC
 */
static void
sjis2mic(unsigned char *sjis, unsigned char *p, int len)
{
	int			c1,
				c2,
/* Eiji Tokuya patched begin */
				i,
				k,
				k2;

/* Eiji Tokuya patched end */
	while (len > 0 && (c1 = *sjis++))
	{
		if (c1 >= 0xa1 && c1 <= 0xdf)
		{
			/* JIS X0201 (1 byte kana) */
			len--;
			*p++ = LC_JISX0201K;
			*p++ = c1;
		}
		else if (c1 > 0x7f)
		{
			/*
			 * JIS X0208, X0212, user defined extended characters
			 */
			c2 = *sjis++;
			k = (c1 << 8) + c2;
/* Eiji Tokuya patched begin */
			if (k >= 0xed40 && k < 0xf040)
			{
				/* NEC selection IBM kanji */
				for (i = 0;; i++)
				{
					k2 = ibmkanji[i].nec;
					if (k2 == 0xffff)
						break;
					if (k2 == k)
					{
						k = ibmkanji[i].sjis;
						c1 = (k >> 8) & 0xff;
						c2 = k & 0xff;
					}
				}
			}

			if (k < 0xeb3f)
/* Eiji Tokuya patched end */
			{
				/* JIS X0208 */
				len -= 2;
				*p++ = LC_JISX0208;
				*p++ = ((c1 & 0x3f) << 1) + 0x9f + (c2 > 0x9e);
				*p++ = c2 + ((c2 > 0x9e) ? 2 : 0x60) + (c2 < 0x80);
			}
/* Eiji Tokuya patched begin */
			else if ((k >= 0xeb40 && k < 0xf040) || (k >= 0xfc4c && k <= 0xfcfc))
			{
				/* NEC selection IBM kanji - Other undecided justice */
/* Eiji Tokuya patched end */
				*p++ = LC_JISX0208;
				*p++ = PGEUCALTCODE >> 8;
				*p++ = PGEUCALTCODE & 0xff;
			}
			else if (k >= 0xf040 && k < 0xf540)
			{
				/*
				 * UDC1 mapping to X0208 85 ku - 94 ku JIS code 0x7521 -
				 * 0x7e7e EUC 0xf5a1 - 0xfefe
				 */
				len -= 2;
				*p++ = LC_JISX0208;
				c1 -= 0x6f;
				*p++ = ((c1 & 0x3f) << 1) + 0xf3 + (c2 > 0x9e);
				*p++ = c2 + ((c2 > 0x9e) ? 2 : 0x60) + (c2 < 0x80);
			}
			else if (k >= 0xf540 && k < 0xfa40)
			{
				/*
				 * UDC2 mapping to X0212 85 ku - 94 ku JIS code 0x7521 -
				 * 0x7e7e EUC 0x8ff5a1 - 0x8ffefe
				 */
				len -= 2;
				*p++ = LC_JISX0212;
				c1 -= 0x74;
				*p++ = ((c1 & 0x3f) << 1) + 0xf3 + (c2 > 0x9e);
				*p++ = c2 + ((c2 > 0x9e) ? 2 : 0x60) + (c2 < 0x80);
			}
			else if (k >= 0xfa40)
			{
				/*
				 * mapping IBM kanji to X0208 and X0212
				 *
				 */
				len -= 2;
				for (i = 0;; i++)
				{
					k2 = ibmkanji[i].sjis;
					if (k2 == 0xffff)
						break;
					if (k2 == k)
					{
						k = ibmkanji[i].euc;
						if (k >= 0x8f0000)
						{
							*p++ = LC_JISX0212;
							*p++ = 0x80 | ((k & 0xff00) >> 8);
							*p++ = 0x80 | (k & 0xff);
						}
						else
						{
							*p++ = LC_JISX0208;
							*p++ = 0x80 | (k >> 8);
							*p++ = 0x80 | (k & 0xff);
						}
					}
				}
			}
		}
		else
		{						/* should be ASCII */
			len--;
			*p++ = c1;
		}
	}
	*p = '\0';
}

/*
 * MIC ---> SJIS
 */
static void
mic2sjis(unsigned char *mic, unsigned char *p, int len)
{
	int			c1,
				c2,
				k;

	while (len > 0 && (c1 = *mic))
	{
		len -= pg_mic_mblen(mic++);

		if (c1 == LC_JISX0201K)
			*p++ = *mic++;
		else if (c1 == LC_JISX0208)
		{
			c1 = *mic++;
			c2 = *mic++;
			k = (c1 << 8) | (c2 & 0xff);
			if (k >= 0xf5a1)
			{
				/* UDC1 */
				c1 -= 0x54;
				*p++ = ((c1 - 0xa1) >> 1) + ((c1 < 0xdf) ? 0x81 : 0xc1) + 0x6f;
			}
			else
				*p++ = ((c1 - 0xa1) >> 1) + ((c1 < 0xdf) ? 0x81 : 0xc1);
			*p++ = c2 - ((c1 & 1) ? ((c2 < 0xe0) ? 0x61 : 0x60) : 2);
		}
		else if (c1 == LC_JISX0212)
		{
			int			i,
						k2;

			c1 = *mic++;
			c2 = *mic++;
			k = c1 << 8 | c2;
			if (k >= 0xf5a1)
			{
				/* UDC2 */
				c1 -= 0x54;
				*p++ = ((c1 - 0xa1) >> 1) + ((c1 < 0xdf) ? 0x81 : 0xc1) + 0x74;
				*p++ = c2 - ((c1 & 1) ? ((c2 < 0xe0) ? 0x61 : 0x60) : 2);
			}
			else
			{
				/* IBM kanji */
				for (i = 0;; i++)
				{
					k2 = ibmkanji[i].euc & 0xffff;
					if (k2 == 0xffff)
					{
						*p++ = PGSJISALTCODE >> 8;
						*p++ = PGSJISALTCODE & 0xff;
						break;
					}
					if (k2 == k)
					{
						k = ibmkanji[i].sjis;
						*p++ = k >> 8;
						*p++ = k & 0xff;
						break;
					}
				}
			}
		}
		else if (c1 > 0x7f)
		{
			/* cannot convert to SJIS! */
			*p++ = PGSJISALTCODE >> 8;
			*p++ = PGSJISALTCODE & 0xff;
		}
		else
		{						/* should be ASCII */
			*p++ = c1;
		}
	}
	*p = '\0';
}

/*
 * EUC_JP ---> MIC
 */
static void
euc_jp2mic(unsigned char *euc, unsigned char *p, int len)
{
	int			c1;

	while (len > 0 && (c1 = *euc++))
	{
		if (c1 == SS2)
		{						/* 1 byte kana? */
			len -= 2;
			*p++ = LC_JISX0201K;
			*p++ = *euc++;
		}
		else if (c1 == SS3)
		{						/* JIS X0212 kanji? */
			len -= 3;
			*p++ = LC_JISX0212;
			*p++ = *euc++;
			*p++ = *euc++;
		}
		else if (c1 & 0x80)
		{						/* kanji? */
			len -= 2;
			*p++ = LC_JISX0208;
			*p++ = c1;
			*p++ = *euc++;
		}
		else
		{						/* should be ASCII */
			len--;
			*p++ = c1;
		}
	}
	*p = '\0';
}

/*
 * MIC ---> EUC_JP
 */
static void
mic2euc_jp(unsigned char *mic, unsigned char *p, int len)
{
	int			c1;

	while (len > 0 && (c1 = *mic))
	{
		len -= pg_mic_mblen(mic++);

		if (c1 == LC_JISX0201K)
		{
			*p++ = SS2;
			*p++ = *mic++;
		}
		else if (c1 == LC_JISX0212)
		{
			*p++ = SS3;
			*p++ = *mic++;
			*p++ = *mic++;
		}
		else if (c1 == LC_JISX0208)
		{
			*p++ = *mic++;
			*p++ = *mic++;
		}
		else if (c1 > 0x7f)
		{						/* cannot convert to EUC_JP! */
			mic--;
			printBogusChar(&mic, &p);
		}
		else
		{						/* should be ASCII */
			*p++ = c1;
		}
	}
	*p = '\0';
}

/*
 * EUC_KR ---> MIC
 */
static void
euc_kr2mic(unsigned char *euc, unsigned char *p, int len)
{
	int			c1;

	while (len > 0 && (c1 = *euc++))
	{
		if (c1 & 0x80)
		{
			len -= 2;
			*p++ = LC_KS5601;
			*p++ = c1;
			*p++ = *euc++;
		}
		else
		{						/* should be ASCII */
			len--;
			*p++ = c1;
		}
	}
	*p = '\0';
}

/*
 * MIC ---> EUC_KR
 */
static void
mic2euc_kr(unsigned char *mic, unsigned char *p, int len)
{
	int			c1;

	while (len > 0 && (c1 = *mic))
	{
		len -= pg_mic_mblen(mic++);

		if (c1 == LC_KS5601)
		{
			*p++ = *mic++;
			*p++ = *mic++;
		}
		else if (c1 > 0x7f)
		{						/* cannot convert to EUC_KR! */
			mic--;
			printBogusChar(&mic, &p);
		}
		else
		{						/* should be ASCII */
			*p++ = c1;
		}
	}
	*p = '\0';
}

/*
 * EUC_CN ---> MIC
 */
static void
euc_cn2mic(unsigned char *euc, unsigned char *p, int len)
{
	int			c1;

	while (len > 0 && (c1 = *euc++))
	{
		if (c1 & 0x80)
		{
			len -= 2;
			*p++ = LC_GB2312_80;
			*p++ = c1;
			*p++ = *euc++;
		}
		else
		{						/* should be ASCII */
			len--;
			*p++ = c1;
		}
	}
	*p = '\0';
}

/*
 * MIC ---> EUC_CN
 */
static void
mic2euc_cn(unsigned char *mic, unsigned char *p, int len)
{
	int			c1;

	while (len > 0 && (c1 = *mic))
	{
		len -= pg_mic_mblen(mic++);

		if (c1 == LC_GB2312_80)
		{
			*p++ = *mic++;
			*p++ = *mic++;
		}
		else if (c1 > 0x7f)
		{						/* cannot convert to EUC_CN! */
			mic--;
			printBogusChar(&mic, &p);
		}
		else
		{						/* should be ASCII */
			*p++ = c1;
		}
	}
	*p = '\0';
}

/*
 * EUC_TW ---> MIC
 */
static void
euc_tw2mic(unsigned char *euc, unsigned char *p, int len)
{
	int			c1;

	while (len > 0 && (c1 = *euc++))
	{
		if (c1 == SS2)
		{
			len -= 4;
			c1 = *euc++;		/* plane No. */
			if (c1 == 0xa1)
				*p++ = LC_CNS11643_1;
			else if (c1 == 0xa2)
				*p++ = LC_CNS11643_2;
			else
			{
				*p++ = 0x9d;	/* LCPRV2 */
				*p++ = 0xa3 - c1 + LC_CNS11643_3;
			}
			*p++ = *euc++;
			*p++ = *euc++;
		}
		else if (c1 & 0x80)
		{						/* CNS11643-1 */
			len -= 2;
			*p++ = LC_CNS11643_1;
			*p++ = c1;
			*p++ = *euc++;
		}
		else
		{						/* should be ASCII */
			len--;
			*p++ = c1;
		}
	}
	*p = '\0';
}

/*
 * MIC ---> EUC_TW
 */
static void
mic2euc_tw(unsigned char *mic, unsigned char *p, int len)
{
	int			c1;

	while (len > 0 && (c1 = *mic))
	{
		len -= pg_mic_mblen(mic++);

		if (c1 == LC_CNS11643_1)
		{
			*p++ = *mic++;
			*p++ = *mic++;
		}
		else if (c1 == LC_CNS11643_2)
		{
			*p++ = SS2;
			*p++ = 0xa2;
			*p++ = *mic++;
			*p++ = *mic++;
		}
		else if (c1 == 0x9d)
		{						/* LCPRV2? */
			*p++ = SS2;
			*p++ = *mic++ - LC_CNS11643_3 + 0xa3;
			*p++ = *mic++;
			*p++ = *mic++;
		}
		else if (c1 > 0x7f)
		{						/* cannot convert to EUC_TW! */
			mic--;
			printBogusChar(&mic, &p);
		}
		else
		{						/* should be ASCII */
			*p++ = c1;
		}
	}
	*p = '\0';
}

/*
 * Big5 ---> MIC
 */
static void
big52mic(unsigned char *big5, unsigned char *p, int len)
{
	unsigned short c1;
	unsigned short big5buf,
				cnsBuf;
	unsigned char lc;
	char		bogusBuf[3];
	int			i;

	while (len > 0 && (c1 = *big5++))
	{
		if (c1 <= 0x7fU)
		{						/* ASCII */
			len--;
			*p++ = c1;
		}
		else
		{
			len -= 2;
			big5buf = c1 << 8;
			c1 = *big5++;
			big5buf |= c1;
			cnsBuf = BIG5toCNS(big5buf, &lc);
			if (lc != 0)
			{
				if (lc == LC_CNS11643_3 || lc == LC_CNS11643_4)
				{
					*p++ = 0x9d;	/* LCPRV2 */
				}
				*p++ = lc;		/* Plane No. */
				*p++ = (cnsBuf >> 8) & 0x00ff;
				*p++ = cnsBuf & 0x00ff;
			}
			else
			{					/* cannot convert */
				big5 -= 2;
				*p++ = '(';
				for (i = 0; i < 2; i++)
				{
					sprintf(bogusBuf, "%02x", *big5++);
					*p++ = bogusBuf[0];
					*p++ = bogusBuf[1];
				}
				*p++ = ')';
			}
		}
	}
	*p = '\0';
}

/*
 * MIC ---> Big5
 */
static void
mic2big5(unsigned char *mic, unsigned char *p, int len)
{
	int			l;
	unsigned short c1;
	unsigned short big5buf,
				cnsBuf;

	while (len > 0 && (c1 = *mic))
	{
		l = pg_mic_mblen(mic++);
		len -= l;

		/* 0x9d means LCPRV2 */
		if (c1 == LC_CNS11643_1 || c1 == LC_CNS11643_2 || c1 == 0x9d)
		{
			if (c1 == 0x9d)
			{
				c1 = *mic++;	/* get plane no. */
			}
			cnsBuf = (*mic++) << 8;
			cnsBuf |= (*mic++) & 0x00ff;
			big5buf = CNStoBIG5(cnsBuf, c1);
			if (big5buf == 0)
			{					/* cannot convert to Big5! */
				mic -= l;
				printBogusChar(&mic, &p);
			}
			else
			{
				*p++ = (big5buf >> 8) & 0x00ff;
				*p++ = big5buf & 0x00ff;
			}
		}
		else if (c1 <= 0x7f)	/* ASCII */
			*p++ = c1;
		else
		{						/* cannot convert to Big5! */
			mic--;
			printBogusChar(&mic, &p);
		}
	}
	*p = '\0';
}

/*
 * LATINn ---> MIC
 */
static void
latin2mic(unsigned char *l, unsigned char *p, int len, int lc)
{
	int			c1;

	while (len-- > 0 && (c1 = *l++))
	{
		if (c1 > 0x7f)
		{						/* Latin1? */
			*p++ = lc;
		}
		*p++ = c1;
	}
	*p = '\0';
}

/*
 * MIC ---> LATINn
 */
static void
mic2latin(unsigned char *mic, unsigned char *p, int len, int lc)
{
	int			c1;

	while (len > 0 && (c1 = *mic))
	{
		len -= pg_mic_mblen(mic++);

		if (c1 == lc)
			*p++ = *mic++;
		else if (c1 > 0x7f)
		{
			mic--;
			printBogusChar(&mic, &p);
		}
		else
		{						/* should be ASCII */
			*p++ = c1;
		}
	}
	*p = '\0';
}

static void
latin12mic(unsigned char *l, unsigned char *p, int len)
{
	latin2mic(l, p, len, LC_ISO8859_1);
}
static void
mic2latin1(unsigned char *mic, unsigned char *p, int len)
{
	mic2latin(mic, p, len, LC_ISO8859_1);
}
static void
latin22mic(unsigned char *l, unsigned char *p, int len)
{
	latin2mic(l, p, len, LC_ISO8859_2);
}
static void
mic2latin2(unsigned char *mic, unsigned char *p, int len)
{
	mic2latin(mic, p, len, LC_ISO8859_2);
}
static void
latin32mic(unsigned char *l, unsigned char *p, int len)
{
	latin2mic(l, p, len, LC_ISO8859_3);
}
static void
mic2latin3(unsigned char *mic, unsigned char *p, int len)
{
	mic2latin(mic, p, len, LC_ISO8859_3);
}
static void
latin42mic(unsigned char *l, unsigned char *p, int len)
{
	latin2mic(l, p, len, LC_ISO8859_4);
}
static void
mic2latin4(unsigned char *mic, unsigned char *p, int len)
{
	mic2latin(mic, p, len, LC_ISO8859_4);
}

#ifdef NOT_USED
static void
latin52mic(unsigned char *l, unsigned char *p, int len)
{
	latin2mic(l, p, len, LC_ISO8859_5);
}
static void
mic2latin5(unsigned char *mic, unsigned char *p, int len)
{
	mic2latin(mic, p, len, LC_ISO8859_5);
}
#endif

/*
 * ASCII ---> MIC
 */
static void
ascii2mic(unsigned char *l, unsigned char *p, int len)
{
	int			c1;

	while (len-- > 0 && (c1 = *l++))
		*p++ = (c1 & 0x7f);
	*p = '\0';
}

/*
 * MIC ---> ASCII
 */
static void
mic2ascii(unsigned char *mic, unsigned char *p, int len)
{
	int			c1;

	while (len-- > 0 && (c1 = *mic))
	{
		if (c1 > 0x7f)
			printBogusChar(&mic, &p);
		else
		{						/* should be ASCII */
			*p++ = c1;
			mic++;
		}
	}
	*p = '\0';
}

/*
 * Cyrillic support
 * currently supported Cyrillic encodings:
 *
 * KOI8-R (this is the charset for the mule internal code
 *		for Cyrillic)
 * ISO-8859-5
 * Microsoft's CP1251(windows-1251)
 * Alternativny Variant (MS-DOS CP866)
 */

/* koi8r2mic: KOI8-R to Mule internal code */
static void
koi8r2mic(unsigned char *l, unsigned char *p, int len)
{
	latin2mic(l, p, len, LC_KOI8_R);
}

/* mic2koi8r: Mule internal code to KOI8-R */
static void
mic2koi8r(unsigned char *mic, unsigned char *p, int len)
{
	mic2latin(mic, p, len, LC_KOI8_R);
}

/*
 * latin2mic_with_table: a generic single byte charset encoding
 * conversion from a local charset to the mule internal code.
 * with a encoding conversion table.
 * the table is ordered according to the local charset,
 * starting from 128 (0x80). each entry in the table
 * holds the corresponding code point for the mule internal code.
 */
static void
latin2mic_with_table(
					 unsigned char *l,	/* local charset string (source) */
					 unsigned char *p,	/* pointer to store mule internal
										 * code (destination) */
					 int len,	/* length of l */
					 int lc,	/* leading character of p */
					 unsigned char *tab /* code conversion table */
)
{
	unsigned char c1,
				c2;

	while (len-- > 0 && (c1 = *l++))
	{
		if (c1 < 128)
			*p++ = c1;
		else
		{
			c2 = tab[c1 - 128];
			if (c2)
			{
				*p++ = lc;
				*p++ = c2;
			}
			else
			{
				*p++ = ' ';		/* cannot convert */
			}
		}
	}
	*p = '\0';
}

/*
 * mic2latin_with_table: a generic single byte charset encoding
 * conversion from the mule internal code to a local charset
 * with a encoding conversion table.
 * the table is ordered according to the second byte of the mule
 * internal code starting from 128 (0x80).
 * each entry in the table
 * holds the corresponding code point for the local code.
 */
static void
mic2latin_with_table(
					 unsigned char *mic,		/* mule internal code
												 * (source) */
					 unsigned char *p,	/* local code (destination) */
					 int len,	/* length of p */
					 int lc,	/* leading character */
					 unsigned char *tab /* code conversion table */
)
{

	unsigned char c1,
				c2;

	while (len-- > 0 && (c1 = *mic++))
	{
		if (c1 < 128)
			*p++ = c1;
		else if (c1 == lc)
		{
			c1 = *mic++;
			len--;
			c2 = tab[c1 - 128];
			if (c2)
				*p++ = c2;
			else
			{
				*p++ = ' ';		/* cannot convert */
			}
		}
		else
		{
			*p++ = ' ';			/* bogus character */
		}
	}
	*p = '\0';
}

/* iso2mic: ISO-8859-5 to Mule internal code */
static void
iso2mic(unsigned char *l, unsigned char *p, int len)
{
	static unsigned char iso2koi[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xe1, 0xe2, 0xf7, 0xe7, 0xe4, 0xe5, 0xf6, 0xfa,
		0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0,
		0xf2, 0xf3, 0xf4, 0xf5, 0xe6, 0xe8, 0xe3, 0xfe,
		0xfb, 0xfd, 0xff, 0xf9, 0xf8, 0xfc, 0xe0, 0xf1,
		0xc1, 0xc2, 0xd7, 0xc7, 0xc4, 0xc5, 0xd6, 0xda,
		0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
		0xd2, 0xd3, 0xd4, 0xd5, 0xc6, 0xc8, 0xc3, 0xde,
		0xdb, 0xdd, 0xdf, 0xd9, 0xd8, 0xdc, 0xc0, 0xd1,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	latin2mic_with_table(l, p, len, LC_KOI8_R, iso2koi);
}

/* mic2iso: Mule internal code to ISO8859-5 */
static void
mic2iso(unsigned char *mic, unsigned char *p, int len)
{
	static unsigned char koi2iso[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xee, 0xd0, 0xd1, 0xe6, 0xd4, 0xd5, 0xe4, 0xd3,
		0xe5, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde,
		0xdf, 0xef, 0xe0, 0xe1, 0xe2, 0xe3, 0xd6, 0xd2,
		0xec, 0xeb, 0xd7, 0xe8, 0xed, 0xe9, 0xe7, 0xea,
		0xce, 0xb0, 0xb1, 0xc6, 0xb4, 0xb5, 0xc4, 0xb3,
		0xc5, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe,
		0xbf, 0xcf, 0xc0, 0xc1, 0xc2, 0xc3, 0xb6, 0xb2,
		0xcc, 0xcb, 0xb7, 0xc8, 0xcd, 0xc9, 0xc7, 0xca
	};

	mic2latin_with_table(mic, p, len, LC_KOI8_R, koi2iso);
}

/* win2mic: CP1251 to Mule internal code */
static void
win12512mic(unsigned char *l, unsigned char *p, int len)
{
	static unsigned char win2koi[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x00, 0x00,
		0xb3, 0x00, 0xb4, 0x00, 0x00, 0x00, 0x00, 0xb7,
		0x00, 0x00, 0xb6, 0xa6, 0xad, 0x00, 0x00, 0x00,
		0xa3, 0x00, 0xa4, 0x00, 0x00, 0x00, 0x00, 0xa7,
		0xe1, 0xe2, 0xf7, 0xe7, 0xe4, 0xe5, 0xf6, 0xfa,
		0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0,
		0xf2, 0xf3, 0xf4, 0xf5, 0xe6, 0xe8, 0xe3, 0xfe,
		0xfb, 0xfd, 0xff, 0xf9, 0xf8, 0xfc, 0xe0, 0xf1,
		0xc1, 0xc2, 0xd7, 0xc7, 0xc4, 0xc5, 0xd6, 0xda,
		0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
		0xd2, 0xd3, 0xd4, 0xd5, 0xc6, 0xc8, 0xc3, 0xde,
		0xdb, 0xdd, 0xdf, 0xd9, 0xd8, 0xdc, 0xc0, 0xd1
	};

	latin2mic_with_table(l, p, len, LC_KOI8_R, win2koi);
}

/* mic2win: Mule internal code to CP1251 */
static void
mic2win1251(unsigned char *mic, unsigned char *p, int len)
{
	static unsigned char koi2win[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xb8, 0xba, 0x00, 0xb3, 0xbf,
		0x00, 0x00, 0x00, 0x00, 0x00, 0xb4, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xa8, 0xaa, 0x00, 0xb2, 0xaf,
		0x00, 0x00, 0x00, 0x00, 0x00, 0xa5, 0x00, 0x00,
		0xfe, 0xe0, 0xe1, 0xf6, 0xe4, 0xe5, 0xf4, 0xe3,
		0xf5, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee,
		0xef, 0xff, 0xf0, 0xf1, 0xf2, 0xf3, 0xe6, 0xe2,
		0xfc, 0xfb, 0xe7, 0xf8, 0xfd, 0xf9, 0xf7, 0xfa,
		0xde, 0xc0, 0xc1, 0xd6, 0xc4, 0xc5, 0xd4, 0xc3,
		0xd5, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce,
		0xcf, 0xdf, 0xd0, 0xd1, 0xd2, 0xd3, 0xc6, 0xc2,
		0xdc, 0xdb, 0xc7, 0xd8, 0xdd, 0xd9, 0xd7, 0xda
	};

	mic2latin_with_table(mic, p, len, LC_KOI8_R, koi2win);
}

/* alt2mic: CP866 to Mule internal code */
static void
alt2mic(unsigned char *l, unsigned char *p, int len)
{
	static unsigned char alt2koi[] = {
		0xe1, 0xe2, 0xf7, 0xe7, 0xe4, 0xe5, 0xf6, 0xfa,
		0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0,
		0xf2, 0xf3, 0xf4, 0xf5, 0xe6, 0xe8, 0xe3, 0xfe,
		0xfb, 0xfd, 0xff, 0xf9, 0xf8, 0xfc, 0xe0, 0xf1,
		0xc1, 0xc2, 0xd7, 0xc7, 0xc4, 0xc5, 0xd6, 0xda,
		0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0xd2, 0xd3, 0xd4, 0xd5, 0xc6, 0xc8, 0xc3, 0xde,
		0xdb, 0xdd, 0xdf, 0xd9, 0xd8, 0xdc, 0xc0, 0xd1,
		0xb3, 0xa3, 0xb4, 0xa4, 0xb7, 0xa7, 0x00, 0x00,
		0xb6, 0xa6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	latin2mic_with_table(l, p, len, LC_KOI8_R, alt2koi);
}

/* mic2alt: Mule internal code to CP866 */
static void
mic2alt(unsigned char *mic, unsigned char *p, int len)
{
	static unsigned char koi2alt[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xf1, 0xf3, 0x00, 0xf9, 0xf5,
		0x00, 0x00, 0x00, 0x00, 0x00, 0xad, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xf0, 0xf2, 0x00, 0xf8, 0xf4,
		0x00, 0x00, 0x00, 0x00, 0x00, 0xbd, 0x00, 0x00,
		0xee, 0xa0, 0xa1, 0xe6, 0xa4, 0xa5, 0xe4, 0xa3,
		0xe5, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae,
		0xaf, 0xef, 0xe0, 0xe1, 0xe2, 0xe3, 0xa6, 0xa2,
		0xec, 0xeb, 0xa7, 0xe8, 0xed, 0xe9, 0xe7, 0xea,
		0x9e, 0x80, 0x81, 0x96, 0x84, 0x85, 0x94, 0x83,
		0x95, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e,
		0x8f, 0x9f, 0x90, 0x91, 0x92, 0x93, 0x86, 0x82,
		0x9c, 0x9b, 0x87, 0x98, 0x9d, 0x99, 0x97, 0x9a
	};

	mic2latin_with_table(mic, p, len, LC_KOI8_R, koi2alt);
}

/*
 * end of Cyrillic support
 */


/*-----------------------------------------------------------------
 * WIN1250
 * Microsoft's CP1250(windows-1250)
 *-----------------------------------------------------------------*/
static void
win12502mic(unsigned char *l, unsigned char *p, int len)
{
	static unsigned char win1250_2_iso88592[] = {
		0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
		0x88, 0x89, 0xA9, 0x8B, 0xA6, 0xAB, 0xAE, 0xAC,
		0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
		0x98, 0x99, 0xB9, 0x9B, 0xB6, 0xBB, 0xBE, 0xBC,
		0xA0, 0xB7, 0xA2, 0xA3, 0xA4, 0xA1, 0x00, 0xA7,
		0xA8, 0x00, 0xAA, 0x00, 0x00, 0xAD, 0x00, 0xAF,
		0xB0, 0x00, 0xB2, 0xB3, 0xB4, 0x00, 0x00, 0x00,
		0xB8, 0xB1, 0xBA, 0x00, 0xA5, 0xBD, 0xB5, 0xBF,
		0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
		0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
		0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
		0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
		0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
		0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
		0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
		0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
	};

	latin2mic_with_table(l, p, len, LC_ISO8859_2, win1250_2_iso88592);
}
static void
mic2win1250(unsigned char *mic, unsigned char *p, int len)
{
	static unsigned char iso88592_2_win1250[] = {
		0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
		0x88, 0x89, 0x00, 0x8B, 0x00, 0x00, 0x00, 0x00,
		0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
		0x98, 0x99, 0x00, 0x9B, 0x00, 0x00, 0x00, 0x00,
		0xA0, 0xA5, 0xA2, 0xA3, 0xA4, 0xBC, 0x8C, 0xA7,
		0xA8, 0x8A, 0xAA, 0x8D, 0x8F, 0xAD, 0x8E, 0xAF,
		0xB0, 0xB9, 0xB2, 0xB3, 0xB4, 0xBE, 0x9C, 0xA1,
		0xB8, 0x9A, 0xBA, 0x9D, 0x9F, 0xBD, 0x9E, 0xBF,
		0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
		0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
		0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7,
		0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
		0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
		0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
		0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
		0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
	};

	mic2latin_with_table(mic, p, len, LC_ISO8859_2, iso88592_2_win1250);
}

#ifdef UNICODE_CONVERSION

/*
 * UNICODE(UTF-8) support
 */

/*
 * ASCII ---> UTF-8
 */
static void
ascii2utf(unsigned char *ascii, unsigned char *utf, int len)
{
	ascii2mic(ascii, utf, len);
}

/*
 * UTF-8 ---> ASCII
 */
static void
utf2ascii(unsigned char *utf, unsigned char *ascii, int len)
{
	mic2ascii(utf, ascii, len);
}

/*
 * ISO8859-1 ---> UTF-8
 */
static void
iso8859_1_to_utf(unsigned char *iso, unsigned char *utf, int len)
{
	unsigned short c;

	while (len-- > 0 && (c = *iso++))
	{
		if (c < 0x80)
			*utf++ = c;
		else
		{
			*utf++ = (c >> 6) | 0xc0;
			*utf++ = (c & 0x003f) | 0x80;
		}
	}
	*utf = '\0';
}

/*
 * UTF-8 ---> ISO8859-1
 */
static void
utf_to_iso8859_1(unsigned char *utf, unsigned char *iso, int len)
{
	unsigned short c,
				c1,
				c2;

	while (len > 0 && (c = *utf++))
	{
		if ((c & 0xe0) == 0xc0)
		{
			c1 = c & 0x1f;
			c2 = *utf++ & 0x3f;
			*iso = c1 << 6;
			*iso++ |= c2;
			len -= 2;
		}
		else if ((c & 0xe0) == 0xe0)
			elog(ERROR, "Could not convert UTF-8 to ISO8859-1");
		else
		{
			*iso++ = c;
			len--;
		}
	}
	*iso = '\0';
}

/*
 * comparison routine for bsearch()
 * this routine is intended for UTF-8 -> local code
 */
static int
compare1(const void *p1, const void *p2)
{
	unsigned int v1,
				v2;

	v1 = *(unsigned int *) p1;
	v2 = ((pg_utf_to_local *) p2)->utf;
	return (v1 - v2);
}

/*
 * comparison routine for bsearch()
 * this routine is intended for local code -> UTF-8
 */
static int
compare2(const void *p1, const void *p2)
{
	unsigned int v1,
				v2;

	v1 = *(unsigned int *) p1;
	v2 = ((pg_local_to_utf *) p2)->code;
	return (v1 - v2);
}

/*
 * UTF-8 ---> local code
 *
 * utf: input UTF-8 string. Its length is limited by "len" parameter
 *		or a null terminater.
 * iso: pointer to the output.
 * map: the conversion map.
 * size: the size of the conversion map.
 */
static void
utf_to_local(unsigned char *utf, unsigned char *iso,
			 pg_utf_to_local *map, int size, int len)
{
	unsigned int iutf;
	int			l;
	pg_utf_to_local *p;

	for (; len > 0 && *utf; len -= l)
	{
		l = pg_utf_mblen(utf);
		if (l == 1)
		{
			*iso++ = *utf++;
			continue;
		}
		else if (l == 2)
		{
			iutf = *utf++ << 8;
			iutf |= *utf++;
		}
		else
		{
			iutf = *utf++ << 16;
			iutf |= *utf++ << 8;
			iutf |= *utf++;
		}
		p = bsearch(&iutf, map, size,
					sizeof(pg_utf_to_local), compare1);
		if (p == NULL)
		{
			elog(NOTICE, "utf_to_local: could not convert UTF-8 (0x%04x). Ignored", iutf);
			continue;
		}
		if (p->code & 0xff000000)
			*iso++ = p->code >> 24;
		if (p->code & 0x00ff0000)
			*iso++ = (p->code & 0x00ff0000) >> 16;
		if (p->code & 0x0000ff00)
			*iso++ = (p->code & 0x0000ff00) >> 8;
		if (p->code & 0x000000ff)
			*iso++ = p->code & 0x000000ff;
	}
	*iso = '\0';
}

#ifdef NOT_USED
/*
 * UTF-8 ---> ISO8859-2
 */
static void
utf_to_latin2(unsigned char *utf, unsigned char *iso, int len)
{
	utf_to_local(utf, iso, ULmapISO8859_2, sizeof(ULmapISO8859_2) / sizeof(pg_utf_to_local), len);
}

/*
 * UTF-8 ---> ISO8859-3
 */
static void
utf_to_latin3(unsigned char *utf, unsigned char *iso, int len)
{
	utf_to_local(utf, iso, ULmapISO8859_3, sizeof(ULmapISO8859_3) / sizeof(pg_utf_to_local), len);
}

/*
 * UTF-8 ---> ISO8859-4
 */
static void
utf_to_latin4(unsigned char *utf, unsigned char *iso, int len)
{
	utf_to_local(utf, iso, ULmapISO8859_4, sizeof(ULmapISO8859_4) / sizeof(pg_utf_to_local), len);
}

/*
 * UTF-8 ---> ISO8859-5
 */
static void
utf_to_latin5(unsigned char *utf, unsigned char *iso, int len)
{
	utf_to_local(utf, iso, ULmapISO8859_5, sizeof(ULmapISO8859_5) / sizeof(pg_utf_to_local), len);
}
#endif   /* NOT_USED */

/*
 * Cyrillic charsets
 */

/*
 * UTF-8 --->KOI8-R
 */
static void
utf_to_KOI8R(unsigned char *utf, unsigned char *iso, int len)

{
	utf_to_local(utf, iso, ULmap_KOI8R, sizeof(ULmap_KOI8R) / sizeof(pg_utf_to_local), len);
}

/*
 * UTF-8 --->WIN1251
 */
static void
utf_to_WIN1251(unsigned char *utf, unsigned char *iso, int len)

{
	utf_to_local(utf, iso, ULmap_WIN1251, sizeof(ULmap_WIN1251) / sizeof(pg_utf_to_local), len);
}

/*
 * UTF-8 --->ALT
 */
static void
utf_to_ALT(unsigned char *utf, unsigned char *iso, int len)

{
	utf_to_local(utf, iso, ULmap_ALT, sizeof(ULmap_ALT) / sizeof(pg_utf_to_local), len);
}

/*
 * local code ---> UTF-8
 */
static void
local_to_utf(unsigned char *iso, unsigned char *utf,
			 pg_local_to_utf *map, int size, int encoding, int len)
{
	unsigned int iiso;
	int			l;
	pg_local_to_utf *p;

	if (!PG_VALID_ENCODING(encoding))
		elog(ERROR, "Invalid encoding number %d", encoding);

	for (; len > 0 && *iso; len -= l)
	{
		if (*iso < 0x80)
		{
			*utf++ = *iso++;
			l = 1;
			continue;
		}

		l = pg_encoding_mblen(encoding, iso);

		if (l == 1)
			iiso = *iso++;
		else if (l == 2)
		{
			iiso = *iso++ << 8;
			iiso |= *iso++;
		}
		else if (l == 3)
		{
			iiso = *iso++ << 16;
			iiso |= *iso++ << 8;
			iiso |= *iso++;
		}
		else if (l == 4)
		{
			iiso = *iso++ << 24;
			iiso |= *iso++ << 16;
			iiso |= *iso++ << 8;
			iiso |= *iso++;
		}
		p = bsearch(&iiso, map, size,
					sizeof(pg_local_to_utf), compare2);
		if (p == NULL)
		{
			elog(NOTICE, "local_to_utf: could not convert (0x%04x) %s to UTF-8. Ignored",
				 iiso, (&pg_enc2name_tbl[encoding])->name);
			continue;
		}
		if (p->utf & 0xff000000)
			*utf++ = p->utf >> 24;
		if (p->utf & 0x00ff0000)
			*utf++ = (p->utf & 0x00ff0000) >> 16;
		if (p->utf & 0x0000ff00)
			*utf++ = (p->utf & 0x0000ff00) >> 8;
		if (p->utf & 0x000000ff)
			*utf++ = p->utf & 0x000000ff;
	}
	*utf = '\0';
}

#ifdef NOT_USED
/*
 * ISO-8859-2 ---> UTF-8
 */
static void
latin2_to_utf(unsigned char *iso, unsigned char *utf, int len)
{
	local_to_utf(iso, utf, LUmapISO8859_2, sizeof(LUmapISO8859_2) / sizeof(pg_local_to_utf), PG_LATIN2, len);
}

/*
 * ISO-8859-3 ---> UTF-8
 */
static void
latin3_to_utf(unsigned char *iso, unsigned char *utf, int len)
{
	local_to_utf(iso, utf, LUmapISO8859_3, sizeof(LUmapISO8859_3) / sizeof(pg_local_to_utf), PG_LATIN3, len);
}

/*
 * ISO-8859-4 ---> UTF-8
 */
static void
latin4_to_utf(unsigned char *iso, unsigned char *utf, int len)
{
	local_to_utf(iso, utf, LUmapISO8859_4, sizeof(LUmapISO8859_4) / sizeof(pg_local_to_utf), PG_LATIN4, len);
}

/*
 * ISO-8859-5 ---> UTF-8
 */
static void
latin5_to_utf(unsigned char *iso, unsigned char *utf, int len)
{
	local_to_utf(iso, utf, LUmapISO8859_5, sizeof(LUmapISO8859_5) / sizeof(pg_local_to_utf), PG_LATIN5, len);
}
#endif   /* NOT_USED */

#define UTF_ISO8859(_id_) \
static void \
utf_to_iso8859_##_id_(unsigned char *utf, unsigned char *iso, int len) \
{ \
	utf_to_local(utf, iso, ULmapISO8859_##_id_, sizeof(ULmapISO8859_##_id_) / sizeof(pg_utf_to_local), len); \
} \
static void \
iso8859_##_id_##_to_utf(unsigned char *iso, unsigned char *utf, int len) \
{ \
	local_to_utf(iso, utf, LUmapISO8859_##_id_, sizeof(LUmapISO8859_##_id_) / sizeof(pg_local_to_utf), PG_LATIN1, len); \
}

UTF_ISO8859(2);
UTF_ISO8859(3);
UTF_ISO8859(4);
UTF_ISO8859(5);
UTF_ISO8859(6);
UTF_ISO8859(7);
UTF_ISO8859(8);
UTF_ISO8859(9);
UTF_ISO8859(10);
UTF_ISO8859(13);
UTF_ISO8859(14);
UTF_ISO8859(15);
UTF_ISO8859(16);

/*
 * KOI8-R ---> UTF-8
 */
static void
KOI8R_to_utf(unsigned char *iso, unsigned char *utf, int len)
{
	local_to_utf(iso, utf, LUmapKOI8R, sizeof(LUmapKOI8R) / sizeof(pg_local_to_utf), PG_KOI8R, len);
}

/*
 * WIN1251 ---> UTF-8
 */
static void
WIN1251_to_utf(unsigned char *iso, unsigned char *utf, int len)
{
	local_to_utf(iso, utf, LUmapWIN1251, sizeof(LUmapWIN1251) / sizeof(pg_local_to_utf), PG_WIN1251, len);
}

/*
 * ALT ---> UTF-8
 */
static void
ALT_to_utf(unsigned char *iso, unsigned char *utf, int len)
{
	local_to_utf(iso, utf, LUmapALT, sizeof(LUmapALT) / sizeof(pg_local_to_utf), PG_ALT, len);
}

/*
 * UTF-8 ---> EUC_JP
 */
static void
utf_to_euc_jp(unsigned char *utf, unsigned char *euc, int len)

{
	utf_to_local(utf, euc, ULmapEUC_JP,
				 sizeof(ULmapEUC_JP) / sizeof(pg_utf_to_local), len);
}

/*
 * EUC_JP ---> UTF-8
 */
static void
euc_jp_to_utf(unsigned char *euc, unsigned char *utf, int len)
{
	local_to_utf(euc, utf, LUmapEUC_JP,
		  sizeof(LUmapEUC_JP) / sizeof(pg_local_to_utf), PG_EUC_JP, len);
}

/*
 * UTF-8 ---> EUC_CN
 */
static void
utf_to_euc_cn(unsigned char *utf, unsigned char *euc, int len)

{
	utf_to_local(utf, euc, ULmapEUC_CN,
				 sizeof(ULmapEUC_CN) / sizeof(pg_utf_to_local), len);
}

/*
 * EUC_CN ---> UTF-8
 */
static void
euc_cn_to_utf(unsigned char *euc, unsigned char *utf, int len)
{
	local_to_utf(euc, utf, LUmapEUC_CN,
		  sizeof(LUmapEUC_CN) / sizeof(pg_local_to_utf), PG_EUC_CN, len);
}

/*
 * UTF-8 ---> EUC_KR
 */
static void
utf_to_euc_kr(unsigned char *utf, unsigned char *euc, int len)

{
	utf_to_local(utf, euc, ULmapEUC_KR,
				 sizeof(ULmapEUC_KR) / sizeof(pg_utf_to_local), len);
}

/*
 * EUC_KR ---> UTF-8
 */
static void
euc_kr_to_utf(unsigned char *euc, unsigned char *utf, int len)
{
	local_to_utf(euc, utf, LUmapEUC_KR,
		  sizeof(LUmapEUC_KR) / sizeof(pg_local_to_utf), PG_EUC_KR, len);
}

/*
 * UTF-8 ---> EUC_TW
 */
static void
utf_to_euc_tw(unsigned char *utf, unsigned char *euc, int len)

{
	utf_to_local(utf, euc, ULmapEUC_TW,
				 sizeof(ULmapEUC_TW) / sizeof(pg_utf_to_local), len);
}

/*
 * EUC_TW ---> UTF-8
 */
static void
euc_tw_to_utf(unsigned char *euc, unsigned char *utf, int len)
{
	local_to_utf(euc, utf, LUmapEUC_TW,
		  sizeof(LUmapEUC_TW) / sizeof(pg_local_to_utf), PG_EUC_TW, len);
}

/*
 * UTF-8 ---> SJIS
 */
static void
utf_to_sjis(unsigned char *utf, unsigned char *euc, int len)

{
	utf_to_local(utf, euc, ULmapSJIS,
				 sizeof(ULmapSJIS) / sizeof(pg_utf_to_local), len);
}

/*
 * SJIS ---> UTF-8
 */
static void
sjis_to_utf(unsigned char *euc, unsigned char *utf, int len)
{
	local_to_utf(euc, utf, LUmapSJIS,
			  sizeof(LUmapSJIS) / sizeof(pg_local_to_utf), PG_SJIS, len);
}

/*
 * UTF-8 ---> BIG5
 */
static void
utf_to_big5(unsigned char *utf, unsigned char *euc, int len)

{
	utf_to_local(utf, euc, ULmapBIG5,
				 sizeof(ULmapBIG5) / sizeof(pg_utf_to_local), len);
}

/*
 * BIG5 ---> UTF-8
 */
static void
big5_to_utf(unsigned char *euc, unsigned char *utf, int len)
{
	local_to_utf(euc, utf, LUmapBIG5,
			  sizeof(LUmapBIG5) / sizeof(pg_local_to_utf), PG_BIG5, len);
}

/* ----------
 * Encoding conversion routines
 *
 * WARINIG: must by same order as pg_enc in include/mb/pg_wchar.h!
 * ----------
 */
pg_enconv	pg_enconv_tbl[] =
{
	{
		PG_SQL_ASCII, ascii2mic, mic2ascii, ascii2utf, utf2ascii
	},
	{
		PG_EUC_JP, euc_jp2mic, mic2euc_jp, euc_jp_to_utf, utf_to_euc_jp
	},
	{
		PG_EUC_CN, euc_cn2mic, mic2euc_cn, euc_cn_to_utf, utf_to_euc_cn
	},
	{
		PG_EUC_KR, euc_kr2mic, mic2euc_kr, euc_kr_to_utf, utf_to_euc_kr
	},
	{
		PG_EUC_TW, euc_tw2mic, mic2euc_tw, euc_tw_to_utf, utf_to_euc_tw
	},
	{
		PG_UTF8, 0, 0, 0, 0
	},
	{
		PG_MULE_INTERNAL, 0, 0, 0, 0
	},
	{
		PG_LATIN1, latin12mic, mic2latin1, iso8859_1_to_utf, utf_to_iso8859_1
	},
	{
		PG_LATIN2, latin22mic, mic2latin2, iso8859_2_to_utf, utf_to_iso8859_2
	},
	{
		PG_LATIN3, latin32mic, mic2latin3, iso8859_3_to_utf, utf_to_iso8859_3
	},
	{
		PG_LATIN4, latin42mic, mic2latin4, iso8859_4_to_utf, utf_to_iso8859_4
	},
	{
		PG_LATIN5, iso2mic, mic2iso, iso8859_9_to_utf, utf_to_iso8859_9
	},
	{
		PG_LATIN6, 0, 0, iso8859_10_to_utf, utf_to_iso8859_10
	},
	{
		PG_LATIN7, 0, 0, iso8859_13_to_utf, utf_to_iso8859_13
	},
	{
		PG_LATIN8, 0, 0, iso8859_14_to_utf, utf_to_iso8859_14
	},
	{
		PG_LATIN9, 0, 0, iso8859_15_to_utf, utf_to_iso8859_15
	},
	{
		PG_LATIN10, 0, 0, iso8859_16_to_utf, utf_to_iso8859_16
	},
	{
		PG_KOI8R, koi8r2mic, mic2koi8r, KOI8R_to_utf, utf_to_KOI8R
	},
	{
		PG_WIN1251, win12512mic, mic2win1251, WIN1251_to_utf, utf_to_WIN1251
	},
	{
		PG_ALT, alt2mic, mic2alt, ALT_to_utf, utf_to_ALT
	},
	{
		PG_ISO_8859_5, 0, 0, iso8859_5_to_utf, utf_to_iso8859_5
	},
	{
		PG_ISO_8859_6, 0, 0, iso8859_6_to_utf, utf_to_iso8859_6
	},
	{
		PG_ISO_8859_7, 0, 0, iso8859_7_to_utf, utf_to_iso8859_7
	},
	{
		PG_ISO_8859_8, 0, 0, iso8859_8_to_utf, utf_to_iso8859_8
	},

	{
		PG_SJIS, sjis2mic, mic2sjis, sjis_to_utf, utf_to_sjis
	},
	{
		PG_BIG5, big52mic, mic2big5, big5_to_utf, utf_to_big5
	},
	{
		PG_WIN1250, win12502mic, mic2win1250, 0, 0
	},
};

#else

pg_enconv	pg_enconv_tbl[] =
{
	{
		PG_SQL_ASCII, ascii2mic, mic2ascii, 0, 0
	},
	{
		PG_EUC_JP, euc_jp2mic, mic2euc_jp, 0, 0
	},
	{
		PG_EUC_CN, euc_cn2mic, mic2euc_cn, 0, 0
	},
	{
		PG_EUC_KR, euc_kr2mic, mic2euc_kr, 0, 0
	},
	{
		PG_EUC_TW, euc_tw2mic, mic2euc_tw, 0, 0
	},
	{
		PG_UTF8, 0, 0, 0, 0
	},
	{
		PG_MULE_INTERNAL, 0, 0, 0, 0
	},
	{
		PG_LATIN1, latin12mic, mic2latin1, 0, 0
	},
	{
		PG_LATIN2, latin22mic, mic2latin2, 0, 0
	},
	{
		PG_LATIN3, latin32mic, mic2latin3, 0, 0
	},
	{
		PG_LATIN4, latin42mic, mic2latin4, 0, 0
	},
	{
		PG_LATIN5, iso2mic, mic2iso, 0, 0
	},
	{
		PG_LATIN6, 0, 0, 0, 0
	},
	{
		PG_LATIN7, 0, 0, 0, 0
	},
	{
		PG_LATIN8, 0, 0, 0, 0
	},
	{
		PG_LATIN9, 0, 0, 0, 0
	},
	{
		PG_LATIN10, 0, 0, 0, 0
	},
	{
		PG_KOI8R, koi8r2mic, mic2koi8r, 0, 0
	},
	{
		PG_WIN1251, win12512mic, mic2win1251, 0, 0
	},
	{
		PG_ALT, alt2mic, mic2alt, 0, 0
	},
	{
		PG_ISO_8859_5, 0, 0, 0, 0
	},
	{
		PG_ISO_8859_6, 0, 0, 0, 0
	},
	{
		PG_ISO_8859_7, 0, 0, 0, 0
	},
	{
		PG_ISO_8859_8, 0, 0, 0, 0
	},
	{
		PG_SJIS, sjis2mic, mic2sjis, 0, 0
	},
	{
		PG_BIG5, big52mic, mic2big5, 0, 0
	},
	{
		PG_WIN1250, win12502mic, mic2win1250, 0, 0
	},
};

#endif   /* UNICODE_CONVERSION */
