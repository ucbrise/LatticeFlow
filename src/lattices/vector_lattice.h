#ifndef LATTICES_VECTOR_LATTICE_H_
#define LATTICES_VECTOR_LATTICE_H_

#include <initializer_list>
#include <type_traits>
#include <vector>

#include "lattices/lattice.h"

namespace latticeflow {

// Consider a semillatice T = (S_T, join_T). We can form a vector semilattice V
// = (std::vector<S_T>, join_V) where join_V is defined as follows. Let a =
// [a_1, ..., a_m] and b = [b_1, ..., b_n] be two vectors of S_T where m <= n.
//
//   join_V(a, b) = [a_1 join b_1, ..., a_m join b_m, b_{m+1}, ..., b_n]
//
// For example,
//
//   VectorLattice<BoolAndLattice> a = {
//     BoolAndLattice(true),
//     BoolAndLattice(false),
//     BoolAndLattice(true)
//   };
//   VectorLattice<BoolAndLattice> b = {
//     BoolAndLattice(false),
//     BoolAndLattice(false),
//     BoolAndLattice(true),
//     BoolAndLattice(false),
//     BoolAndLattice(true)
//   };
//   a.join(b); // = [true, false, true] join [false, false, true, false, true]
//              // = [true && false, false && false, true && true, false, true]
//              // = [false, false, true, false, true]
template <typename T>
class VectorLattice : public Lattice<VectorLattice<T>, std::vector<T>> {
  static_assert(std::is_base_of<Lattice<T, typename T::lattice_type>, T>::value,
                "Type of VectorLattice does not inherit from Lattice.");

 public:
  VectorLattice() = default;
  explicit VectorLattice(std::initializer_list<T> xs) : xs_(xs) {}
  VectorLattice(const VectorLattice<T>& l) = default;
  VectorLattice& operator=(const VectorLattice<T>& l) = default;

  const std::vector<T>& get() const override { return xs_; }

  void join(const VectorLattice<T>& l) override {
    for (std::size_t i = 0; i < std::min(xs_.size(), l.xs_.size()); ++i) {
      xs_[i].join(l.xs_[i]);
    }
    if (xs_.size() < l.xs_.size()) {
      xs_.insert(xs_.end(), l.xs_.begin() + xs_.size(), l.xs_.end());
    }
  }

 private:
  std::vector<T> xs_;
};

}  // namespace latticeflow

#endif  // LATTICES_VECTOR_LATTICE_H_
