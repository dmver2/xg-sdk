/*
 * This file was auto-generated by the Axis C++ Web Service Generator (WSDL2Ws)
 * This file contains an Exception class of the web service.
 */

#if !defined(__AXISCLIENTEXCEPTION_EXCEPTION_H__INCLUDED_)
#define __AXISCLIENTEXCEPTION_EXCEPTION_H__INCLUDED_

#include <string>
#include <exception>
#include <axis/AxisException.hpp>
#include <axis/ISoapFault.hpp>
using namespace std;
AXIS_CPP_NAMESPACE_USE 

class GameService3Service_AxisClientException: public AxisException
{
public:
	STORAGE_CLASS_INFO GameService3Service_AxisClientException();
	STORAGE_CLASS_INFO GameService3Service_AxisClientException(ISoapFault* pFault);
	STORAGE_CLASS_INFO GameService3Service_AxisClientException(int iExceptionCode);
	STORAGE_CLASS_INFO GameService3Service_AxisClientException(exception* e);
	STORAGE_CLASS_INFO GameService3Service_AxisClientException(exception* e, int iExceptionCode);
	STORAGE_CLASS_INFO GameService3Service_AxisClientException(string sMessage);
	STORAGE_CLASS_INFO virtual ~GameService3Service_AxisClientException() throw();
	STORAGE_CLASS_INFO const char* what() throw();
	STORAGE_CLASS_INFO const int getExceptionCode();
	STORAGE_CLASS_INFO const string getMessage(exception* e);
	STORAGE_CLASS_INFO const string getMessage(int iExceptionCode);
	STORAGE_CLASS_INFO const ISoapFault* getFault();

private:
	 void processException(exception* e);
	 void processException(ISoapFault* pFault);
	 void processException(exception* e, int iExceptionCode);
	 void processException(int iExceptionCode);
	 string m_sMessage;
	 int m_iExceptionCode;
	 ISoapFault* m_pISoapFault;

};

#endif /* !defined(__AXISCLIENTEXCEPTION_EXCEPTION_H__INCLUDED_)*/
