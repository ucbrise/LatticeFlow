# LatticeFlow
[BOOM][boom-analytics-paper], [Bloom][bloom-paper], and [Bloom^L][blooml-paper]
explored how the conjunction of data-centric declarative programming and join
semilattices could make writing eventually consistent programs easier and more
principled. Building upon its academic ancestors, LatticeFlow explores how to
make lattice-based stream processing fast and friendly.

## Getting Started
First, install `clang`, `cmake`, and `protobuf` (See
[here][cpp_project_template] for details).

```bash
# Building Code.
./scripts/build_debug.sh   # build the code in debug mode
./scripts/build_release.sh # build the code in release mode

# Running Code.
./build/lattices/pair_lattice_test  # run a test
(cd build && make test)             # run all the tests
./build/flow/filter_bench           # run a benchmark
./build/key_value_stores/msgqueue   # start the message broker
./build/key_value_stores/server     # start the key-value server
./build/key_value_stores/ruc_client # start a key-value client

# Formatting and Linting Code.
clang-format src/key_value_stores/server.cc                  # format a file
find src -name '*.cc' -o -name '*.h' | xargs clang-format -i # format all files
./scripts/run_clang_tidy.sh src/key_value_stores/server.cc   # lint a file
./scripts/run_clang_tidy.sh $(find src -name '*.cc')         # lint all files
```

## Tour
| Directory                                            | Description                                    |
| ---------------------------------------------------- | ---------------------------------------------- |
| [`concurrency`](src/concurrency/README.md)           | Concurrency and synchronization primitives.    |
| [`flow`](src/flow/README.md)                         | Iterators and stream processors.               |
| [`key_value_stores`](src/key_value_stores/README.md) | Key value store with varying isolation levels. |
| [`lattices`](src/lattices/README.md)                 | A collection of join semilattices.             |

## Administrivia
- All code should abide by the [Google C++ Style
  Guide](https://google.github.io/styleguide/cppguide.html).
- All code should be formatted with `clang-format` and linted using
  `clang-tidy` (see [Getting Started](#getting-started)).
- Third party libraries should be used sparingly.
- TODO: Continuous Integration.

Refer also to the [RISE Lab C++ Project Template][cpp_project_template].

[boom-analytics-paper]: https://scholar.google.com/scholar?cluster=17621967364443794950&hl=en&as_sdt=0,5
[bloom-paper]: https://scholar.google.com/scholar?cluster=9165311711752272482&hl=en&as_sdt=0,5
[blooml-paper]: https://scholar.google.com/scholar?cluster=1332747912204910097&hl=en&as_sdt=0,5
[cpp_project_template]: https://github.com/ucbrise/cpp_project_template
