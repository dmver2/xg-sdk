/*
 * This file was auto-generated by the Axis C++ Web Service Generator (WSDL2Ws)
 * This file contains functions to manipulate complex type serverAddress
 */

#if !defined(__SERVERADDRESS_PARAM_H__INCLUDED_)
#define __SERVERADDRESS_PARAM_H__INCLUDED_

#include <axis/AxisUserAPI.hpp>
AXIS_CPP_NAMESPACE_USE 

/*Local name and the URI for the type*/
static const char* Axis_URI_serverAddress = "urn:BeanService";
static const char* Axis_TypeName_serverAddress = "serverAddress";

class serverAddress
{
public:
	xsd__string host;
	xsd__int port;
	serverAddress();
	virtual ~serverAddress();
};

#endif /* !defined(__SERVERADDRESS_PARAM_H__INCLUDED_)*/
