/*
 * Copyright (c) 1996 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static const char rcsid[] = "$Id: inet_net_pton.c,v 1.2 2004/07/05 08:55:39 SuLiang Exp $";
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <assert.h>
#include <ctype.h>
#include <errno.h>

#include "postgres.h"
#include "utils/builtins.h"

#ifdef SPRINTF_CHAR
#define SPRINTF(x) strlen(sprintf/**/x)
#else
#define SPRINTF(x) ((size_t)sprintf x)
#endif

static int	inet_net_pton_ipv4(const char *src, u_char *dst);
static int	inet_cidr_pton_ipv4(const char *src, u_char *dst, size_t size);

/*
 * static int
 * inet_net_pton(af, src, dst, size)
 *	convert network number from presentation to network format.
 *	accepts hex octets, hex strings, decimal octets, and /CIDR.
 *	"size" is in bytes and describes "dst".
 * return:
 *	number of bits, either imputed classfully or specified with /CIDR,
 *	or -1 if some failure occurred (check errno).  ENOENT means it was
 *	not a valid network specification.
 * author:
 *	Paul Vixie (ISC), June 1996
 *
 * Changes:
 *	I added the inet_cidr_pton function (also from Paul) and changed
 *	the names to reflect their current use.
 *
 */
int
inet_net_pton(int af, const char *src, void *dst, size_t size)
{
	switch (af)
	{
		case AF_INET:
			return size == -1 ?
				inet_net_pton_ipv4(src, dst) :
				inet_cidr_pton_ipv4(src, dst, size);
		default:
			errno = EAFNOSUPPORT;
			return (-1);
	}
}

/*
 * static int
 * inet_cidr_pton_ipv4(src, dst, size)
 *	convert IPv4 network number from presentation to network format.
 *	accepts hex octets, hex strings, decimal octets, and /CIDR.
 *	"size" is in bytes and describes "dst".
 * return:
 *	number of bits, either imputed classfully or specified with /CIDR,
 *	or -1 if some failure occurred (check errno).  ENOENT means it was
 *	not an IPv4 network specification.
 * note:
 *	network byte order assumed.  this means 192.5.5.240/28 has
 *	0x11110000 in its fourth octet.
 * author:
 *	Paul Vixie (ISC), June 1996
 */
static int
inet_cidr_pton_ipv4(const char *src, u_char *dst, size_t size)
{
	static const char
				xdigits[] = "0123456789abcdef",
				digits[] = "0123456789";
	int			n,
				ch,
				tmp,
				dirty,
				bits;
	const u_char *odst = dst;

	ch = *src++;
	if (ch == '0' && (src[0] == 'x' || src[0] == 'X')
		&& isxdigit((unsigned char) src[1]))
	{
		/* Hexadecimal: Eat nybble string. */
		if (size <= 0)
			goto emsgsize;
		dirty = 0;
		tmp = 0;
		src++;					/* skip x or X. */
		while ((ch = *src++) != '\0' && isxdigit((unsigned char) ch))
		{
			if (isupper((unsigned char) ch))
				ch = tolower((unsigned char) ch);
			n = strchr(xdigits, ch) - xdigits;
			assert(n >= 0 && n <= 15);
			if (dirty == 0)
				tmp = n;
			else
				tmp = (tmp << 4) | n;
			if (++dirty == 2)
			{
				if (size-- <= 0)
					goto emsgsize;
				*dst++ = (u_char) tmp;
				dirty = 0;
			}
		}
		if (dirty)
		{						/* Odd trailing nybble? */
			if (size-- <= 0)
				goto emsgsize;
			*dst++ = (u_char) (tmp << 4);
		}
	}
	else if (isdigit((unsigned char) ch))
	{
		/* Decimal: eat dotted digit string. */
		for (;;)
		{
			tmp = 0;
			do
			{
				n = strchr(digits, ch) - digits;
				assert(n >= 0 && n <= 9);
				tmp *= 10;
				tmp += n;
				if (tmp > 255)
					goto enoent;
			} while ((ch = *src++) != '\0' &&
					 isdigit((unsigned char) ch));
			if (size-- <= 0)
				goto emsgsize;
			*dst++ = (u_char) tmp;
			if (ch == '\0' || ch == '/')
				break;
			if (ch != '.')
				goto enoent;
			ch = *src++;
			if (!isdigit((unsigned char) ch))
				goto enoent;
		}
	}
	else
		goto enoent;

	bits = -1;
	if (ch == '/' && isdigit((unsigned char) src[0]) && dst > odst)
	{
		/* CIDR width specifier.  Nothing can follow it. */
		ch = *src++;			/* Skip over the /. */
		bits = 0;
		do
		{
			n = strchr(digits, ch) - digits;
			assert(n >= 0 && n <= 9);
			bits *= 10;
			bits += n;
		} while ((ch = *src++) != '\0' && isdigit((unsigned char) ch));
		if (ch != '\0')
			goto enoent;
		if (bits > 32)
			goto emsgsize;
	}

	/* Firey death and destruction unless we prefetched EOS. */
	if (ch != '\0')
		goto enoent;

	/* If nothing was written to the destination, we found no address. */
	if (dst == odst)
		goto enoent;
	/* If no CIDR spec was given, infer width from net class. */
	if (bits == -1)
	{
		if (*odst >= 240)		/* Class E */
			bits = 32;
		else if (*odst >= 224)	/* Class D */
			bits = 4;
		else if (*odst >= 192)	/* Class C */
			bits = 24;
		else if (*odst >= 128)	/* Class B */
			bits = 16;
		else
/* Class A */
			bits = 8;
		/* If imputed mask is narrower than specified octets, widen. */
		if (bits >= 8 && bits < ((dst - odst) * 8))
			bits = (dst - odst) * 8;
	}
	/* Extend network to cover the actual mask. */
	while (bits > ((dst - odst) * 8))
	{
		if (size-- <= 0)
			goto emsgsize;
		*dst++ = '\0';
	}
	return (bits);

enoent:
	errno = ENOENT;
	return (-1);

emsgsize:
	errno = EMSGSIZE;
	return (-1);
}

/*
 * int
 * inet_net_pton(af, src, dst, *bits)
 *	convert network address from presentation to network format.
 *	accepts inet_pton()'s input for this "af" plus trailing "/CIDR".
 *	"dst" is assumed large enough for its "af".  "bits" is set to the
 *	/CIDR prefix length, which can have defaults (like /32 for IPv4).
 * return:
 *	-1 if an error occurred (inspect errno; ENOENT means bad format).
 *	0 if successful conversion occurred.
 * note:
 *	192.5.5.1/28 has a nonzero host part, which means it isn't a network
 *	as called for by inet_cidr_pton() but it can be a host address with
 *	an included netmask.
 * author:
 *	Paul Vixie (ISC), October 1998
 */
static int
inet_net_pton_ipv4(const char *src, u_char *dst)
{
	static const char digits[] = "0123456789";
	const u_char *odst = dst;
	int			n,
				ch,
				tmp,
				bits;
	size_t		size = 4;

	/* Get the mantissa. */
	while (ch = *src++, isdigit((unsigned char) ch))
	{
		tmp = 0;
		do
		{
			n = strchr(digits, ch) - digits;
			assert(n >= 0 && n <= 9);
			tmp *= 10;
			tmp += n;
			if (tmp > 255)
				goto enoent;
		} while ((ch = *src++) != '\0' && isdigit((unsigned char) ch));
		if (size-- == 0)
			goto emsgsize;
		*dst++ = (u_char) tmp;
		if (ch == '\0' || ch == '/')
			break;
		if (ch != '.')
			goto enoent;
	}

	/* Get the prefix length if any. */
	bits = -1;
	if (ch == '/' && isdigit((unsigned char) src[0]) && dst > odst)
	{
		/* CIDR width specifier.  Nothing can follow it. */
		ch = *src++;			/* Skip over the /. */
		bits = 0;
		do
		{
			n = strchr(digits, ch) - digits;
			assert(n >= 0 && n <= 9);
			bits *= 10;
			bits += n;
		} while ((ch = *src++) != '\0' && isdigit((unsigned char) ch));
		if (ch != '\0')
			goto enoent;
		if (bits > 32)
			goto emsgsize;
	}

	/* Firey death and destruction unless we prefetched EOS. */
	if (ch != '\0')
		goto enoent;

	/* Prefix length can default to /32 only if all four octets spec'd. */
	if (bits == -1)
	{
		if (dst - odst == 4)
			bits = 32;
		else
			goto enoent;
	}

	/* If nothing was written to the destination, we found no address. */
	if (dst == odst)
		goto enoent;

	/* If prefix length overspecifies mantissa, life is bad. */
	if ((bits / 8) > (dst - odst))
		goto enoent;

	/* Extend address to four octets. */
	while (size-- > 0)
		*dst++ = 0;

	return bits;

enoent:
	errno = ENOENT;
	return (-1);

emsgsize:
	errno = EMSGSIZE;
	return (-1);
}
