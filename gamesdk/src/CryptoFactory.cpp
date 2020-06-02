#include ".\cryptofactory.h"
#include <log.h>

using namespace std;

namespace GSDK
{
HCRYPTPROV CryptoFactory::m_hProv = (HCRYPTPROV)0;
volatile int CryptoFactory::sm_ref = 0;

CryptoFactory::CryptoFactory(void) 
    : m_hExchangeKey(0)
    , m_hPublicKey(0)
    , m_hSessionKey(0)
    , m_sz_name()
{
    // 
	if(sm_ref == 0) {
		_stprintf_s(m_sz_name, sizeof(m_sz_name), _T("%08X"), ::GetCurrentProcessId());
		if (!CryptAcquireContext(&m_hProv, m_sz_name, MS_STRONG_PROV, 
			PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
            TCHAR msg[0x80] = {0};
            _stprintf_s(msg, ARRAY_LENGTH(msg), _T("A cryptographic service handle could not be acquired. (0x%08X)"), ::GetLastError());
            Log::log("SYSTEM", Log::error, msg);
			throw Exception(msg);
		}
	}
	++sm_ref;
}

CryptoFactory::~CryptoFactory(void) {
    // 
    if(m_hSessionKey)
        ::CryptDestroyKey(m_hSessionKey);
	if(--sm_ref == 0) {
        // 
        //CryptAcquireContext(&m_hProv, m_sz_name, MS_STRONG_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
        // 
        ::CryptReleaseContext(m_hProv, 0);
	}
}

/// 
/// 
/// @param os: 
/// @return	true 
bool CryptoFactory::createPublicKey(OutputStream &os) {
    // server side
    DWORD nCount = 0;

    // 
    if (!CryptGenKey(m_hProv, CALG_RSA_KEYX, 0, &m_hExchangeKey))
        return false;
    // 
    if (!CryptExportKey(m_hExchangeKey, NULL, PUBLICKEYBLOB, 0, NULL, &nCount)) {
        // Getting BLOB size error
        return false;
    }
    // 
    BYTE* pPubKeyData = static_cast<BYTE*>(malloc(nCount));
    if (!pPubKeyData) {
        // Out of memory!
        return false;
    }
    ZeroMemory(pPubKeyData, nCount);
    // 
    if (!CryptExportKey(m_hExchangeKey, NULL, PUBLICKEYBLOB, 0, pPubKeyData, &nCount)) {
        // Public key export error
        return false;
    } 
    // 
    os.write_int16(nCount);
    os.write(pPubKeyData, nCount);
    // 
    free(pPubKeyData);

    return true;
}

/// 
/// @param inp: 
/// @return	true 
bool CryptoFactory::readPublicKey(InputStream & inp) {
    // client side
    DWORD nCount = 0;

    // 
    nCount = inp.read_int16();
    // 
    BYTE* pPubKeyData = static_cast<BYTE*>(malloc(nCount));
    if (!pPubKeyData) {
        // Out of memory!
        return false;
    }
    // 
    inp.read(pPubKeyData, nCount);
    // 
    if(!CryptImportKey(m_hProv, pPubKeyData, nCount, NULL, 0, &m_hPublicKey)) {
        // Public key import error
        return false;
    }
    // 
    free(pPubKeyData);

    return true;
}

/// 
/// 
/// 
/// @param conn_msg: 
/// @return	true 
bool CryptoFactory::createSessionKey(OutputStream &conn_msg) {
    // client side
    DWORD nCount = 0;

    // 
    if (!CryptGenKey(m_hProv, CALG_RC2, CRYPT_EXPORTABLE, &m_hSessionKey)) {
        // Session key generation error
        return false;
    }
    // 
    if (!CryptExportKey(m_hSessionKey, m_hPublicKey, SIMPLEBLOB, 0, NULL, &nCount)) {
        // Getting BLOB size error
        return false;
    }
    // 
    BYTE* pSessionKeyData = static_cast<BYTE*>(malloc(nCount));
    if (!pSessionKeyData) {
        // Out of memory
        return false;
    }
    ZeroMemory(pSessionKeyData, nCount);
    // 
    if (!CryptExportKey(m_hSessionKey, m_hPublicKey, SIMPLEBLOB, 0, pSessionKeyData, &nCount)) {
        // Session key export error
        return false;
    }
    // 
    conn_msg.write_int16(nCount);
    conn_msg.write(pSessionKeyData, nCount);
    // 
    free(pSessionKeyData);
    // CryptDestroyKey(m_hPublicKey);

    return true;
}

/// 
/// 
/// @param inp: 
/// @return	true 
bool CryptoFactory::readSessionKey(InputStream & inp) {
    // server side
    DWORD nCount = 0;

    // 
    nCount = inp.read_int16();
    // 
    BYTE* pSessionKeyData = static_cast<BYTE*>(malloc(nCount));
    if (!pSessionKeyData) {
        // Out of memory!
        return false;
    }
    // 
    inp.read(pSessionKeyData, nCount);
    // 
    if(!CryptImportKey(m_hProv, pSessionKeyData, nCount, m_hExchangeKey, CRYPT_EXPORTABLE, &m_hSessionKey)) {
        // Session key import error
        return false;
    }
    // 
    free(pSessionKeyData);
    // CryptDestroyKey(m_hExchangeKey);

    return true;
}

/// 
/// @param pKey: 
/// @return	
int CryptoFactory::getSessionKey(u_char* pKey) {
    DWORD i, nCount = 0;

    // 
    if (!CryptExportKey(m_hSessionKey, NULL, OPAQUEKEYBLOB, 0, NULL, &nCount)) {
        // Getting BLOB size error
        return 0;
    }
    // 
    u_char* pKeyData = static_cast<u_char*>(malloc(nCount));
    if (!pKeyData) {
        // Out of memory!
        return 0;
    }
    ZeroMemory(pKeyData, nCount);
    // 
    CryptExportKey(m_hSessionKey, NULL, OPAQUEKEYBLOB, 0, pKeyData, &nCount);
    // 
    for (i=0; i<16; i++)
        pKey[i] = pKeyData[nCount-16+i];
    // 
    free(pKeyData);

    return (int)i;
}

} // namespace