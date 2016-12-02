# Flow
This directory includes a collection of relational iterators: iterators which
iterate over tuples. All iterators implement the [`Operator`
interface](operator.h). Some are implemented using
[coroutines](coroutine_operator.h). We also include a [naive exchange
operator](exchange.h) to parallelize a dataflow. The code also uses C++
variadic templates to allow for generic iteration of tuples.
