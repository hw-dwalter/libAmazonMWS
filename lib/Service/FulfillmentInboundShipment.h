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



#ifndef FULFILLMENTINBOUNDSHIPMENT_H
#define FULFILLMENTINBOUNDSHIPMENT_H

namespace AmazonMWS {
namespace Service {

struct FulfillmentInboundShipment
{
    enum Call {
        CreateInboundShipmentPlan = 0,
        CreateInboundShipment,
        UpdateInboundShipment,
        ListInboundShipments,
        ListInboundShipmentsByNextToken,
        ListInboundShipmentItems,
	ListInboundShipmentItemsByNextToken,
	GetServiceStatus
    };

    static const char* calls[];
    static const char* name;
    static const char* version;
};

};
};

#endif // FULFILLMENTINBOUNDSHIPMENT_H
