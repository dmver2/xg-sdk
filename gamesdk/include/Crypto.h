/*	$Id: Crypto.h,v 1.7 2006/01/18 17:52:56 dmitry.vereshchagin Exp $
** Crypto header file
** 
*/ 
#pragma once

#include "types.h"
#include "decl6n.h"

namespace GSDK
{
/// 
/**	
 *	
 *	
 */
class CxxLibExport Crypto
{

public:
	enum {
			ALG_CAST_128  =  1, 	///< 
			ALG_CAST_256  =  2, 	///< 
			KEY_128_BIT   = 16  	///< 
	};

public:
    virtual ~Crypto(void);

// 
protected:

// 
public:
    /// 
	///	
    ///	@param newKey - 
    /// @param keyLen - 
    virtual void set_key (u_char* newKey, int keyLen) = 0;

    ///	
	///	
    /// @param inStream - 
    /// @param outStream - 
    /// @param streamLen - 
    ///	@return 
    virtual int encrypt (u_char* inStream, u_char* outStream, int streamLen) = 0;

    ///	
	///	
    /// @param inStream - 
    /// @param outStream - 
    /// @param streamLen - 
    ///	@return 
    virtual int decrypt (u_char* inStream, u_char* outStream, int streamLen) = 0;

    /// 
    /// @param nAlgId - 
    ///	@return 
    static Crypto* new_instance (int nAlgId);

	/// 
	static void delete_instance(Crypto* p_crypto);

};

} // namespace GSDK
