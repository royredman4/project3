///////////////////////////////////////////////////////////////////////////////
// CSUF CPSC 452, Summer 2017
// Project 2: SDES Key Expansion
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

#include <stdint.h>

// This class represents a bit string. Following our textbook, the
// first bit is at index 1, not index 0. Each bit is stored in an int,
// which is not space-efficient, but is convenient for this
// educational exercise.
class Bits {
public:
  // Construct an empty bit string.
  Bits() { }

  // Copy constructor.
  Bits(const Bits& other)
    : _zero_based(other._zero_based) { }

  // Create a bit string, width long, all initialized to bit.
  Bits(int width, int bit=0)
    : _zero_based(width, bit) {
    assert(width >= 0);
  }

  // Parse a string, which must contain only the 0 character, 1
  // character, or whitespace. Whitespace is ignored.
  Bits(const std::string& str) {
    for ( char ch : str ) {
      assert(isspace(ch) || (ch == '0') || (ch == '1'));
      if ( ! isspace(ch) ) {
	_zero_based.push_back(ch == '1');
      }
    }
  }

  // Comparison operator.
  bool operator==(const Bits& o) const {
    return (_zero_based == o._zero_based);
  }

  // Width/length accessor.
  int width() const {
    return _zero_based.size();
  }

  // Add one bit to the back (highest index) of the bit string,
  // incrementing its width.
  void push_back(int bit) {
    _zero_based.push_back(bit != 0);
  }

  // Square-bracket operator (array lookup). Note that the first bit
  // is at index 1.
  int operator[] (int index) const {
    assert((index >= 1) && (index <= width()));
    return _zero_based[index - 1];
  }
  int& operator[] (int index) {
    assert((index >= 1) && (index <= width()));
    return _zero_based[index - 1];
  }

  // Convert to a string, where the first bit is at the beginning of
  // the string. When separate_nybbles is true, there is a space
  // between every group of 4 binary digits.
  std::string to_string(bool separate_nybbles) const {
    std::string result;
    for (int i = 0, column = width() - 1; i < _zero_based.size(); i++, column--) {
      if (_zero_based[i]) {
	result.push_back('1');
      } else {
	result.push_back('0');
      }

      if (separate_nybbles &&
	  (i > 0) &&
	  (column > 0) &&
	  ((column % 4) == 0)) {
	result.push_back(' ');
      }
    }
    return result;
  }
  
private:
  std::vector<int> _zero_based;
};

// Permutation box (P-box).
class PBox {
public:
  // Construct a PBox; indices stores the source index numbers in
  // left-to-right order, as presented in the textbook. Each index
  // should be between 1, and the width of an input Bits, inclusive.
  PBox(const std::vector<int>& indices) {
    // TODO: implement this function and delete this comment
  }

  // Apply this P-box, i.e. actually do the bit permutation.
  Bits permute(const Bits& in) const {
    // TODO: implement this function and delete this comment
    return Bits();
  }

private:
  // TODO: add data members if any, and delete this comment
};


// Compute the SDES P8 function.
Bits sdes_p8(const Bits& in) {
  // TODO: implement this function and delete this comment
  return Bits();
}

// Compute the SDES P10 function.
Bits sdes_p10(const Bits& in) {
  // TODO: implement this function and delete this comment
  return Bits();
}

// Compute the SDES Shift function.
Bits sdes_shift(const Bits& in) {
  // TODO: implement this function and delete this comment
  return Bits();
}

// Compute the SDES K1 function.
Bits sdes_k1(const Bits& key) {
  // TODO: implement this function and delete this comment
  return Bits();
}

// Compute the SDES K2 function.
Bits sdes_k2(const Bits& key) {
  // TODO: implement this function and delete this comment
  return Bits();
}
