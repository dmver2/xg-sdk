/*	$Id: cast256.cpp,v 1.4 2006/01/18 17:05:02  Exp $
** cast256 implementation
** 
*/

#include "cast256.h"

using namespace std;

namespace GSDK
{

cast256::cast256() : m_is_keyInitialized(false) {
	// The constructor is empty
}

// internal method to initialize the enc/decryption key
void cast256::set_key(u_char* newKey, int keyLen) {
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

// cast-256 byte-stream encryption
int cast256::encrypt (u_char* inStream, u_char* outStream, int streamLen) {
    if(!m_is_keyInitialized)
		throw exception("Key hasn't been set\n");

    int i, nLen, nRemainder, nBlocks;
    u_char *ptrIn, *ptrOut;

    nLen = streamLen;
    nRemainder = nLen % 16;
    if (nRemainder > 0)
        nLen += 16 - nRemainder;
    nBlocks = nLen / 16;

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
        encryptBlock(ptrIn+(i*16), ptrOut+(i*16));
    // Copying output data
    for (i=0; i<nLen; i++)
        outStream[i] = ptrOut[i];
    free(ptrIn);

    return nLen;
}

// cast-256 byte-stream decryption
int cast256::decrypt (u_char* inStream, u_char* outStream, int streamLen) {
    if(!m_is_keyInitialized)
		throw exception("Key hasn't been set\n");

    int i, nLen, nBlocks;
    u_char *ptrIn, *ptrOut;

    nLen = streamLen;
    nBlocks = nLen / 16;

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
        decryptBlock(ptrIn+(i*16), ptrOut+(i*16));
    // Copying output data
    for (i=0; i<nLen; i++)
        outStream[i] = ptrOut[i];
    free(ptrIn);
    
    return nLen;
}

// cast-256 encrypt 16 bytes using key
void cast256::encryptBlock(u_char* inblock, u_char* outblock) {
    /* Variable declaration */
    u_long t, A, B, C, D;
    cast256_key* key = &m_cKey;

    /* Get inblock into A, B, C, D */
    A = ((u_long)inblock[0] << 24) | ((u_long)inblock[1] << 16) |
	    ((u_long)inblock[2] << 8) | (u_long)inblock[3];
    B = ((u_long)inblock[4] << 24) | ((u_long)inblock[5] << 16) |
	    ((u_long)inblock[6] << 8) | (u_long)inblock[7];
    C = ((u_long)inblock[8] << 24) | ((u_long)inblock[9] << 16) |
	    ((u_long)inblock[10] << 8) | (u_long)inblock[11];
    D = ((u_long)inblock[12] << 24) | ((u_long)inblock[13] << 16) |
	    ((u_long)inblock[14] << 8) | (u_long)inblock[15];

    // Perform 6 forward quad-rounds
	Q(0);
	Q(1);
	Q(2);
	Q(3);
	Q(4);
	Q(5);

	// Perform 6 reverse quad-rounds
	QBar(6);
	QBar(7);
	QBar(8);
	QBar(9);
	QBar(10);
	QBar(11);

    /* Put A, B, C, D into outblock */
	outblock[ 0] = U8a(A);
	outblock[ 1] = U8b(A);
	outblock[ 2] = U8c(A);
	outblock[ 3] = U8d(A);
	outblock[ 4] = U8a(B);
	outblock[ 5] = U8b(B);
	outblock[ 6] = U8c(B);
	outblock[ 7] = U8d(B);
    outblock[ 8] = U8a(C);
	outblock[ 9] = U8b(C);
	outblock[10] = U8c(C);
	outblock[11] = U8d(C);
	outblock[12] = U8a(D);
	outblock[13] = U8b(D);
	outblock[14] = U8c(D);
	outblock[15] = U8d(D);
	/* Wipe clean */
	t = A = B = C = D = 0;
}

// cast-256 encrypt 16 bytes using key
void cast256::decryptBlock(u_char* inblock, u_char* outblock) {
    /* Variable declaration */
    u_long t, A, B, C, D;
    cast256_key* key = &m_cKey;

    /* Get inblock into A, B, C, D */
    A = ((u_long)inblock[0] << 24) | ((u_long)inblock[1] << 16) |
	    ((u_long)inblock[2] << 8) | (u_long)inblock[3];
    B = ((u_long)inblock[4] << 24) | ((u_long)inblock[5] << 16) |
	    ((u_long)inblock[6] << 8) | (u_long)inblock[7];
    C = ((u_long)inblock[8] << 24) | ((u_long)inblock[9] << 16) |
	    ((u_long)inblock[10] << 8) | (u_long)inblock[11];
    D = ((u_long)inblock[12] << 24) | ((u_long)inblock[13] << 16) |
	    ((u_long)inblock[14] << 8) | (u_long)inblock[15];

    // Perform 6 forward quad-rounds
	Q(11);
	Q(10);
	Q(9);
	Q(8);
	Q(7);
	Q(6);

	// Perform 6 reverse quad-rounds
	QBar(5);
	QBar(4);
	QBar(3);
	QBar(2);
	QBar(1);
	QBar(0);

    /* Put A, B, C, D into outblock */
	outblock[ 0] = U8a(A);
	outblock[ 1] = U8b(A);
	outblock[ 2] = U8c(A);
	outblock[ 3] = U8d(A);
	outblock[ 4] = U8a(B);
	outblock[ 5] = U8b(B);
	outblock[ 6] = U8c(B);
	outblock[ 7] = U8d(B);
    outblock[ 8] = U8a(C);
	outblock[ 9] = U8b(C);
	outblock[10] = U8c(C);
	outblock[11] = U8d(C);
	outblock[12] = U8a(D);
	outblock[13] = U8b(D);
	outblock[14] = U8c(D);
	outblock[15] = U8d(D);
	/* Wipe clean */
	t = A = B = C = D = 0;
}

// The key initialization procedure
void cast256::InitKey(cast256_key* key, u_char* rawkey, int keybytes) {
    /* Variable declaration */
    u_long t, u, Cm, Cr, Lk[8], Tm[8], Tr[8];
    int i, j;
    
    /* Copy key to bit-array lk[i] */
    for(i=0; i<8; i++) {
        Lk[i] = 0;
		if ((i*4+0) < keybytes) Lk[i]  = (u_long)rawkey[i*4+0] << 24;
		if ((i*4+1) < keybytes) Lk[i] |= (u_long)rawkey[i*4+1] << 16;
		if ((i*4+2) < keybytes) Lk[i] |= (u_long)rawkey[i*4+2] << 8;
		if ((i*4+3) < keybytes) Lk[i] |= (u_long)rawkey[i*4+3];
    }

    // Initialization:
    Cm = 0x5a827999; Cr = 19;

    for(i=0; i<96; i+=8) {
        for(j=0; j<8; j++) {
            Tm[j] = Cm; Cm += 0x6ed9eba1;
            Tr[j] = Cr; Cr += 17;
        }
        k_rnd(Lk, Tr, Tm);
        
        for(j=0; j<8; j++) {
            Tm[j] = Cm; Cm += 0x6ed9eba1;
            Tr[j] = Cr; Cr += 17;
        }
        k_rnd(Lk, Tr, Tm);

        key->xkey[i + 0] = Lk[0]; key->xkey[i + 1] = Lk[2];
        key->xkey[i + 2] = Lk[4]; key->xkey[i + 3] = Lk[6];
        key->xkey[i + 4] = Lk[7]; key->xkey[i + 5] = Lk[5];
        key->xkey[i + 6] = Lk[3]; key->xkey[i + 7] = Lk[1];
    }
	/* Wipe clean */
    t = u = Cm = Cr = 0;
	for (i = 0; i < 8; i++) {
		Lk[i] = Tm[i] = Tr[i] = 0;
	}
}

} // namespace GSDK