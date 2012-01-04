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
#include <sstream>

#include "CURLGlobal.h"
#include "AmazonMWS.h"

#include <boost/date_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>

#include "../pugixml.hpp"

using namespace std;
using namespace AmazonMWS;
using namespace boost::posix_time;
using namespace boost::local_time;

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Missing parameter! Use it like:" << endl;
        cerr << argv[0] << " Endpoint AccessKey SecretAccessKey MerchantId" << endl;
        return 1;
    }

    Client<Service::Order> mws(
        argv[1],	// Endpoint
        argv[2],	// AccessKey
        argv[3],	// SecretAccessKey
        argv[4]		// MerchantId
    );

    stringstream response;
    paramMap_t request;
    request["LastUpdatedAfter"] = timestamp(second_clock::local_time() - hours(2));
    request["MaxResultsPerPage"] = "5";
    request["MarketplaceId.Id.1"] = "A1F83G8C2ARO7P"; //Amazon.co.uk
    request["MarketplaceId.Id.2"] = "A1PA6795UKMFR9"; //Amazon.de
    request["MarketplaceId.Id.3"] = "A13V1IB3VIYZZH"; //Amazon.fr
    request["MarketplaceId.Id.4"] = "APJ6JRA9NG5V4";  //Amazon.it

    double httpCode = mws.call(Service::Order::ListOrders, request, response);

    if (httpCode == 200) {
	pugi::xpath_node nexttoken;
	do {
	  pugi::xml_document doc;
	  pugi::xml_parse_result result = doc.load(response);
	  nexttoken = doc.select_single_node("//NextToken");
	  pugi::xpath_node_set orders = doc.select_nodes("//Orders/Order");
	  std::cout << "Got " << orders.size() << " orders:\n";

	  for (pugi::xpath_node_set::const_iterator it = orders.begin(); it != orders.end(); ++it) {
	      cout << it->node().child_value("AmazonOrderId") << " purchased at " << it->node().child_value("PurchaseDate");
	      cout << " from " << it->node().child_value("SalesChannel") << " is in state \"";
	      cout << it->node().child_value("OrderStatus") << "\"" << endl;
	  }
	  if(nexttoken) {
	      response.str();
	      paramMap_t request;
	      request["NextToken"] = nexttoken.node().child_value();
	      httpCode = mws.call(Service::Order::ListOrdersByNextToken, request, response);
	  }
	}while(httpCode == 200 && nexttoken);
    } else {
        cout << response.str() << endl;
    }

    return httpCode;
}
