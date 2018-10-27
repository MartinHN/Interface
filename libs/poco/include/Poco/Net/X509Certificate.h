//
// X509Certificate.h
//
// $Id: //poco/1.4/NetSSL_OpenSSL/include/Poco/Net/X509Certificate.h#1 $
//
// Library: NetSSL_OpenSSL
// Package: SSLCore
// Module:  X509Certificate
//
// Definition of the X509Certificate class.
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef NetSSL_X509Certificate_INCLUDED
#define NetSSL_X509Certificate_INCLUDED


#include "Poco/Net/NetSSL.h"
#include "Poco/Net/SocketDefs.h"
#include "Poco/Crypto/X509Certificate.h"
#include "Poco/DateTime.h"
#include "Poco/SharedPtr.h"
#include <set>

#ifdef verify 
#undef verify 
#endif

namespace Poco {
namespace Net {


class HostEntry;


class NetSSL_API X509Certificate: public Poco::Crypto::X509Certificate
	/// This class extends Poco::Crypto::X509Certificate with the
	/// feature to validate a certificate.
{
public:
	explicit X509Certificate(std::istream& istr);
		/// Creates the X509Certificate object by reading
		/// a certificate in PEM format from a stream.

	explicit X509Certificate(const std::string& path);
		/// Creates the X509Certificate object by reading
		/// a certificate in PEM format from a file.

	explicit X509Certificate(X509* pCert);
		/// Creates the X509Certificate from an existing
		/// OpenSSL certificate. Ownership is taken of 
		/// the certificate.

	X509Certificate(X509* pCert, bool shared);
		/// Creates the X509Certificate from an existing
		/// OpenSSL certificate. Ownership is taken of 
		/// the certificate. If shared is true, the 
		/// certificate's reference count is incremented.

	X509Certificate(const Poco::Crypto::X509Certificate& cert);
		/// Creates the certificate by copying another one.

	X509Certificate& operator = (const Poco::Crypto::X509Certificate& cert);
		/// Assigns a certificate.

	~X509Certificate();
		/// Destroys the X509Certificate.

	bool verify(const std::string& hostName) const;
		/// Verifies the validity of the certificate against the host name.
		///
		/// For this check to be successful, the certificate must contain
		/// a domain name that matches the domain name
		/// of the host.
		/// 
		/// Returns true if verification succeeded, or false otherwise.
		
	static bool verify(const Poco::Crypto::X509Certificate& cert, const std::string& hostName);
		/// Verifies the validity of the certificate against the host name.
		///
		/// For this check to be successful, the certificate must contain
		/// a domain name that matches the domain name
		/// of the host.
		///
		/// Returns true if verification succeeded, or false otherwise.
		
protected:
	static bool containsWildcards(const std::string& commonName);
	static bool matchByAlias(const std::string& alias, const HostEntry& heData);
	
private:
	enum
	{
		NAME_BUFFER_SIZE = 256
	};
};


} } // namespace Poco::Net


#endif // NetSSL_X509Certificate_INCLUDED
