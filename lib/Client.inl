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


#include "Client.h"

namespace AmazonMWS {

template <typename Service> Client<Service>::Client(const string& Endpoint, const string& AWSAccessKeyId, const string& AWSSecretKey, const string& Merchant)
:BasicClient(Endpoint, AWSAccessKeyId, AWSSecretKey, Merchant) {
}

template <typename Service> Client<Service>::~Client()
{

}

template <typename Service> long Client<Service>::call(const typename Service::Call call, ostream& response, long verbose) {
	paramMap_t request;
	return BasicClient::call(Service::name, Service::calls[call], Service::version, request, response, verbose);
}

template <typename Service>  long Client<Service>::call(const typename Service::Call call, paramMap_t& request, ostream& response, long verbose) {
	return BasicClient::call(Service::name, Service::calls[call], Service::version, request, response, verbose);
}

};