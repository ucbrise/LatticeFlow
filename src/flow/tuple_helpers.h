#ifndef FLOW_TUPLE_HELPERS_H_
#define FLOW_TUPLE_HELPERS_H_

#include <iostream>
#include <tuple>
#include <type_traits>

namespace latticeflow {

template <size_t N, typename... Ts>
typename std::enable_if<N == sizeof...(Ts)-1>::type print_tuple(
    std::ostream& stream, const std::tuple<Ts...>& t) {
  stream << std::get<N>(t);
}

template <size_t N, typename... Ts>
    typename std::enable_if <
    N<sizeof...(Ts)-1>::type print_tuple(std::ostream& stream,
                                         const std::tuple<Ts...>& t) {
  stream << std::get<N>(t) << ", ";
  print_tuple<N + 1>(stream, t);
}

template <typename... Ts>
std::ostream& operator<<(std::ostream& stream, const std::tuple<Ts...>& t) {
  stream << "(";
  print_tuple<0>(stream, t);
  stream << ")";
  return stream;
}

}  // namespace latticeflow

#endif  // FLOW_TUPLE_HELPERS_H_
