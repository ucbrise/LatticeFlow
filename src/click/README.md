# Push and Pull
A collection of Click-style push and pull operators.

```
// `Call(f).push(x)` perfect forwards `x` to `f` where `x` is an lvalue
// reference or rvalue reference. The type of `f`, `F`, is a template
// parameter, which means the type of `f`'s argument is generic. Here is a
// refresher on which combinations of `T` and `F` will compile.
//
// Assuming A is CopyConstructible.
//   +---------------+-----------------------------------------------+
//   |               | argument of F                                 |
//   |               +---+----+-----+---------+----------+-----------+
//   |               | A | A& | A&& | const A | const A& | const A&& |
//   +---+-----------+---+----+-----+---------+----------+-----------+
//   | T | A         | y | n  | y   | y       | y        | y         |
//   |   | A&        | y | y  | n   | y       | y        | n         |
//   |   | A&&       | y | n  | y   | y       | y        | y         |
//   |   | const A   | y | n  | n   | y       | y        | y         |
//   |   | const A&  | y | n  | n   | y       | y        | n         |
//   |   | const A&& | y | n  | n   | y       | y        | y         |
//   +---+-----------+---+----+-----+---------+----------+-----------+
//
// Assuming A is not CopyConstructible.
//   +---------------+-----------------------------------------------+
//   |               | argument of F                                 |
//   |               +---+----+-----+---------+----------+-----------+
//   |               | A | A& | A&& | const A | const A& | const A&& |
//   +---+-----------+---+----+-----+---------+----------+-----------+
//   | T | A         | y | n  | y   | y       | y        | y         |
//   |   | A&        | n | y  | n   | n       | y        | n         |
//   |   | A&&       | y | n  | y   | y       | y        | y         |
//   |   | const A   | n | n  | n   | n       | y        | y         |
//   |   | const A&  | n | n  | n   | n       | y        | n         |
//   |   | const A&& | n | n  | n   | n       | y        | y         |
//   +---+-----------+---+----+-----+---------+----------+-----------+
//
// If you'd like to verify these tables, you can use the following code:
//
// class Copy {
//  public:
//   Copy() {}
//   Copy(const Copy&) = default;
//   Copy(Copy&&) = default;
// };
//
// class NonCopy {
//  public:
//   NonCopy() {}
//   NonCopy(const NonCopy&) = delete;
//   NonCopy(NonCopy&&) = default;
// };
//
// namespace copyable {
// void nref(Copy) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// void lref(Copy&) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// void rref(Copy&&) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// void cnref(const Copy) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// void clref(const Copy&) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// void crref(const Copy&&) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// }  // namespace copyable
//
// namespace noncopyable {
// void nref(NonCopy) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// void lref(NonCopy&) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// void rref(NonCopy&&) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// void cnref(const NonCopy) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// void clref(const NonCopy&) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// void crref(const NonCopy&&) {std::cout << __PRETTY_FUNCTION__ << std::endl;};
// }  // namespace noncopyable
//
// int main() {
//   {
//     using namespace copyable;
//
//     { lf::Call<Copy, decltype(&nref)> x(nref); }
//     // { lf::Call<Copy, decltype(&lref)> x(lref); }
//     { lf::Call<Copy, decltype(&rref)> x(rref); }
//     { lf::Call<Copy, decltype(&cnref)> x(cnref); }
//     { lf::Call<Copy, decltype(&clref)> x(clref); }
//     { lf::Call<Copy, decltype(&crref)> x(crref); }
//
//     { lf::Call<Copy&, decltype(&nref)> x(nref); }
//     { lf::Call<Copy&, decltype(&lref)> x(lref); }
//     // { lf::Call<Copy&, decltype(&rref)> x(rref); }
//     { lf::Call<Copy&, decltype(&cnref)> x(cnref); }
//     { lf::Call<Copy&, decltype(&clref)> x(clref); }
//     // { lf::Call<Copy&, decltype(&crref)> x(crref); }
//
//     { lf::Call<Copy&&, decltype(&nref)> x(nref); }
//     // { lf::Call<Copy&&, decltype(&lref)> x(lref); }
//     { lf::Call<Copy&&, decltype(&rref)> x(rref); }
//     { lf::Call<Copy&&, decltype(&cnref)> x(cnref); }
//     { lf::Call<Copy&&, decltype(&clref)> x(clref); }
//     { lf::Call<Copy&&, decltype(&crref)> x(crref); }
//
//     { lf::Call<const Copy, decltype(&nref)> x(nref); }
//     // { lf::Call<const Copy, decltype(&lref)> x(lref); }
//     // { lf::Call<const Copy, decltype(&rref)> x(rref); }
//     { lf::Call<const Copy, decltype(&cnref)> x(cnref); }
//     { lf::Call<const Copy, decltype(&clref)> x(clref); }
//     { lf::Call<const Copy, decltype(&crref)> x(crref); }
//
//     { lf::Call<const Copy&, decltype(&nref)> x(nref); }
//     // { lf::Call<const Copy&, decltype(&lref)> x(lref); }
//     // { lf::Call<const Copy&, decltype(&rref)> x(rref); }
//     { lf::Call<const Copy&, decltype(&cnref)> x(cnref); }
//     { lf::Call<const Copy&, decltype(&clref)> x(clref); }
//     // { lf::Call<const Copy&, decltype(&crref)> x(crref); }
//
//     { lf::Call<const Copy&&, decltype(&nref)> x(nref); }
//     // { lf::Call<const Copy&&, decltype(&lref)> x(lref); }
//     // { lf::Call<const Copy&&, decltype(&rref)> x(rref); }
//     { lf::Call<const Copy&&, decltype(&cnref)> x(cnref); }
//     { lf::Call<const Copy&&, decltype(&clref)> x(clref); }
//     { lf::Call<const Copy&&, decltype(&crref)> x(crref); }
//   }
//
//   {
//     using namespace noncopyable;
//
//     { lf::Call<NonCopy, decltype(&nref)> x(nref); }
//     // { lf::Call<NonCopy, decltype(&lref)> x(lref); }
//     { lf::Call<NonCopy, decltype(&rref)> x(rref); }
//     { lf::Call<NonCopy, decltype(&cnref)> x(cnref); }
//     { lf::Call<NonCopy, decltype(&clref)> x(clref); }
//     { lf::Call<NonCopy, decltype(&crref)> x(crref); }
//
//     // { lf::Call<NonCopy&, decltype(&nref)> x(nref); }
//     { lf::Call<NonCopy&, decltype(&lref)> x(lref); }
//     // { lf::Call<NonCopy&, decltype(&rref)> x(rref); }
//     // { lf::Call<NonCopy&, decltype(&cnref)> x(cnref); }
//     { lf::Call<NonCopy&, decltype(&clref)> x(clref); }
//     // { lf::Call<NonCopy&, decltype(&crref)> x(crref); }
//
//     { lf::Call<NonCopy&&, decltype(&nref)> x(nref); }
//     // { lf::Call<NonCopy&&, decltype(&lref)> x(lref); }
//     { lf::Call<NonCopy&&, decltype(&rref)> x(rref); }
//     { lf::Call<NonCopy&&, decltype(&cnref)> x(cnref); }
//     { lf::Call<NonCopy&&, decltype(&clref)> x(clref); }
//     { lf::Call<NonCopy&&, decltype(&crref)> x(crref); }
//
//     // { lf::Call<const NonCopy, decltype(&nref)> x(nref); }
//     // { lf::Call<const NonCopy, decltype(&lref)> x(lref); }
//     // { lf::Call<const NonCopy, decltype(&rref)> x(rref); }
//     // { lf::Call<const NonCopy, decltype(&cnref)> x(cnref); }
//     { lf::Call<const NonCopy, decltype(&clref)> x(clref); }
//     { lf::Call<const NonCopy, decltype(&crref)> x(crref); }
//
//     // { lf::Call<const NonCopy&, decltype(&nref)> x(nref); }
//     // { lf::Call<const NonCopy&, decltype(&lref)> x(lref); }
//     // { lf::Call<const NonCopy&, decltype(&rref)> x(rref); }
//     // { lf::Call<const NonCopy&, decltype(&cnref)> x(cnref); }
//     { lf::Call<const NonCopy&, decltype(&clref)> x(clref); }
//     // { lf::Call<const NonCopy&, decltype(&crref)> x(crref); }
//
//     // { lf::Call<const NonCopy&&, decltype(&nref)> x(nref); }
//     // { lf::Call<const NonCopy&&, decltype(&lref)> x(lref); }
//     // { lf::Call<const NonCopy&&, decltype(&rref)> x(rref); }
//     // { lf::Call<const NonCopy&&, decltype(&cnref)> x(cnref); }
//     { lf::Call<const NonCopy&&, decltype(&clref)> x(clref); }
//     { lf::Call<const NonCopy&&, decltype(&crref)> x(crref); }
//   }
// }
```
