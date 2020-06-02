/*	$Id: cast128.cpp,v 1.4 2006/01/18 17:05:02  Exp $
** cast128 implementation
** 
*/

#include "cast128.h"

using namespace std;

namespace GSDK
{

cast128::cast128() {
	m_is_keyInitialized = false;
}

// internal method to initialize the enc/decryption key
void cast128::set_key(u_char* newKey, int keyLen) {
	u_char rowKey[256];
	int i, nKeyLen;
	nKeyLen = keyLen;

	if (nKeyLen > 255)
        nKeyLen = 256;
	for (i=0; i<nKeyLen; i++) 
        rowKey[i] = newKey[i];
	this->InitKey(&m_cKey, rowKey, nKeyLen);
	m_is_keyInitialized = true;
}

// cast-128 byte-stream encryption
int cast128::encrypt (u_char* inStream, u_char* outStream, int streamLen) {
    if(!m_is_keyInitialized)
		throw exception("Key hasn't been set\n");

    int i, nLen, nRemainder, nBlocks;
    u_char *ptrIn, *ptrOut;

    nLen = streamLen;
    nRemainder = nLen % 8;
    if (nRemainder > 0)
        nLen += 8 - nRemainder;
    nBlocks = nLen / 8;

    ptrIn = new u_char[nLen];
    if (!ptrIn)
        throw exception("Insufficient memory available\n");

    // Input bits array initializing
    for (i=0; i<nLen; i++)
        ptrIn[i] = 0;
    // Copying input data
    for (i=0; i<streamLen; i++)
        ptrIn[i] = inStream[i];

    ptrOut = ptrIn;
    for (i=0; i < nBlocks; i++)
        encryptBlock(ptrIn+(i*8), ptrOut+(i*8));
    // Copying output data
    for (i=0; i<nLen; i++)
        outStream[i] = ptrOut[i];
    free(ptrIn);

    return nLen;
}

// cast-128 byte-stream decryption
int cast128::decrypt (u_char* inStream, u_char* outStream, int streamLen) {
    if(!m_is_keyInitialized)
		throw exception("Key hasn't been set\n");

    int i, nLen, nBlocks;
    u_char *ptrIn, *ptrOut;

    nLen = streamLen;
    nBlocks = nLen / 8;

    ptrIn = new u_char[nLen];
    if (!ptrIn)
        throw exception("Insufficient memory available\n");
    // Input bits array initializing
    for (i=0; i<nLen; i++)
        ptrIn[i] = 0;
    // Copying input data
    for (i=0; i<nLen; i++)
        ptrIn[i] = inStream[i];

    ptrOut = ptrIn;
    for (i=0; i < nBlocks; i++)
        decryptBlock(ptrIn+(i*8), ptrOut+(i*8));
    // Copying output data
    for (i=0; i<nLen; i++)
        outStream[i] = ptrOut[i];
    free(ptrIn);
    
    return nLen;
}

// cast-128 encrypt 8 bytes using key
void cast128::encryptBlock(u_char* inblock, u_char* outblock) {
    /* Variable declaration */
    u_long t, l, r;
    cast128_key *key = &m_cKey;

	/* Get inblock into l,r */
	l = ((u_long)inblock[0] << 24) | ((u_long)inblock[1] << 16) |
	    ((u_long)inblock[2] << 8) | (u_long)inblock[3];
	r = ((u_long)inblock[4] << 24) | ((u_long)inblock[5] << 16) |
	    ((u_long)inblock[6] << 8) | (u_long)inblock[7];
	/* Do the work */
	F1(l, r,  0); // round 01
	F2(r, l,  1); // round 02
	F3(l, r,  2); // round 03
	F1(r, l,  3); // round 04
	F2(l, r,  4); // round 05
	F3(r, l,  5); // round 06
	F1(l, r,  6); // round 07
	F2(r, l,  7); // round 08
	F3(l, r,  8); // round 09
	F1(r, l,  9); // round 10
	F2(l, r, 10); // round 11
	F3(r, l, 11); // round 12
	/* Only do full 16 rounds if key length > 80 bits */
	if (key->rounds > 12) {
		F1(l, r, 12); // round 13
		F2(r, l, 13); // round 14
		F3(l, r, 14); // round 15
		F1(r, l, 15); // round 16
	}
	/* Put l,r into outblock */
	outblock[0] = U8a(r);
	outblock[1] = U8b(r);
	outblock[2] = U8c(r);
	outblock[3] = U8d(r);
	outblock[4] = U8a(l);
	outblock[5] = U8b(l);
	outblock[6] = U8c(l);
	outblock[7] = U8d(l);
	/* Wipe clean */
	t = l = r = 0;
}

// cast-128 decrypt 8 bytes using key
void cast128::decryptBlock(u_char* inblock, u_char* outblock) {
    /* Variable declaration */
    u_long t, l, r;
    cast128_key *key = &m_cKey;

	/* Get inblock into l,r */
	r = ((u_long)inblock[0] << 24) | ((u_long)inblock[1] << 16) |
	    ((u_long)inblock[2] << 8) | (u_long)inblock[3];
	l = ((u_long)inblock[4] << 24) | ((u_long)inblock[5] << 16) |
	    ((u_long)inblock[6] << 8) | (u_long)inblock[7];
	/* Do the work */
	/* Only do full 16 rounds if key length > 80 bits */
	if (key->rounds > 12) {
		F1(r, l, 15); // round 01
		F3(l, r, 14); // round 02
		F2(r, l, 13); // round 03
		F1(l, r, 12); // round 04
	}
	F3(r, l, 11); // round 05
	F2(l, r, 10); // round 06
	F1(r, l,  9); // round 07
	F3(l, r,  8); // round 08
	F2(r, l,  7); // round 09
	F1(l, r,  6); // round 10
	F3(r, l,  5); // round 11
	F2(l, r,  4); // round 12
	F1(r, l,  3); // round 13
	F3(l, r,  2); // round 14
	F2(r, l,  1); // round 15
	F1(l, r,  0); // round 16
	/* Put l,r into outblock */
	outblock[0] = U8a(l);
	outblock[1] = U8b(l);
	outblock[2] = U8c(l);
	outblock[3] = U8d(l);
	outblock[4] = U8a(r);
	outblock[5] = U8b(r);
	outblock[6] = U8c(r);
	outblock[7] = U8d(r);
	/* Wipe clean */
	t = l = r = 0;
}

// initialize the key using the S-Boxes
void cast128::InitKey(cast128_key* key, u_char* rawkey, int keybytes) {
    /* Variable declaration */
    u_long t[4], z[4], x[4];
    int i;

	/* Set number of rounds to 12 or 16, depending on key length */
	key->rounds = (keybytes <= 10 ? 12 : 16);

	/* Copy key to workspace x */
	for (i = 0; i < 4; i++) {
		x[i] = 0;
		if ((i*4+0) < keybytes) x[i]  = (u_long)rawkey[i*4+0] << 24;
		if ((i*4+1) < keybytes) x[i] |= (u_long)rawkey[i*4+1] << 16;
		if ((i*4+2) < keybytes) x[i] |= (u_long)rawkey[i*4+2] << 8;
		if ((i*4+3) < keybytes) x[i] |= (u_long)rawkey[i*4+3];
	}
	/* Generate 32 subkeys, four at a time */
	for (i = 0; i < 32; i+=4) {
		switch (i & 4) {
		 case 0:
			t[0] = z[0] = x[0] ^ s_box5[U8b(x[3])] ^
			 s_box6[U8d(x[3])] ^ s_box7[U8a(x[3])] ^
			 s_box8[U8c(x[3])] ^ s_box7[U8a(x[2])];
			t[1] = z[1] = x[2] ^ s_box5[U8a(z[0])] ^
			 s_box6[U8c(z[0])] ^ s_box7[U8b(z[0])] ^
			 s_box8[U8d(z[0])] ^ s_box8[U8c(x[2])];
			t[2] = z[2] = x[3] ^ s_box5[U8d(z[1])] ^
			 s_box6[U8c(z[1])] ^ s_box7[U8b(z[1])] ^
			 s_box8[U8a(z[1])] ^ s_box5[U8b(x[2])];
			t[3] = z[3] = x[1] ^ s_box5[U8c(z[2])] ^
			 s_box6[U8b(z[2])] ^ s_box7[U8d(z[2])] ^
			 s_box8[U8a(z[2])] ^ s_box6[U8d(x[2])];
			break;
		 case 4:
			t[0] = x[0] = z[2] ^ s_box5[U8b(z[1])] ^
			 s_box6[U8d(z[1])] ^ s_box7[U8a(z[1])] ^
			 s_box8[U8c(z[1])] ^ s_box7[U8a(z[0])];
			t[1] = x[1] = z[0] ^ s_box5[U8a(x[0])] ^
			 s_box6[U8c(x[0])] ^ s_box7[U8b(x[0])] ^
			 s_box8[U8d(x[0])] ^ s_box8[U8c(z[0])];
			t[2] = x[2] = z[1] ^ s_box5[U8d(x[1])] ^
			 s_box6[U8c(x[1])] ^ s_box7[U8b(x[1])] ^
			 s_box8[U8a(x[1])] ^ s_box5[U8b(z[0])];
			t[3] = x[3] = z[3] ^ s_box5[U8c(x[2])] ^
			 s_box6[U8b(x[2])] ^ s_box7[U8d(x[2])] ^
			 s_box8[U8a(x[2])] ^ s_box6[U8d(z[0])];
			break;
		}
		switch (i & 12) {
		 case 0:
		 case 12:
			key->xkey[i+0] = s_box5[U8a(t[2])] ^ s_box6[U8b(t[2])] ^
			 s_box7[U8d(t[1])] ^ s_box8[U8c(t[1])];
			key->xkey[i+1] = s_box5[U8c(t[2])] ^ s_box6[U8d(t[2])] ^
			 s_box7[U8b(t[1])] ^ s_box8[U8a(t[1])];
			key->xkey[i+2] = s_box5[U8a(t[3])] ^ s_box6[U8b(t[3])] ^
			 s_box7[U8d(t[0])] ^ s_box8[U8c(t[0])];
			key->xkey[i+3] = s_box5[U8c(t[3])] ^ s_box6[U8d(t[3])] ^
			 s_box7[U8b(t[0])] ^ s_box8[U8a(t[0])];
			break;
		 case 4:
		 case 8:
			key->xkey[i+0] = s_box5[U8d(t[0])] ^ s_box6[U8c(t[0])] ^
			 s_box7[U8a(t[3])] ^ s_box8[U8b(t[3])];
			key->xkey[i+1] = s_box5[U8b(t[0])] ^ s_box6[U8a(t[0])] ^
			 s_box7[U8c(t[3])] ^ s_box8[U8d(t[3])];
			key->xkey[i+2] = s_box5[U8d(t[1])] ^ s_box6[U8c(t[1])] ^
			 s_box7[U8a(t[2])] ^ s_box8[U8b(t[2])];
			key->xkey[i+3] = s_box5[U8b(t[1])] ^ s_box6[U8a(t[1])] ^
			 s_box7[U8c(t[2])] ^ s_box8[U8d(t[2])];
			break;
		}
		switch (i & 12) {
		 case 0:
			key->xkey[i+0] ^= s_box5[U8c(z[0])];
			key->xkey[i+1] ^= s_box6[U8c(z[1])];
			key->xkey[i+2] ^= s_box7[U8b(z[2])];
			key->xkey[i+3] ^= s_box8[U8a(z[3])];
			break;
		 case 4:
			key->xkey[i+0] ^= s_box5[U8a(x[2])];
			key->xkey[i+1] ^= s_box6[U8b(x[3])];
			key->xkey[i+2] ^= s_box7[U8d(x[0])];
			key->xkey[i+3] ^= s_box8[U8d(x[1])];
			break;
		 case 8:
			key->xkey[i+0] ^= s_box5[U8b(z[2])];
			key->xkey[i+1] ^= s_box6[U8a(z[3])];
			key->xkey[i+2] ^= s_box7[U8c(z[0])];
			key->xkey[i+3] ^= s_box8[U8c(z[1])];
			break;
		 case 12:
			key->xkey[i+0] ^= s_box5[U8d(x[0])];
			key->xkey[i+1] ^= s_box6[U8d(x[1])];
			key->xkey[i+2] ^= s_box7[U8a(x[2])];
			key->xkey[i+3] ^= s_box8[U8b(x[3])];
			break;
		}
		if (i >= 16) {
			key->xkey[i+0] &= 31;
			key->xkey[i+1] &= 31;
			key->xkey[i+2] &= 31;
			key->xkey[i+3] &= 31;
		}
	}
	/* Wipe clean */
	for (i = 0; i < 4; i++) {
		t[i] = x[i] = z[i] = 0;
	}
}

} // namespace GSDK