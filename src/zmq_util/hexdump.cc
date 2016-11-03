#include "zmq_util/hexdump.h"

#include <cassert>
#include <cctype>
#include <iomanip>
#include <ios>
#include <iostream>

namespace latticeflow {
namespace {

// `std::cout << Hex(data, size)` prints each byte of `data` in hex separated by
// a space. For example, `Hex("the sun did not ", 16)` looks like this:
//
//   74 68 65 20 73 75 6e 20 64 69 64 20 6e 6f 74 20
class Hex {
 public:
  Hex(const unsigned char* const data, const std::size_t size)
      : data_(data), size_(size) {}

  friend std::ostream& operator<<(std::ostream& out, const Hex& hex) {
    for (std::size_t i = 0; i < hex.size_; ++i) {
      if (i != 0) {
        out << ' ';
      }
      out << std::setw(2) << std::setfill('0') << std::hex << +hex.data_[i];
    }
    return out;
  }

 private:
  const unsigned char* const data_;
  const std::size_t size_;
};

// `std::cout << Ascii(data, size)` prints each printable byte in `data` or `.`
// if the byte is not printable. For example `Hex("the sun did not ", 16)`
// looks like this:
//
//   the sun did not
class Ascii {
 public:
  Ascii(const unsigned char* const data, const std::size_t size)
      : data_(data), size_(size) {}

  friend std::ostream& operator<<(std::ostream& out, const Ascii& ascii) {
    for (std::size_t i = 0; i < ascii.size_; ++i) {
      if (std::isprint(ascii.data_[i])) {
        out << ascii.data_[i];
      } else {
        out << ".";
      }
    }
    return out;
  }

 private:
  const unsigned char* const data_;
  const std::size_t size_;
};

}  // namespace

//  index
//  width    half row width           half row width           row width
//  _____    _____________________    _____________________    ________________
// /     \  /                     \  /                     \  /                \
// 0000000  74 68 65 20 73 75 6e 20  64 69 64 20 6e 6f 74 20  |the sun did not |
// 0000010  73 68 69 6e 65 2e 20 69  74 20 77 61 73 20 74 6f  |shine. it was to|
// 0000020  6f 20 77 65 74 20 74 6f  20 70 6c 61 79 2e 0a 73  |o wet to play..s|
// 0000030  6f 20 77 65 20 73 61 74  20 69 6e 20 74 68 65 20  |o we sat in the |
// 0000040  68 6f 75 73 65 0a 61 6c  6c 20 74 68 61 74 20 63  |house.all that c|
// 0000050  6f 6c 64 2c 20 63 6f 6c  64 2c 20 77 65 74 20 64  |old, cold, wet d|
std::ostream& operator<<(std::ostream& out, const HexDump& hexdump) {
  const std::size_t index_width = 8;
  const std::size_t row_width = 16;
  const std::size_t half_row_width = row_width / 2;

  if (hexdump.size_ == 0) {
    out << std::setw(index_width) << std::setfill('0') << 0 << ' '
        << std::string(3 * half_row_width, ' ') << ' '
        << std::string(3 * half_row_width, ' ') << "  ||";
    return out;
  }

  // Output full rows.
  std::size_t i = 0;
  for (i = 0; i + row_width < hexdump.size_; i += row_width) {
    if (i != 0) {
      out << std::endl;
    }
    out << std::setw(index_width) << std::setfill('0') << i << "  "
        << Hex(&hexdump.data_[i], half_row_width) << "  "
        << Hex(&hexdump.data_[i + half_row_width], half_row_width) << "  |"
        << Ascii(&hexdump.data_[i], row_width) << "|";
  }

  const std::size_t num_chars_left = hexdump.size_ - i;

  if (num_chars_left > 0 && i != 0) {
    out << std::endl;
  }

  if (num_chars_left <= 0) {
    return out;
  } else if (num_chars_left <= half_row_width) {
    // Partial first half row.
    out << std::setw(index_width) << std::setfill('0') << i << "  "
        << Hex(&hexdump.data_[i], num_chars_left)
        << std::string((half_row_width - num_chars_left) * 3, ' ') << ' '
        << std::string(half_row_width * 3, ' ') << "  |"
        << Ascii(&hexdump.data_[i], num_chars_left) << "|";
  } else if (num_chars_left <= row_width) {
    // Partial second half row.
    out << std::setw(index_width) << std::setfill('0') << i << "  "
        << Hex(&hexdump.data_[i], half_row_width) << "  "
        << Hex(&hexdump.data_[i + half_row_width],
               num_chars_left - half_row_width)
        << std::string((row_width - num_chars_left) * 3, ' ') << "  |"
        << Ascii(&hexdump.data_[i], num_chars_left) << "|";
  } else {
    assert(false);
  }

  return out;
}

}  // namespace latticeflow
