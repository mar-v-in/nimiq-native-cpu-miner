#include <iostream>
#include "core_native_miner.h"

extern "C" {
#include "nimiq_native.h"
}

void CoreNativeMiner::start () {
  running = true;
  for(int i = 0; i < 8; ++i) {
    futures.push_back (std::async (std::launch::async, &CoreNativeMiner::single_thread, this));
  }
}

void CoreNativeMiner::stop () {
  running = false;
}

void CoreNativeMiner::single_thread () {
  while (running) {
    uint8_t out[32];
    MiningTask task = gather_task (1024);
    uint8_t in[task.data->size ()];
    std::copy (task.data->begin (), task.data->end (), in);
    uint32_t result_nonce = nimiq_argon2_target (out, in, task.data->size (), task.target_compact, task.nonce_start, task.nonce_end, 0);
    if (result_nonce < task.nonce_end) {
      this->report_task_success (task, result_nonce);
    }
  }
}

