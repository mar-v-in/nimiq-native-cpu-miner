#ifndef NIMIQ_MINER_BLOCK_TEMPLATE_H
#define NIMIQ_MINER_BLOCK_TEMPLATE_H

#include "utils.h"

class BlockBodyTemplate {
  Uint8Array miner_address;
  std::vector<Uint8Array> transactions;
  std::vector<Uint8Array> merkle_hashes;
  std::vector<Uint8Array> pruned_accounts;

 public:
  Uint8Array extra_data;
  BlockBodyTemplate (Uint8Array miner_address, std::vector<Uint8Array> transactions, std::vector<Uint8Array> merkle_hashes, std::vector<Uint8Array> pruned_accounts, Uint8Array extra_data = {});
  Uint8Array serialize ();
  Uint8Array hash ();
};

class BlockHeaderTemplate {
  uint16_t version;
  Uint8Array previous_hash;
  Uint8Array interlink_hash;
  Uint8Array accounts_hash;
  uint32_t target_compact;
  uint32_t height;

 public:
  Uint8Array body_hash;
  uint32_t timestamp;
  BlockHeaderTemplate (uint16_t version, Uint8Array previous_hash, Uint8Array interlink_hash, Uint8Array accounts_hash, uint32_t target_compact, uint32_t height);
  Uint8Array serialize ();
};

Uint8Array SerializeBlockFromTemplate (BlockHeaderTemplate header, Uint8Array interlink, BlockBodyTemplate body);

#endif //NIMIQ_MINER_BLOCK_TEMPLATE_H
