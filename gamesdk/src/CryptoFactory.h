#pragma once
#include "outputstream.h"
#include "inputstream.h"
#include <types.h>
#include <windows.h>
#include <wincrypt.h>           // CryptoAPI definitions 

namespace GSDK
{

class CryptoFactory
{
private:
    static HCRYPTPROV m_hProv;      // 
    HCRYPTKEY m_hExchangeKey;       // 
    HCRYPTKEY m_hPublicKey;         // 
    HCRYPTKEY m_hSessionKey;        // 
    TCHAR m_sz_name[64];
    volatile static int sm_ref;

public:
	CryptoFactory(void);
	~CryptoFactory(void);

    /// 
    /// 
    /// @param os: 
    /// @return	true 
	bool createPublicKey(OutputStream &os);

    /// 
    /// @param inp: 
    /// @return	true 
	bool readPublicKey(InputStream & inp);

    /// 
    /// 
    /// 
    /// @param conn_msg: 
    /// @return	true 
	bool createSessionKey(OutputStream &conn_msg);

    /// 
    /// 
    /// @param inp: 
    /// @return	true 
	bool readSessionKey(InputStream & inp);

    /// 
    /// @param pKey: 
    /// @return	
    int getSessionKey(u_char* pKey);
};

} // namespace GSDK