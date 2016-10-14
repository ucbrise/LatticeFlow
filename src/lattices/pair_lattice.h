#ifndef LATTICES_PAIR_LATTICE_H_
#define LATTICES_PAIR_LATTICE_H_

#include <type_traits>
#include <utility>

#include "lattices/lattice.h"

namespace latticeflow {

// Consider two semilattices L = (S_L, join_L) and R = (S_R, join_R). The
// semillatice P = (S_L x S_R, join_P) constructed from the cross product of L
// and R is itself a semillatice where
//
//   join_P((l1, r1), (l2, r2)) = (l1 join_L l2, r1 join_R r2)
//
// For example,
//
//   BoolAndLattice l1(true);
//   BoolAndLattice l2(false);
//   BoolOrLattice r1(true);
//   BoolOrLattice r2(false);
//   PairLattice<BoolAndLattice, BoolOrLattice> p1(l1, r1);
//   PairLattice<BoolAndLattice, BoolOrLattice> p2(l2, r2);
//   p1.join(p2); // = (true, true) join (false, false)
//                // = (true && false, true || false)
//                // = (false, true)
template <typename L, typename R>
class PairLattice : public Lattice<PairLattice<L, R>, std::pair<L, R>> {
  static_assert(std::is_base_of<Lattice<L, typename L::lattice_type>, L>::value,
                "Left type of PairLattice does not inherit from Lattice.");
  static_assert(std::is_base_of<Lattice<R, typename R::lattice_type>, R>::value,
                "Right type of PairLattice does not inherit from Lattice.");

 public:
  PairLattice() = default;
  PairLattice(const L& l, const R& r) : p_(l, r) {}
  PairLattice(const PairLattice<L, R>& l) = default;
  PairLattice& operator=(const PairLattice<L, R>& l) = default;

  const std::pair<L, R>& get() const override { return p_; }
  void join(const PairLattice<L, R>& l) override {
    std::get<0>(p_).join(std::get<0>(l.p_));
    std::get<1>(p_).join(std::get<1>(l.p_));
  }

 private:
  std::pair<L, R> p_;
};

}  // namespace latticeflow

#endif  // LATTICES_PAIR_LATTICE_H_
