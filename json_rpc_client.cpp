#include <utility>

#include <utility>

#include <utility>
#include "json_rpc_client.h"

GetWorkResult::GetWorkResult (json::value result) : result (std::move (result)) {}

Uint8Array GetWorkResult::data () {
  return HexToBin (result["data"].as_string ());
}

Uint8Array GetWorkResult::suffix () {
  return HexToBin (result["suffix"].as_string ());
}

uint32_t GetWorkResult::target () {
  return result["target"].as_number ().to_uint32 ();
}

GetBlockTemplateResult::GetBlockTemplateResult (json::value result) : result (std::move (result)) {}

BlockHeaderTemplate GetBlockTemplateResult::header () {
  return BlockHeaderTemplate (
      static_cast<uint16_t>(result["header"]["version"].as_number ().to_uint32 ()),
      HexToBin (result["header"]["prevHash"].as_string ()),
      HexToBin (result["header"]["interlinkHash"].as_string ()),
      HexToBin (result["header"]["accountsHash"].as_string ()),
      result["header"]["nBits"].as_number ().to_uint32 (),
      result["header"]["height"].as_number ().to_uint32 ());
}

Uint8Array GetBlockTemplateResult::interlink () {
  return HexToBin (result["interlink"].as_string ());
}

BlockBodyTemplate GetBlockTemplateResult::body () {
  json::array &json_transactions = result["body"]["transactions"].as_array ();
  json::array &json_merkle_hashes = result["body"]["merkleHashes"].as_array ();
  json::array &json_pruned_accounts = result["body"]["prunedAccounts"].as_array ();

  std::vector<Uint8Array> transactions (json_transactions.size ());
  std::vector<Uint8Array> merkle_hashes (json_merkle_hashes.size ());
  std::vector<Uint8Array> pruned_accounts (json_pruned_accounts.size ());

  std::transform (json_transactions.begin (), json_transactions.end (), transactions.begin (), [] (json::value value) -> Uint8Array { return HexToBin (value.as_string ()); });
  std::transform (json_merkle_hashes.begin (), json_merkle_hashes.end (), merkle_hashes.begin (), [] (json::value value) -> Uint8Array { return HexToBin (value.as_string ()); });
  std::transform (json_pruned_accounts.begin (), json_pruned_accounts.end (), pruned_accounts.begin (), [] (json::value value) -> Uint8Array { return HexToBin (value.as_string ()); });

  return BlockBodyTemplate (
      HexToBin (result["body"]["minerAddr"].as_string ()),
      transactions,
      merkle_hashes,
      pruned_accounts,
      HexToBin (result["body"]["extraData"].as_string ()));
}

uint32_t GetBlockTemplateResult::target () {
  return result["target"].as_number ().to_uint32 ();
}

JsonRpcClient::JsonRpcClient (std::string url) : client (uri::decode (url)) {}

pplx::task<http_response> JsonRpcClient::request (std::string method, std::vector<json::value> params) {
  json::value request = json::value::object ();
  request["jsonrpc"] = json::value ("2.0");
  request["id"] = json::value (1);
  request["method"] = json::value (std::move (method));
  request["params"] = json::value::array (std::move (params));
  auto user_info = client.base_uri ().user_info ();
  auto div = user_info.find_first_of (':');
  http_client_config config;
  config.set_credentials (credentials (user_info.substr (0, div), user_info.substr (div + 1, user_info.length ())));
  client = http_client (client.base_uri (), config);
  return client.request (methods::POST, "/", request.serialize ());
}

GetWorkResult JsonRpcClient::getWork (Uint8Array address, Uint8Array extra_data) {
  return GetWorkResult (request ("getWork", {
      json::value (BinToHex (std::move (address))),
      json::value (BinToHex (std::move (extra_data)))}
  ).get ().extract_json (true).get ().at ("result"));
}

GetBlockTemplateResult JsonRpcClient::getBlockTemplate (Uint8Array address, Uint8Array extra_data) {
  return GetBlockTemplateResult (request ("getBlockTemplate", {
      json::value (BinToHex (std::move (address))),
      json::value (BinToHex (std::move (extra_data)))}
  ).get ().extract_json (true).get ().at ("result"));
}

void JsonRpcClient::submitBlock (Uint8Array block) {
  request ("submitBlock", {json::value (BinToHex (std::move (block)))});
}
