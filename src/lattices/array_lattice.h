#ifndef LATTICES_ARRAY_LATTICE_H_
#define LATTICES_ARRAY_LATTICE_H_

#include <array>
#include <cstddef>
#include <initializer_list>
#include <type_traits>

#include "lattices/lattice.h"

namespace latticeflow {

// Consider an arbitrary lattice T = (S_T, join_T). We can form an array
// lattice of size N A = (S_t ^ N, join_A) where
//
//   join_A([x1, ..., xN], [y1, ..., yN]) = [x1 join_T y1, ..., xN join_T yN]
//
// That is, array semillatices are merged pairwise.
template <typename T, std::size_t N>
class ArrayLattice : public Lattice<ArrayLattice<T, N>, std::array<T, N>> {
  static_assert(std::is_base_of<Lattice<T, typename T::lattice_type>, T>::value,
                "Type of ArrayLattice does not inherit from Lattice.");

 public:
  ArrayLattice() = default;
  explicit ArrayLattice(std::array<T, N> xs) : xs_(xs) {}
  ArrayLattice(const ArrayLattice<T, N>& l) = default;
  ArrayLattice& operator=(const ArrayLattice<T, N>& l) = default;

  const std::array<T, N>& get() const override { return xs_; }

  void join(const ArrayLattice<T, N>& l) override {
    for (std::size_t i = 0; i < N; ++i) {
      xs_[i].join(l.xs_[i]);
    }
  }

 private:
  std::array<T, N> xs_;
};

}  // namespace latticeflow

#endif  // LATTICES_ARRAY_LATTICE_H_
