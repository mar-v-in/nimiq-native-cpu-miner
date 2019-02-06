#ifndef NIMIQ_MINER_CORENATIVEMINER_H
#define NIMIQ_MINER_CORENATIVEMINER_H

#include <future>
#include "miner.h"

class CoreNativeMiner : public Miner {
  bool running;
  std::vector<std::future<void>> futures;

 public:
  CoreNativeMiner () = default;
  void start () final;
  void stop () final;
  void single_thread ();
};

#endif //NIMIQ_MINER_CORENATIVEMINER_H
