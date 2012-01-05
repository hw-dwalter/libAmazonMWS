libAmazonMWS
===========

libAmazonMWS is a REST client library for Amazon MWS Webservice.

German description is available here https://www.helmundwalter.de/blog/amazonmws-cxx-webservice/

Useage
------

### Sample 1 (Basic)

  Client<Service::Order> orderClient(
      "mws.amazonservices.de",	// Endpoint
      "AXIXIXZX2X6X5XNXFXUX",	// AccessKey
      "Yayykyl/yfy4yYV3YoyBYRY7",	// SecretAccessKey
      "A2ZTZ7ZSZBZKZZ"		// MerchantId
  );
  double httpCode = orderClient.call(Service::Order::GetServiceStatus, cout);

Will produce output similar to this:

  <?xml version="1.0"?>
  <GetServiceStatusResponse xmlns="https://mws.amazonservices.com/Orders/2011-01-01">
	  <GetServiceStatusResult>
		  <Status>GREEN</Status>
		  <Timestamp>2012-01-04T08:54:28.503Z</Timestamp>
	  </GetServiceStatusResult>
	  <ResponseMetadata>
		  <RequestId>d0b4beba-80b8-4dc0-ae85-e9f3b8811838</RequestId>
	  </ResponseMetadata>
  </GetServiceStatusResponse>

### Sample 2 (Advanced)

  stringstream response;
  paramMap_t request;
  request["LastUpdatedAfter"] = timestamp(second_clock::local_time() - hours(2));
  request["MaxResultsPerPage"] = "5";
  request["MarketplaceId.Id.1"] = "A1F83G8C2ARO7P"; //Amazon.co.uk
  request["MarketplaceId.Id.2"] = "A1PA6795UKMFR9"; //Amazon.de
  request["MarketplaceId.Id.3"] = "A13V1IB3VIYZZH"; //Amazon.fr
  request["MarketplaceId.Id.4"] = "APJ6JRA9NG5V4";  //Amazon.it

double httpCode = orderClient.call(Service::Order::ListOrders, request, response);

Response holds now 5 orders which are changed in the last 2 hours on the given marketplaces.
In the complete samples the response will be processed further with the pugi xml parser.

Internals
---------

Internally these library makes use of boost for timestamp calculation, libcurl für HTTP processing
and OpenSSL for base64 calculation and HMAC SHA256 encryption.