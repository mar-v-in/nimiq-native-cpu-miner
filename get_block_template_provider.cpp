#include <utility>
#include "block_template.h"
#include "get_block_template_provider.h"

GetBlockTemplateProvider::GetBlockTemplateProvider (std::string url) : JsonRpcProvider (std::move (url)) {}

void GetBlockTemplateProvider::assign_work (Miner &miner) {
  Provider::assign_work (miner);
  GetBlockTemplateResult result = client.getBlockTemplate ();
  BlockBodyTemplate body = result.body ();
  BlockHeaderTemplate header = result.header ();
  header.timestamp = static_cast<uint32_t>(std::time (nullptr));
  header.body_hash = body.hash ();
  Uint8Array block = SerializeBlockFromTemplate (header, result.interlink (), body);
  Uint8Array header_serialized = header.serialize ();
  miner.update_work (
      header_serialized,
      Uint8Array (block.begin () + header_serialized.size (), block.end ()),
      result.target ());
}
