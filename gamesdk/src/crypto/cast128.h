/*	$Id: cast128.h,v 1.5 2006/01/18 17:05:02  Exp $
** cast128 header file
** 
*/ 

#ifndef _CAST128_H_
#define _CAST128_H_

#include "crypto.h"
#include "defs.h"

typedef struct {
	u_long xkey[32];	/* Key, after expansion */
	int rounds;		/* Number of rounds to use, 12 or 16 */
} cast128_key;

/* CAST-128 uses three different round functions */
#define F1(l, r, i) \
	t = ROL(key->xkey[i] + r, key->xkey[i+16]); \
	l ^= ((s_box1[U8a(t)] ^ s_box2[U8b(t)]) - \
	 s_box3[U8c(t)]) + s_box4[U8d(t)];
#define F2(l, r, i) \
	t = ROL(key->xkey[i] ^ r, key->xkey[i+16]); \
	l ^= ((s_box1[U8a(t)] - s_box2[U8b(t)]) + \
	 s_box3[U8c(t)]) ^ s_box4[U8d(t)];
#define F3(l, r, i) \
	t = ROL(key->xkey[i] - r, key->xkey[i+16]); \
	l ^= ((s_box1[U8a(t)] + s_box2[U8b(t)]) ^ \
	 s_box3[U8c(t)]) - s_box4[U8d(t)];

namespace GSDK
{

class cast128 : public Crypto {

// Attributes
private:
    cast128_key m_cKey;
	bool m_is_keyInitialized;

// Construction & destruction
public:
	cast128();
//   virtual ~cast128();

// Operations
protected:
	void InitKey (cast128_key* key, u_char* rawkey, int keybytes);
    void decryptBlock (u_char* inblock, u_char* outblock);
	void encryptBlock (u_char* inblock, u_char* outblock);
public:
    int decrypt (u_char* inStream, u_char* outStream, int streamLen);
    int encrypt (u_char* inStream, u_char* outStream, int streamLen);
	void set_key  (u_char* newKey, int keyLen);
};

} // namespace GSDK

#endif /* ifndef _CAST128_H_ */ 