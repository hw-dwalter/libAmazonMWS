/*
    Copyright (C) 2011  Daniel Walter daniel.walter@helmundwalter.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef AMAZONMWS_BASICCLIENT_H
#define AMAZONMWS_BASICCLIENT_H

#include <iostream>

#include <curl/curl.h>
#include <curl/easy.h>

#include <boost/date_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/hmac.h>

using namespace boost::posix_time;
using namespace boost::local_time;
using namespace std;

namespace AmazonMWS {

//Functors
struct csLessBoost: binary_function<string, string, bool> {
	bool operator()(const string & s1, const string & s2) const;
};


//Typedefs

/**
 * @brief Maptype for parameters sorted in natural byte order.
 **/

typedef map< string, string, csLessBoost> paramMap_t;


//Functions
string timestamp(const ptime& time, const string& format = "%Y-%m-%dT%H:%M:%SZ");


//Classes
class BasicClient {
protected:
	BasicClient(const string& Endpoint, const string& AWSAccessKeyId, const string& AWSSecretKey, const string& Merchant);
	virtual ~BasicClient();

	long call(const char* service, const char* call, const char* version,
		paramMap_t& request, ostream& response, long verbose = 0L);
private:
	struct impl;
	auto_ptr<impl> m_pimpl;
};

}
#endif /* AMAZONMWS_BASICCLIENT_H */
