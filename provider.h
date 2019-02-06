#ifndef NIMIQ_MINER_PROVIDER_H
#define NIMIQ_MINER_PROVIDER_H

#include "json_rpc_client.h"
#include "miner.h"

class Provider {
 public:
  uint32_t submissions = 0;
  time_t last_submission;
  time_t last_work;
  virtual void submit_block (Uint8Array block);
  virtual void assign_work (Miner &miner);
};

class JsonRpcProvider : public Provider {
 protected:
  JsonRpcClient client;
 public:
  explicit JsonRpcProvider (std::string url);
  void submit_block (Uint8Array block) final;
};

#endif //NIMIQ_MINER_PROVIDER_H
