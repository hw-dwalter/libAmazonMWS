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



#include "Order.h"

namespace AmazonMWS {
namespace Service {
  
const char* Order::calls[] = {
		"ListOrders",
		"ListOrdersByNextToken",
		"GetOrder",
		"ListOrderItems",
		"ListOrderItemsByNextToken",
		"GetServiceStatus"
};
const char* Order::name = "Orders";
const char* Order::version = "2011-01-01";

};
};