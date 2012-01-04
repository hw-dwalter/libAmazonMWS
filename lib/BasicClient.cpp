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


#include "BasicClient.h"
#include <boost/algorithm/string.hpp>

namespace AmazonMWS {

struct BasicClient::impl {
	impl(const string& Endpoint, const string& AWSAccessKeyId, const string& AWSSecretKey, const string& Merchant);
	virtual ~impl();

	static size_t curl_write_callback(void *ptr, size_t size, size_t nmemb, ostream *data);

	void base64(const unsigned char *input, unsigned int input_len, unsigned char*& output, unsigned int& output_len) const;
	string signature(const string& data) const;
	long post(const string& endpoint, const string& postfields, ostream& response, unsigned int restoreTime = 10, long verbose = 0L);

	string m_Endpoint;
	string m_AWSAccessKeyId;
	string m_AWSSecretKey;
	string m_Merchant;

	CURL* m_curl;
	curl_slist* m_headers;
	
	const string m_signatureMethod;
	const string m_signatureVersion;
};

BasicClient::BasicClient(const string& Endpoint, const string& AWSAccessKeyId, const string& AWSSecretKey, const string& Merchant)
:m_pimpl(new impl( Endpoint, AWSAccessKeyId, AWSSecretKey, Merchant))
{ }

BasicClient::~BasicClient() { }

long BasicClient::call( const char* service, const char* call, const char* version,
		paramMap_t& request, ostream& response, long verbose) {
	string HTTPRequestURI = "/";
	HTTPRequestURI.append(service);
	if(HTTPRequestURI != "/") {
	  HTTPRequestURI.append("/").append(version);
	}

	/*
	 * Sort the UTF-8 query string components by parameter name with natural byte ordering. The parameters can
	 * come from the GET URI or from the POST body (when Content-Type is application/x-www-form-urlencoded).
	 */
	request["AWSAccessKeyId"]		= m_pimpl->m_AWSAccessKeyId;
	request["Action"]			= call;
	request["SellerId"]			= m_pimpl->m_Merchant;
	request["SignatureMethod"]		= m_pimpl->m_signatureMethod;
	request["SignatureVersion"]		= m_pimpl->m_signatureVersion;
	request["Timestamp"]			= timestamp(second_clock::local_time());
	request["Version"]			= version;

	//params
	string CanonicalizedQueryString;
	paramMap_t::const_iterator paramIt = request.begin();
	for(; paramIt != request.end(); ++paramIt) {
		/*
		 * Separate the name-value pairs with an ampersand ( & ) (ASCII code 38).
		 */
		if(!CanonicalizedQueryString.empty()) {
			CanonicalizedQueryString.append("&");
		}

		/*
		 * URL encode the parameter name and values according to the following rules:
		 * - Do not URL encode any of the unreserved characters that RFC 3986 defines. These unreserved characters
		 *   are A-Z, a-z, 0-9, hyphen ( - ), underscore ( _ ), period ( . ), and tilde ( ~ ).
		 * - Percent encode all other characters with %XY, where X and Y are hex characters 0-9 and uppercase A-F.
		 * - Percent encode extended UTF-8 characters in the form %XY%ZA....
		 * - Percent encode the space character as %20 (and not +, as common encoding schemes do).
		 */
		char* value = curl_easy_escape(m_pimpl->m_curl, paramIt->second.c_str() , paramIt->second.length());

		/*
		 * Separate the encoded parameter names from their encoded values with the equals sign ( = ) (ASCII character 61),
		 * even if the parameter value is empty.
		 */
		CanonicalizedQueryString.append(paramIt->first).append("=").append(value);
		curl_free(value);
	}

	/*
	 * Create the string to sign according to the following pseudo-grammar (the "\n" represents an ASCII newline).
	 * StringToSign = HTTPVerb + "\n" +
	 * 	ValueOfHostHeaderInLowercase + "\n" +
	 * 	HTTPRequestURI + "\n" +
	 * 	CanonicalizedQueryString
	 */
	string StringToSign = "POST\n" + m_pimpl->m_Endpoint + "\n" + HTTPRequestURI + "\n" + CanonicalizedQueryString;
	CanonicalizedQueryString.append("&Signature=").append(m_pimpl->signature(StringToSign));

	string endpoint = "https://" + m_pimpl->m_Endpoint + HTTPRequestURI;
	return m_pimpl->post(endpoint, CanonicalizedQueryString, response);
}

BasicClient::impl::impl(const string& Endpoint, const string& AWSAccessKeyId, const string& AWSSecretKey, const string& Merchant)
:m_Endpoint(Endpoint)
,m_AWSAccessKeyId(AWSAccessKeyId)
,m_AWSSecretKey(AWSSecretKey)
,m_Merchant(Merchant)
,m_curl(curl_easy_init())
,m_headers(NULL)
,m_signatureMethod("HmacSHA256")
,m_signatureVersion("2")
{
	assert(m_curl != NULL);
	/*
	 * To create a User-Agent header, begin with the name of your application, followed by a forward slash, followed
	 * by the version of the application, followed by a space, an opening parenthesis, the Language name value pair,
	 * and a closing parenthesis. The Language parameter is a required attribute, but you can add additional attributes
	 * separated by semicolons.
	 * Because the User-Agent header is transmitted in every request, it is a good practice to limit the size of the header.
	 * Amazon MWS will reject a User-Agent header if it is longer than 500 characters.
	 */
	m_headers = curl_slist_append(m_headers, "x-amazon-user-agent: H&W IT-Solutions/0.2 (Language=C++; Platform=Linux)");

	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, impl::curl_write_callback);
	curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_headers);
	//curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(m_curl, CURLOPT_POST, 1);
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
}

BasicClient::impl::~impl() {
	curl_slist_free_all(m_headers);
	curl_easy_cleanup(m_curl);
}

void BasicClient::impl::base64(const unsigned char *input, unsigned int input_len, unsigned char*& output, unsigned int& output_len) const {
	BIO *bmem, *b64;
	BUF_MEM *bptr;

	b64 = BIO_new(BIO_f_base64());
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, input_len);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	output = new unsigned char[bptr->length];
	output_len = bptr->length;

	memcpy(output, bptr->data, bptr->length - 1);
	output[bptr->length - 1] = 0;

	BIO_free_all(b64);
}

string BasicClient::impl::signature(const string& data) const {
	string signature;
	unsigned char *mac = new unsigned char[EVP_MAX_MD_SIZE], *mac64 = NULL;
	unsigned int mac_len = 0, mac64_len = 0;

	HMAC(EVP_sha256(), m_AWSSecretKey.c_str(), m_AWSSecretKey.length(), (const unsigned char*)data.c_str(), data.size(), mac, &mac_len);
	if(!mac || mac_len <= 0) {
		cerr << "Could not generate HMAC SHA256!" << endl;
		delete[] mac;
		return string();
	}

	impl::base64(mac, mac_len, mac64, mac64_len);
	if(!mac64 || mac64_len <= 0) {
		cerr << "Could not generate base64 of mac!" << endl;
		return string();
	}

	char* mac64Escape = curl_easy_escape(m_curl, (const char*)mac64 , mac64_len-1);
	if(!mac64Escape) {
		cerr << "Could not urlencode base64 mac!" << endl;
		return string();
	}

	signature = mac64Escape;

	delete[] mac;
	delete[] mac64;
	curl_free(mac64Escape);

	return signature;
}

long BasicClient::impl::post(const string& endpoint, const string& postfields, ostream& response, unsigned int restoreTime, long verbose) {
	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, verbose);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, postfields.c_str());
	curl_easy_setopt(m_curl, CURLOPT_URL, endpoint.c_str());
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, (void*)&response);

	CURLcode res = curl_easy_perform(m_curl);
	response.flush();
	
	long httpCode = -1;
	curl_easy_getinfo (m_curl, CURLINFO_RESPONSE_CODE, &httpCode);
	return httpCode;
}

size_t BasicClient::impl::curl_write_callback(void *ptr, size_t size, size_t nmemb, ostream *data) {
	size_t r;
	r = size * nmemb;
	data->write((const char*)ptr, r);
	return r;
}

bool csLessBoost::operator()(const string & s1, const string & s2) const {
	return lexicographical_compare(s1, s2, boost::algorithm::is_less());
}

string timestamp(const ptime& time, const string& format) {
	ostringstream msg;
	//'-'? yyyy '-' mm '-' dd 'T' hh ':' mm ':' ss ('.' s+)? (zzzzzz)?
	time_facet* f = new time_facet(format.c_str());
	msg.imbue(locale(msg.getloc(), f));
	msg << time;
	//delete f;
	return msg.str();
}

};
