add_subdirectory(Service)

set(libSrcs
CURLGlobal.cpp
BasicClient.cpp
Client.h
Service/Feed.cpp
Service/FulfillmentOutboundShipment.cpp
Service/FulfillmentInventory.cpp
Service/FulfillmentInboundShipment.cpp
Service/Order.cpp
Service/Report.cpp
Service/Sellers.cpp
)

add_library(amazonmws SHARED ${libSrcs})

