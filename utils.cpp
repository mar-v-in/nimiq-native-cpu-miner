#include "utils.h"

Uint8Array HexToBin (std::string in) {
  size_t pos;
  uint8_t idx0;
  uint8_t idx1;

  // mapping of ASCII characters to hex values
  const uint8_t hashmap[] =
      {
          0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, // 01234567
          0x08, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 89:;<=>?
          0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, // @ABCDEFG
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // HIJKLMNO
      };

  std::vector<uint8_t> out (in.length () / 2);
  for (pos = 0; pos < in.length (); pos += 2) {
    idx0 = ((uint8_t) in[pos + 0] & 0x1F) ^ 0x10;
    idx1 = ((uint8_t) in[pos + 1] & 0x1F) ^ 0x10;
    out[pos / 2] = (uint8_t) (hashmap[idx0] << 4) | hashmap[idx1];
  };
  return out;
}

std::string BinToHex (Uint8Array in) {
  uint8_t *_in = in.data ();
  std::string out (in.size () * 2, ' ');
  unsigned char *pin = _in;
  const char *hex = "0123456789abcdef";
  uint32_t pout = 0;
  for (; pin < _in + in.size (); pout += 2, pin++) {
    out[pout] = hex[(*pin >> 4) & 0xF];
    out[pout + 1] = hex[*pin & 0xF];
  }
  return out;
}