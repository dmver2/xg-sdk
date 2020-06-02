/*	$Id: Crypto.cpp,v 1.6 2006/01/18 17:52:56 dmitry.vereshchagin Exp $
** Crypto implementation
** 
*/

#include <Crypto.h>
#include "cast128.h"
#include "cast256.h"

namespace GSDK
{

Crypto::~Crypto(void)
{
}

Crypto* Crypto::new_instance(int nAlgId)
{
    if (nAlgId == ALG_CAST_128)
        return new cast128();
    else
        return new cast256();
}

void Crypto::delete_instance(Crypto* p_crypto)
{
	delete p_crypto;
}

} // namespace GSDK
