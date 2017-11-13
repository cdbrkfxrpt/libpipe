//
// Copyright 2017- Florian Eich <florian.eich@gmail.com>
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <cassert>
#include <type_traits>
#include <utility>

#include "libpipe.h"

//
// functions and lambdas for testing arity and defines
//
const char * test_function(int i, const char * n) {
  return n + i;
}

const char * test_function_noexcept(int i, const char * n) noexcept {
  return n + i;
}

auto test_lambda          = []() {};
auto test_lambda_noexcept = []() noexcept {};

//
// functors for testing arity and pipes
//
struct increment_fn
{
  constexpr auto operator()(int t) const noexcept { return t + 1; }
};

template <typename T, REQUIRES(std::is_arithmetic<T>())>
struct multiply_fn
{
  constexpr auto operator()(T t, T f) const noexcept { return t * f; }
};

struct modulo_fn
{
  constexpr auto operator()(int t, int d) const noexcept { return t % d; }
};
//
// ------------------------------------------------------------------------- *
//
int main() {
  //
  // first_arg tests
  //
  static_assert(std::is_same_v<pipe::first_arg_t<increment_fn>, int>);
  static_assert(std::is_same_v<pipe::first_arg_t<multiply_fn<float>>, float>);

  //
  // arity tests - not working currently
  //
  // static_assert(pipe::arity(test_function) == 2);
  // static_assert(pipe::arity(test_function_noexcept) == 2);

  // static_assert(pipe::arity(test_lambda) == 0);
  // static_assert(pipe::arity(test_lambda_noexcept) == 0);

  // static_assert(pipe::arity([](int x) { return x; }) == 1);
  // static_assert(pipe::arity([](int x) noexcept { return x; }) == 1);

  // static_assert(pipe::arity(increment_fn{}) == 1);
  // static_assert(pipe::arity(multiply_fn<int>{}) == 2);
  // static_assert(pipe::arity(modulo_fn{}) == 2);

  //
  // pipes tests
  //
  constexpr auto increment = make_pipeable(increment_fn{});
  constexpr auto multiply  = make_pipeable(multiply_fn<int>{});
  constexpr auto modulo    = make_pipeable(modulo_fn{});

  constexpr auto add = make_pipeable([](int v, int i) { return v + i; });

  const auto fortytwo = 41 | increment;
  static_assert(fortytwo == 42);

  const auto alsofortytwo = 21 | multiply(2);
  static_assert(alsofortytwo == 42);

  const auto anotherfortytwo =
    19 | increment | multiply(2) | increment | increment;
  static_assert(anotherfortytwo == 42);

  const auto againfortytwo = 21 | add(21);
  static_assert(againfortytwo == 42);

  //
  // the big cahoona - not working constexpr yet
  //
  auto alwaysfortytwo = multiply(10)
                      | add(4)
                      | modulo(10)
                      | multiply(10)
                      | increment
                      | increment;

  assert(alwaysfortytwo(1983447) == 42);

  // constexpr auto alwaysfortytwo = multiply(10)
  //                               | add(4)
  //                               | modulo(10)
  //                               | multiply(10)
  //                               | increment
  //                               | increment;

  // static_assert(alwaysfortytwo(1983447) == 42);

  return 0;
}

