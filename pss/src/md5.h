#ifndef _MD5_H
#define _MD5_H


#define S11 7

#define S12 12

#define S13 17

#define S14 22

#define S21 5

#define S22 9

#define S23 14

#define S24 20

#define S31 4

#define S32 11

#define S33 16

#define S34 23

#define S41 6

#define S42 10

#define S43 15

#define S44 21


void MD5Transform PROTO_LIST ((UINT4 [4], unsigned char [64]));

void Encode PROTO_LIST ((unsigned char *, UINT4 *, unsigned int)); 

void Decode PROTO_LIST ((UINT4 *, unsigned char *, unsigned int)); 

void MD5_memcpy PROTO_LIST ((POINTER, POINTER, unsigned int));
void MD5_memset PROTO_LIST ((POINTER, int, unsigned int));

extern unsigned char PADDING[64];

/* F, G, H and I are basic MD5 functions.
*/

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))

#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))

#define H(x, y, z) ((x) ^ (y) ^ (z))

#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
*/

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.

  Rotation is separate from addition to prevent recomputation.
*/

#define FF(a, b, c, d, x, s, ac) { (a) += F((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT((a), (s)); (a) += (b); } 
	
#define GG(a, b, c, d, x, s, ac) { (a) += G((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
	
#define HH(a, b, c, d, x, s, ac) { (a) += H((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
	
#define II(a, b, c, d, x, s, ac) { (a) += I((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }
	

/* MD5 context. */

typedef struct {
	UINT4 state[4];                                   /* state (ABCD) */
	UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];                         /* input buffer */
	
} MD5_CTX; 

void MD5Init PROTO_LIST ((MD5_CTX *));
void MD5Update PROTO_LIST ((MD5_CTX *, unsigned char *, unsigned int)); 

void MD5Final PROTO_LIST ((unsigned char [16], MD5_CTX *));



#endif	/* _MD5_H */

