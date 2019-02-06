#include <iomanip>
#include "provider.h"

void Provider::submit_block (Uint8Array block) {
  submissions++;
  last_submission = std::time (nullptr);
}

void Provider::assign_work (Miner &miner) {
  last_work = std::time (nullptr);
}

JsonRpcProvider::JsonRpcProvider (std::string url) : client (std::move (url)) {}

void JsonRpcProvider::submit_block (Uint8Array block) {
  Provider::submit_block (block);
  client.submitBlock (std::move (block));
}
