///////////////////////////////////////////////////////////////////////////////
// CSUF CPSC 452, Summer 2017
// Project 3: SDES Block Cipher
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>

#include "sdes_key.hh"

// SDES IP function. Input and output are both 8 bits wide.
Bits sdes_ip(const Bits& in) {
  assert(in.width() == 8);
  // TODO: replace the next line with code that actually works, and
  // delete this comment
  return Bits(8);
}

// SDES IP^-1 (inverse IP) function. Input and output are both 8 bits
// wide.
Bits sdes_ip_inverse(const Bits& in) {
  assert(in.width() == 8);
  // TODO: replace the next line with code that actually works, and
  // delete this comment
  return Bits(8);
}

// SDES IP^-1 (inverse IP) function. Input and output are both 8 bits
// wide.
Bits sdes_sw(const Bits& in) {
  assert(in.width() == 8);
  // TODO: replace the next line with code that actually works, and
  // delete this comment
  return Bits(8);
}

// SBox represents a substitution box (S-box), the same way that the
// PBox class represents a P-box.
class SBox {
public:
  // elements must be a 2D vector (i.e. vector of vectors) with
  // exactly 2 columns and 2 rows, where each element is a 2-bit
  // integer (i.e. between 0 and 3 inclusive). This constructor may be
  // called conveniently using an initializer list, for example:
  //
  //     SBox s({ {0, 1, 2, 3},
  //              {1, 2, 3, 0},
  //              {2, 3, 0, 1},
  //              {3, 0, 1, 2} });
  //
  SBox(std::vector<std::vector<int>> elements)
    : _elements(elements) {
    
    assert(elements.size() == 4);
    for ( auto row : elements ) {
      assert(row.size() == 4);
      for ( auto element : row ) {
	assert((element >= 0) && (element <= 3));
      }
    }
  }

  // Perform the substitution. Input must be 4 bits wide, and output
  // is 2 bits wide.
  Bits substitute(const Bits& in) const {
    assert(in.width() == 4);
    // TODO: replace the next line with code that actually works, and
    // delete this comment
    return Bits(2);
  }

private:
  std::vector<std::vector<int>> _elements;
};

// SDES F (capital F) function. Input is 4 bits wide, subkey is 8 bits
// wide, and output is 4 bits wide.
Bits sdes_F(const Bits& in, const Bits& subkey) {
  assert(in.width() == 4);
  assert(subkey.width() == 8);
  // TODO: replace the next line with code that actually works, and
  // delete this comment
  return Bits(4);
}

// SDES round function f (lowercase f). Subkey is 8 bits wide, input
// is 8 bits wide, and output is 8 bits wide. Note that the argument
// order is the opposite of that for sdes_F, in order to be consistent
// with the textbook's math notation.
Bits sdes_f(const Bits& subkey, const Bits& in) {
  assert(subkey.width() == 8);
  assert(in.width() == 8);
  // TODO: replace the next line with code that actually works, and
  // delete this comment
  return Bits(8);
}

// Encrypt one SDES block. Key is 10 bits wide, plaintext is 8 bits
// wide, and output is 8 bits wide.
Bits sdes_encrypt_block(const Bits& key, const Bits& plaintext) {
  assert(key.width() == 10);
  assert(plaintext.width() == 8);
  // TODO: replace the next line with code that actually works, and
  // delete this comment
  return Bits(8);
}

// Decrypt one SDES block. Key is 10 bits wide, plaintext is 8 bits
// wide, and output is 8 bits wide.
Bits sdes_decrypt_block(const Bits& key, const Bits& ciphertext) {
  assert(key.width() == 10);
  assert(ciphertext.width() == 8);
  // TODO: replace the next line with code that actually works, and
  // delete this comment
  return Bits(8);
}

// Convert a bits object to an unsigned integer. Bits cannot have more
// than 64 bits.
uint64_t bits_to_int(const Bits& bits) {
  assert(bits.width() <= 64);
  uint64_t result(0);
  for (int i = 0; i < bits.width(); i++) {
    result |= bits[bits.width() - i] << i;
  }
  return result;
}

// Convert an unsigned integer to a Bits object. width is the number
// of bits to use, which must be between 0 and 64 inclusive.
Bits int_to_bits(uint64_t bits, int width=8) {
  assert((width >= 0) && (width <= 64));
  Bits result(width);
  for (int i = 0; i < width; i++) {
    result[width - i] = (bits >> i) & 1;
  }
  assert(result.width() == width);
  return result;
}

// Encrypt a string with SDES. Each character is treated as its own
// block. The ciphertext characters are expected to range 0-255, so
// may include non-printable characters.
std::string sdes_encrypt_string(const Bits& key, const std::string& plaintext) {
  std::string ciphertext;
  for ( char c : plaintext ) {
    // We have a picky issue here: char is a signed 8-bit int, but our
    // Bits object expects an unsigned bit pattern. We need to convert
    // between signed and unsigned byte types in a way that is
    // deterministic and portable. We accomplish this with an
    // anonymous union that allows the same byte of memory to be
    // manipulated as either a signed or unsigned integer type. Then
    // we can write in signed mode and read in unsigned mode, or
    // vice-versa.
    union {
      char as_signed;
      uint8_t as_unsigned;
    } block;
    block.as_signed = c;
    Bits encrypted_block = sdes_encrypt_block(key, int_to_bits(block.as_unsigned));
    block.as_unsigned = bits_to_int(encrypted_block);
    ciphertext.push_back(block.as_signed);
  }
  return ciphertext;
}

// Decrypt a string with SDES. Each character is treated as its own block.
std::string sdes_decrypt_string(const Bits& key, const std::string& ciphertext) {
  std::string plaintext;
  for ( char c : ciphertext ) {
    // We use the same trick for signed-unsigned conversion as in
    // sdes_encrypt_string, above.
    union {
      char as_signed;
      uint8_t as_unsigned;
    } block;
    block.as_signed = c;
    Bits decrypted_block = sdes_decrypt_block(key, int_to_bits(block.as_unsigned));
    block.as_unsigned = bits_to_int(decrypted_block);
    plaintext.push_back(block.as_signed);
  }
  return plaintext;
}
