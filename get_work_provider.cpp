#include <utility>
#include "get_work_provider.h"

GetWorkProvider::GetWorkProvider (std::string url) : JsonRpcProvider (std::move (url)) {}

void GetWorkProvider::assign_work (Miner &miner) {
  Provider::assign_work (miner);
  GetWorkResult work = client.getWork ();
  miner.update_work (work.data (), work.suffix (), work.target ());
}