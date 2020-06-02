/*	$Id: cast256.h,v 1.5 2006/01/18 17:05:02  Exp $
** cast256 header file
** 
*/ 

#ifndef _CAST256_H_
#define _CAST256_H_

#include "crypto.h"
#include "defs.h"

typedef struct {
    /* CAST-256 has a variable key size (128, 160, 192, 224, or 256 bits) */
    u_long xkey[96];	/* Key, after expansion */
} cast256_key;

/* CAST-256 uses three different round functions */
#define f1(l, r, i, j) \
	t = ROL(key->xkey[i] + r, key->xkey[i+j]); \
	l ^= ((s_box1[U8a(t)] ^ s_box2[U8b(t)]) - \
        s_box3[U8c(t)]) + s_box4[U8d(t)];
#define f2(l, r, i, j) \
	t = ROL(key->xkey[i] ^ r, key->xkey[i+j]); \
	l ^= ((s_box1[U8a(t)] - s_box2[U8b(t)]) + \
        s_box3[U8c(t)]) ^ s_box4[U8d(t)];
#define f3(l, r, i, j) \
	t = ROL(key->xkey[i] - r, key->xkey[i+j]); \
	l ^= ((s_box1[U8a(t)] + s_box2[U8b(t)]) ^ \
        s_box3[U8c(t)]) - s_box4[U8d(t)];

/* forward quad-round */
#define Q(i) \
	f1(C, D, 8*i+4, -4); \
	f2(B, C, 8*i+5, -4); \
	f3(A, B, 8*i+6, -4); \
	f1(D, A, 8*i+7, -4);

/* reverse quad-round */
#define QBar(i) \
	f1(D, A, 8*i+7, -4); \
	f3(A, B, 8*i+6, -4); \
	f2(B, C, 8*i+5, -4); \
	f1(C, D, 8*i+4, -4);

#define k1(y, x, kr, km)   \
    t  = ROL(km + x, kr);  \
    u  = s_box1[U8a(t)];   \
    u ^= s_box2[U8b(t)];   \
    u -= s_box3[U8c(t)];   \
    u += s_box4[U8d(t)];   \
    y ^= u
#define k2(y, x, kr, km)   \
    t  = ROL(km ^ x, kr);  \
    u  = s_box1[U8a(t)];   \
    u -= s_box2[U8b(t)];   \
    u += s_box3[U8c(t)];   \
    u ^= s_box4[U8d(t)];   \
    y ^= u
#define k3(y, x, kr, km)   \
    t  = ROL(km - x, kr);  \
    u  = s_box1[U8a(t)];   \
    u += s_box2[U8b(t)];   \
    u ^= s_box3[U8c(t)];   \
    u -= s_box4[U8d(t)];   \
    y ^= u

#define k_rnd(k, tr, tm)           \
    k1(k[6], k[7], tr[0], tm[0]);  \
    k2(k[5], k[6], tr[1], tm[1]);  \
    k3(k[4], k[5], tr[2], tm[2]);  \
    k1(k[3], k[4], tr[3], tm[3]);  \
    k2(k[2], k[3], tr[4], tm[4]);  \
    k3(k[1], k[2], tr[5], tm[5]);  \
    k1(k[0], k[1], tr[6], tm[6]);  \
    k2(k[7], k[0], tr[7], tm[7])

namespace GSDK
{

class cast256 : public Crypto {

// Attributes
private:
    cast256_key m_cKey;
    bool m_is_keyInitialized;

// Construction & destruction
public:
	cast256();
//    virtual ~cast256();

// Operations
protected:
	void InitKey (cast256_key* key, u_char* rawkey, int keybytes);
    void decryptBlock (u_char* inblock, u_char* outblock);
	void encryptBlock (u_char* inblock, u_char* outblock);
public:
	int decrypt (u_char* inStream, u_char* outStream, int streamLen);
    int encrypt (u_char* inStream, u_char* outStream, int streamLen);
	void set_key  (u_char* newKey, int keyLen);
};

} // namespace GSDK
#endif /* ifndef _CAST256_H_ */ 