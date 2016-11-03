#ifndef ZMQ_UTIL_HEXDUMP_H_
#define ZMQ_UTIL_HEXDUMP_H_

#include <cstddef>
#include <ostream>

namespace latticeflow {

// Consider a file `cat.txt` with the following contents:
//
//   the sun did not shine. it was too wet to play.
//   so we sat in the house
//   all that cold, cold, wet day.
//
// If we run `hexdump -C cat.txt`, we get the following output:
//
// 0000000  74 68 65 20 73 75 6e 20  64 69 64 20 6e 6f 74 20  |the sun did not |
// 0000010  73 68 69 6e 65 2e 20 69  74 20 77 61 73 20 74 6f  |shine. it was to|
// 0000020  6f 20 77 65 74 20 74 6f  20 70 6c 61 79 2e 0a 73  |o wet to play..s|
// 0000030  6f 20 77 65 20 73 61 74  20 69 6e 20 74 68 65 20  |o we sat in the |
// 0000040  68 6f 75 73 65 0a 61 6c  6c 20 74 68 61 74 20 63  |house.all that c|
// 0000050  6f 6c 64 2c 20 63 6f 6c  64 2c 20 77 65 74 20 64  |old, cold, wet d|
// 0000060  61 79 2e 0a                                       |ay..|
// 0000064
//
// `hexdump` provides us with a nice way to visualize an array of bytes. This
// class implements the `hexdump` pretty pretting. That is, `std::cout <<
// HexDump(data, size)` pretty prints `data`, an byte array of length `size`,
// in a `hexdump -C` style.
class HexDump {
 public:
  HexDump(const unsigned char* const data, const std::size_t size)
      : data_(data), size_(size) {}

  friend std::ostream& operator<<(std::ostream& out, const HexDump& hexdump);

 private:
  const unsigned char* const data_;
  const std::size_t size_;
};

}  // namespace latticeflow

#endif  // ZMQ_UTIL_HEXDUMP_H_
