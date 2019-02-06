#ifndef NIMIQ_MINER_NIMIQJSONRPCCLIENT_H
#define NIMIQ_MINER_NIMIQJSONRPCCLIENT_H

#include "utils.h"
#include "block_template.h"
#include <cpprest/http_client.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class GetWorkResult {
  json::value result;

 public:
  explicit GetWorkResult (json::value result);
  Uint8Array data ();
  Uint8Array suffix ();
  uint32_t target ();
};

class GetBlockTemplateResult {
  json::value result;

 public:
  explicit GetBlockTemplateResult (json::value result);
  BlockHeaderTemplate header ();
  Uint8Array interlink ();
  BlockBodyTemplate body ();
  uint32_t target ();
};

class JsonRpcClient {
  http_client client;

  pplx::task<http_response> request (std::string method, std::vector<json::value> params = std::vector<json::value> (0));

 public:
  explicit JsonRpcClient (std::string url);
  GetWorkResult getWork (Uint8Array address = {}, Uint8Array extra_data = {});
  GetBlockTemplateResult getBlockTemplate (Uint8Array address = {}, Uint8Array extra_data = {});
  void submitBlock (Uint8Array block);
};

#endif //NIMIQ_MINER_NIMIQJSONRPCCLIENT_H
