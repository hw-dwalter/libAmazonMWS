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


#include <iostream>

#include "CURLGlobal.h"
#include "AmazonMWS.h"

using namespace std;
using namespace AmazonMWS;

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Missing parameter! Use it like:" << endl;
	cerr << argv[0] << " Endpoint AccessKey SecretAccessKey MerchantId" << endl;
	return 1;
    }

    Client<Service::Order> mws(
        argv[1],	/* Endpoint */
        argv[2],	/* AccessKey */
        argv[3],	/* SecretAccessKey */
        argv[4]		/* MerchantId */
    );

    double httpCode = mws.call(Service::Order::GetServiceStatus, cout);
    return httpCode;
}
