#ifndef NIMIQ_MINER_WORK_CLIENT_H
#define NIMIQ_MINER_WORK_CLIENT_H

#include "miner.h"
#include "provider.h"

class GetWorkProvider : public JsonRpcProvider {
 public:
  explicit GetWorkProvider (std::string url);
  void assign_work (Miner &miner);
};

#endif //NIMIQ_MINER_WORK_CLIENT_H
