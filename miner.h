#ifndef NIMIQ_MINER_NIMIQMINER_H
#define NIMIQ_MINER_NIMIQMINER_H

#include <memory>
#include <chrono>
#include <mutex>
#include "utils.h"

class MinerCallback {
 public:
  virtual void submit_block (Uint8Array block) = 0;
};

class MiningTask {
 public:
  std::shared_ptr<Uint8Array> data;
  std::shared_ptr<Uint8Array> suffix;
  uint32_t target_compact;
  uint32_t nonce_start;
  uint32_t nonce_end;
  MiningTask (std::shared_ptr<Uint8Array> data, std::shared_ptr<Uint8Array> suffix, uint32_t target_compact, uint32_t nonce_start, uint32_t nonce_end);
};

class Miner {
  MinerCallback *callback;

  std::mutex miner_task_mutex;
  std::shared_ptr<Uint8Array> data;
  std::shared_ptr<Uint8Array> suffix;
  uint32_t target_compact;
  uint32_t next_nonce;

  // For hashrate calculation
  uint32_t nonce_counter;
  std::chrono::steady_clock::time_point last_calculation;
  double last_hashrate;

 protected:
  MiningTask gather_task (uint32_t size);
  void report_task_success (MiningTask &task, uint32_t nonce);

 public:
  virtual void setup (MinerCallback *callback);
  virtual void start () = 0;
  virtual void stop () = 0;
  void update_work (Uint8Array data, Uint8Array suffix, uint32_t target_compact);

  double current_hashrate ();
};

#endif //NIMIQ_MINER_NIMIQMINER_H
