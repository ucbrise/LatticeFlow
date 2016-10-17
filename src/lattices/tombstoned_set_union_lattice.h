#ifndef LATTICES_TOMBSTONED_SET_UNION_LATTICE_H_
#define LATTICES_TOMBSTONED_SET_UNION_LATTICE_H_

#include "lattices/bool_or_lattice.h"
#include "lattices/map_lattice.h"

namespace latticeflow {

// With an increasing SetUnionLattice, elements can only be added to the set;
// they can never be removed. A TombstonedSetUnionLattice allows for elements
// to be deleted by associating a boolean tombstone with the item. If the
// associated boolean is false, the item is thought of as in the set. If it is
// false, the element is thought of as deleted. Once an element is deleted, it
// can never again be inserted.
template <typename T>
using TombstonedSetUnionLattice = MapLattice<T, BoolOrLattice>;

}  // namespace latticeflow

#endif  // LATTICES_TOMBSTONED_SET_UNION_LATTICE_H_
