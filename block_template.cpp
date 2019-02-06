#include <utility>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "block_template.h"
#include "core/src/native/blake2/blake2.h"

BlockBodyTemplate::BlockBodyTemplate (Uint8Array miner_address, std::vector<Uint8Array> transactions, std::vector<Uint8Array> merkle_hashes, std::vector<Uint8Array> pruned_accounts, Uint8Array extra_data)
    : miner_address (std::move (miner_address)), transactions (std::move (transactions)), merkle_hashes (std::move (merkle_hashes)), pruned_accounts (std::move (pruned_accounts)), extra_data (std::move (extra_data)) {}

Uint8Array BlockBodyTemplate::hash () {
  Uint8Array root (32), combined (64);
  blake2b (combined.data (), 32, miner_address.data (), miner_address.size (), nullptr, 0);
  blake2b (combined.data () + 32, 32, extra_data.data (), extra_data.size (), nullptr, 0);
  blake2b (root.data (), 32, combined.data (), 64, nullptr, 0);
  for (const Uint8Array &hash : merkle_hashes) {
    std::copy (root.begin (), root.end (), combined.begin ());
    std::copy (hash.begin (), hash.end (), combined.begin () + 32);
    blake2b (root.data (), 32, combined.data (), 64, nullptr, 0);
  }
  return root;
}

Uint8Array BlockBodyTemplate::serialize () {
  auto serialized_size = static_cast<uint32_t>(5 + extra_data.size () + miner_address.size ());
  for (const Uint8Array &tx : transactions) {
    serialized_size += tx.size ();
  }
  for (const Uint8Array &acc : pruned_accounts) {
    serialized_size += acc.size ();
  }

  Uint8Array res (serialized_size);

  std::copy (miner_address.begin (), miner_address.end (), res.begin ());
  auto offset = static_cast<uint32_t>(miner_address.size ());

  res[offset++] = static_cast<uint8_t >(extra_data.size ());
  std::copy (extra_data.begin (), extra_data.end (), res.begin () + offset);
  offset += extra_data.size ();

  res[offset++] = static_cast<uint8_t >(transactions.size () >> 8);
  res[offset++] = static_cast<uint8_t >(transactions.size ());
  for (const Uint8Array &tx : transactions) {
    std::copy (tx.begin (), tx.end (), res.begin () + offset);
    offset += tx.size ();
  }

  res[offset++] = static_cast<uint8_t >(pruned_accounts.size () >> 8);
  res[offset++] = static_cast<uint8_t >(pruned_accounts.size ());
  for (const Uint8Array &acc : pruned_accounts) {
    std::copy (acc.begin (), acc.end (), res.begin () + offset);
    offset += acc.size ();
  }

  return res;
}

BlockHeaderTemplate::BlockHeaderTemplate (uint16_t version, Uint8Array previous_hash, Uint8Array interlink_hash, Uint8Array accounts_hash, uint32_t target_compact, uint32_t height)
    : version (version), previous_hash (std::move (previous_hash)), interlink_hash (std::move (interlink_hash)), accounts_hash (std::move (accounts_hash)), target_compact (target_compact), height (height) {}

Uint8Array BlockHeaderTemplate::serialize () {
  Uint8Array res (146); // Block header is always 146 bytes for now
  res[0] = static_cast<uint8_t>(version >> 8);
  res[1] = static_cast<uint8_t>(version);
  std::copy (previous_hash.begin (), previous_hash.end (), res.begin () + 2);
  std::copy (interlink_hash.begin (), interlink_hash.end (), res.begin () + 34);
  std::copy (body_hash.begin (), body_hash.end (), res.begin () + 66);
  std::copy (accounts_hash.begin (), accounts_hash.end (), res.begin () + 98);
  auto write_at = [] (Uint8Array &res, uint8_t pos, uint32_t val) {
      res[pos] = static_cast<uint8_t>(val >> 24);
      res[pos + 1] = static_cast<uint8_t>(val >> 16);
      res[pos + 2] = static_cast<uint8_t>(val >> 8);
      res[pos + 3] = static_cast<uint8_t>(val);
  };
  write_at (res, 130, target_compact);
  write_at (res, 134, height);
  write_at (res, 138, timestamp);
  write_at (res, 142, 0);
  return res;
}

Uint8Array SerializeBlockFromTemplate (BlockHeaderTemplate header, Uint8Array interlink, BlockBodyTemplate body) {
  Uint8Array header_serialized = header.serialize ();
  Uint8Array body_serialized = body.serialize ();
  Uint8Array res (interlink.size () + header_serialized.size () + 1 + body_serialized.size ());
  std::move (header_serialized.begin (), header_serialized.end (), res.begin ());
  std::move (interlink.begin (), interlink.end (), res.begin () + header_serialized.size ());
  res[header_serialized.size () + interlink.size ()] = 1;
  std::move (body_serialized.begin (), body_serialized.end (),
             res.begin () + header_serialized.size () + interlink.size () + 1);
  return res;
}