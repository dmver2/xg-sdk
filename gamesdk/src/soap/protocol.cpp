/*
 * This file was auto-generated by the Axis C++ Web Service Generator (WSDL2Ws)
 * This file contains functions to manipulate complex type protocol
 */

#include "protocol.hpp"
#include <axis/AxisWrapperAPI.hpp>

/*
 * This static method serialize a protocol type of object
 */
int Axis_Serialize_protocol(protocol* param, IWrapperSoapSerializer* pSZ, bool bArray = false)
{
	if (bArray)
	{
		pSZ->serialize("<", Axis_TypeName_protocol, ">", NULL);
	}
	else
	{
		const AxisChar* sPrefix = pSZ->getNamespacePrefix(Axis_URI_protocol);
		pSZ->serialize("<", Axis_TypeName_protocol, " xsi:type=\"", sPrefix, ":",
			Axis_TypeName_protocol, "\" xmlns:", sPrefix, "=\"",
			Axis_URI_protocol, "\">", NULL);
	}

	pSZ->serializeAsElement("className", (void*)&(param->className), XSD_STRING);
	pSZ->serializeAsElement("id", (void*)&(param->id), XSD_LONG);
	pSZ->serializeAsElement("name", (void*)&(param->name), XSD_STRING);

	pSZ->serialize("</", Axis_TypeName_protocol, ">", NULL);
	return AXIS_SUCCESS;
}

/*
 * This static method deserialize a protocol type of object
 */
int Axis_DeSerialize_protocol(protocol* param, IWrapperSoapDeSerializer* pIWSDZ)
{
	param->className = pIWSDZ->getElementAsString("className",0);
	param->id = pIWSDZ->getElementAsLong("id",0);
	param->name = pIWSDZ->getElementAsString("name",0);
	return pIWSDZ->getStatus();
}
void* Axis_Create_protocol(protocol* pObj, bool bArray = false, int nSize=0)
{
	if (bArray && (nSize > 0))
	{
		if (pObj)
		{
			protocol* pNew = new protocol[nSize];
			memcpy(pNew, pObj, sizeof(protocol)*nSize/2);
			memset(pObj, 0, sizeof(protocol)*nSize/2);
			delete [] pObj;
			return pNew;
		}
		else
		{
			return new protocol[nSize];
		}
	}
	else
		return new protocol;
}

/*
 * This static method delete a protocol type of object
 */
void Axis_Delete_protocol(protocol* param, bool bArray = false, int nSize=0)
{
	if (bArray)
	{
		delete [] param;
	}
	else
	{
		delete param;
	}
}
/*
 * This static method gives the size of protocol type of object
 */
int Axis_GetSize_protocol()
{
	return sizeof(protocol);
}

protocol::protocol()
{
	/*do not allocate memory to any pointer members here
	 because deserializer will allocate memory anyway. */
	memset( &className, 0, sizeof( xsd__string));
	memset( &id, 0, sizeof( xsd__long));
	memset( &name, 0, sizeof( xsd__string));
}

protocol::~protocol()
{
	/*delete any pointer and array members here*/
}