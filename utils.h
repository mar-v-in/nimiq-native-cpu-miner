#ifndef NIMIQ_MINER_UTILS_H
#define NIMIQ_MINER_UTILS_H

#include <vector>
#include <cstdint>
#include <string>

typedef std::vector<uint8_t> Uint8Array;
Uint8Array HexToBin (std::string in);
std::string BinToHex (Uint8Array in);

#endif //NIMIQ_MINER_UTILS_H
