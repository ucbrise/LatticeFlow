#include <iostream>

#include "click/call.h"
#include "click/drop.h"

#define __PF__ __PRETTY_FUNCTION__

namespace lf = latticeflow;

template <typename T>
void print_type() {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template <typename T>
void print_type(T) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template <template <typename> class T>
void instantiate_all() {
  { T<int> d; }
  { T<int&> d; }
  { T<int&&> d; }
  { T<int*> d; }
  { T<int*&> d; }
  { T<int*&&> d; }
  { T<const int> d; }
  { T<const int&> d; }
  { T<const int&&> d; }
  { T<const int*> d; }
  { T<const int*&> d; }
  { T<const int*&&> d; }
}

class Copy {
 public:
  Copy() {}
  Copy(const Copy&) = default;
  Copy(Copy&&) = default;
};

class NonCopy {
 public:
  NonCopy() {}
  NonCopy(const NonCopy&) = delete;
  NonCopy(NonCopy&&) = default;
};

namespace copyable {
void nref(Copy) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
void lref(Copy&) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
void rref(Copy&&) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
void cnref(const Copy) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
void clref(const Copy&) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
void crref(const Copy&&) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
}  // namespace copyable

namespace noncopyable {
void nref(NonCopy) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
void lref(NonCopy&) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
void rref(NonCopy&&) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
void cnref(const NonCopy) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
void clref(const NonCopy&) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
void crref(const NonCopy&&) { std::cout << __PRETTY_FUNCTION__ << std::endl; };
}  // namespace noncopyable

void call() {
  {
    using namespace copyable;
    { lf::Call<Copy, decltype(&nref)> x(nref); }
    // { lf::Call<Copy, decltype(&lref)> x(lref); }
    { lf::Call<Copy, decltype(&rref)> x(rref); }
    { lf::Call<Copy, decltype(&cnref)> x(cnref); }
    { lf::Call<Copy, decltype(&clref)> x(clref); }
    { lf::Call<Copy, decltype(&crref)> x(crref); }

    { lf::Call<Copy&, decltype(&nref)> x(nref); }
    { lf::Call<Copy&, decltype(&lref)> x(lref); }
    // { lf::Call<Copy&, decltype(&rref)> x(rref); }
    { lf::Call<Copy&, decltype(&cnref)> x(cnref); }
    { lf::Call<Copy&, decltype(&clref)> x(clref); }
    // { lf::Call<Copy&, decltype(&crref)> x(crref); }

    { lf::Call<Copy&&, decltype(&nref)> x(nref); }
    // { lf::Call<Copy&&, decltype(&lref)> x(lref); }
    { lf::Call<Copy&&, decltype(&rref)> x(rref); }
    { lf::Call<Copy&&, decltype(&cnref)> x(cnref); }
    { lf::Call<Copy&&, decltype(&clref)> x(clref); }
    { lf::Call<Copy&&, decltype(&crref)> x(crref); }

    { lf::Call<const Copy, decltype(&nref)> x(nref); }
    // { lf::Call<const Copy, decltype(&lref)> x(lref); }
    // { lf::Call<const Copy, decltype(&rref)> x(rref); }
    { lf::Call<const Copy, decltype(&cnref)> x(cnref); }
    { lf::Call<const Copy, decltype(&clref)> x(clref); }
    { lf::Call<const Copy, decltype(&crref)> x(crref); }

    { lf::Call<const Copy&, decltype(&nref)> x(nref); }
    // { lf::Call<const Copy&, decltype(&lref)> x(lref); }
    // { lf::Call<const Copy&, decltype(&rref)> x(rref); }
    { lf::Call<const Copy&, decltype(&cnref)> x(cnref); }
    { lf::Call<const Copy&, decltype(&clref)> x(clref); }
    // { lf::Call<const Copy&, decltype(&crref)> x(crref); }

    { lf::Call<const Copy&&, decltype(&nref)> x(nref); }
    // { lf::Call<const Copy&&, decltype(&lref)> x(lref); }
    // { lf::Call<const Copy&&, decltype(&rref)> x(rref); }
    { lf::Call<const Copy&&, decltype(&cnref)> x(cnref); }
    { lf::Call<const Copy&&, decltype(&clref)> x(clref); }
    { lf::Call<const Copy&&, decltype(&crref)> x(crref); }
  }

  {
    using namespace noncopyable;
    { lf::Call<NonCopy, decltype(&nref)> x(nref); }
    // { lf::Call<NonCopy, decltype(&lref)> x(lref); }
    { lf::Call<NonCopy, decltype(&rref)> x(rref); }
    { lf::Call<NonCopy, decltype(&cnref)> x(cnref); }
    { lf::Call<NonCopy, decltype(&clref)> x(clref); }
    { lf::Call<NonCopy, decltype(&crref)> x(crref); }

    // { lf::Call<NonCopy&, decltype(&nref)> x(nref); }
    { lf::Call<NonCopy&, decltype(&lref)> x(lref); }
    // { lf::Call<NonCopy&, decltype(&rref)> x(rref); }
    // { lf::Call<NonCopy&, decltype(&cnref)> x(cnref); }
    { lf::Call<NonCopy&, decltype(&clref)> x(clref); }
    // { lf::Call<NonCopy&, decltype(&crref)> x(crref); }

    { lf::Call<NonCopy&&, decltype(&nref)> x(nref); }
    // { lf::Call<NonCopy&&, decltype(&lref)> x(lref); }
    { lf::Call<NonCopy&&, decltype(&rref)> x(rref); }
    { lf::Call<NonCopy&&, decltype(&cnref)> x(cnref); }
    { lf::Call<NonCopy&&, decltype(&clref)> x(clref); }
    { lf::Call<NonCopy&&, decltype(&crref)> x(crref); }

    // { lf::Call<const NonCopy, decltype(&nref)> x(nref); }
    // { lf::Call<const NonCopy, decltype(&lref)> x(lref); }
    // { lf::Call<const NonCopy, decltype(&rref)> x(rref); }
    // { lf::Call<const NonCopy, decltype(&cnref)> x(cnref); }
    { lf::Call<const NonCopy, decltype(&clref)> x(clref); }
    { lf::Call<const NonCopy, decltype(&crref)> x(crref); }

    // { lf::Call<const NonCopy&, decltype(&nref)> x(nref); }
    // { lf::Call<const NonCopy&, decltype(&lref)> x(lref); }
    // { lf::Call<const NonCopy&, decltype(&rref)> x(rref); }
    // { lf::Call<const NonCopy&, decltype(&cnref)> x(cnref); }
    { lf::Call<const NonCopy&, decltype(&clref)> x(clref); }
    // { lf::Call<const NonCopy&, decltype(&crref)> x(crref); }

    // { lf::Call<const NonCopy&&, decltype(&nref)> x(nref); }
    // { lf::Call<const NonCopy&&, decltype(&lref)> x(lref); }
    // { lf::Call<const NonCopy&&, decltype(&rref)> x(rref); }
    // { lf::Call<const NonCopy&&, decltype(&cnref)> x(cnref); }
    { lf::Call<const NonCopy&&, decltype(&clref)> x(clref); }
    { lf::Call<const NonCopy&&, decltype(&crref)> x(crref); }
  }
}

int main() {
  instantiate_all<lf::Drop>();
  call();
}
