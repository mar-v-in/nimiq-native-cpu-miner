#include "miner.h"
#include <algorithm>
#include <utility>
#include <iostream>

MiningTask::MiningTask (std::shared_ptr<Uint8Array> data, std::shared_ptr<Uint8Array> suffix, uint32_t target_compact, uint32_t nonce_start, uint32_t nonce_end)
    : data (std::move (data)), suffix (std::move (suffix)), target_compact (target_compact), nonce_start (nonce_start), nonce_end (nonce_end) {}

void Miner::setup (MinerCallback *callback) {
  this->callback = callback;
  last_calculation = std::chrono::steady_clock::now ();
  nonce_counter = 0;
}

void Miner::update_work (Uint8Array data, Uint8Array suffix, uint32_t target_compact) {
  std::lock_guard<std::mutex> guard (miner_task_mutex);
  if (this->data == nullptr || !std::equal (this->data->begin (), this->data->end (), data.begin ())) {
    this->data = std::make_shared<Uint8Array> (std::move (data));
    this->suffix = std::make_shared<Uint8Array> (std::move (suffix));
    next_nonce = 0;
  }
  this->target_compact = target_compact;
}

MiningTask Miner::gather_task (uint32_t size) {
  std::lock_guard<std::mutex> guard (miner_task_mutex);
  uint32_t min_nonce = next_nonce;
  next_nonce += size;
  nonce_counter += size;
  return MiningTask (data, suffix, target_compact, min_nonce, next_nonce);
}

void Miner::report_task_success (MiningTask &task, uint32_t nonce) {
  //std::cout << "Submitting share with nonce " << nonce << "." << std::endl;
  Uint8Array block (task.data->size () + task.suffix->size ());
  std::copy (task.data->begin (), task.data->end (), block.begin ());
  std::copy (task.suffix->begin (), task.suffix->end (), block.begin () + task.data->size ());
  block[task.data->size () - 4] = static_cast<uint8_t>(nonce >> 24);
  block[task.data->size () - 3] = static_cast<uint8_t>(nonce >> 16);
  block[task.data->size () - 2] = static_cast<uint8_t>(nonce >> 8);
  block[task.data->size () - 1] = static_cast<uint8_t>(nonce);
  callback->submit_block (block);
}

template<class duration>
double DiffToSeconds (const duration &diff) {
  return diff.count () * ((double) duration::period::num / duration::period::den);
}

double Miner::current_hashrate () {
  auto now = std::chrono::steady_clock::now ();
  std::lock_guard<std::mutex> guard (this->miner_task_mutex);
  auto diff_seconds = DiffToSeconds (now - this->last_calculation);
  if (diff_seconds > 1) {
    this->last_hashrate = ((double) this->nonce_counter) / diff_seconds;
    this->nonce_counter = 0;
    this->last_calculation = now;
  }
  return this->last_hashrate;
}
