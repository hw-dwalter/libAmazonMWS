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



#ifndef AMAZONMWS_CLIENT_H
#define AMAZONMWS_CLIENT_H

#include "BasicClient.h"

namespace AmazonMWS {

template <typename Service>
class Client: private BasicClient
{

public:
    Client(const string& Endpoint, const string& AWSAccessKeyId, const string& AWSSecretKey, const string& Merchant);
    virtual ~Client();

    long call(const typename Service::Call call, ostream& response, long verbose = 0L );
    long call(const typename Service::Call call, paramMap_t& request, ostream& response, long verbose = 0L );
};

};

#include "Client.inl"

#endif // AMAZONMWS_CLIENT_H
