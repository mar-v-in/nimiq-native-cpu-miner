#ifndef NIMIQ_MINER_GET_BLOCK_TEMPLATE_PROVIDER_H
#define NIMIQ_MINER_GET_BLOCK_TEMPLATE_PROVIDER_H

#include "json_rpc_client.h"
#include "miner.h"
#include "provider.h"

class GetBlockTemplateProvider : public JsonRpcProvider {
 public:
  explicit GetBlockTemplateProvider (std::string url);
  void assign_work (Miner &miner);
};

#endif //NIMIQ_MINER_GET_BLOCK_TEMPLATE_PROVIDER_H
