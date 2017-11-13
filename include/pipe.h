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

#ifndef LIBPIPE_INCLUDE_PIPE_H_
#define LIBPIPE_INCLUDE_PIPE_H_

#include <type_traits>

#include "arity.h"
#include "defines.h"
#include "first_arg.h"


namespace pipe {


struct pipeable_base
{};

template <typename T>
struct is_pipeable : std::is_base_of<pipeable_base, T>
{};

template <typename T>
struct is_pipeable<T &> : is_pipeable<T>
{};

template <typename T>
inline constexpr bool is_pipeable_v = is_pipeable<T>::value;


template <typename T>
struct argument
{
  constexpr argument(T && t) noexcept : value(std::forward<T>(t)) {}

  T value;
};

template <typename T>
constexpr auto make_argument(T && t) noexcept {
  return argument<T>(std::forward<T>(t));
}


template <typename F>
struct pipeable : F, pipeable_base
{
  template <typename X>
  constexpr pipeable(X && x) noexcept : F(std::forward<X>(x)) {}
};

template <typename F, IS_UNARY_FN(F)>
constexpr auto make_pipeable(F f) noexcept {
  return pipeable<F>{std::move(f)};
}


template <typename F>
struct pipeable_generator
{
  constexpr pipeable_generator(F f) noexcept : _f(std::move(f)) {}

  template <typename... Xs>
  constexpr decltype(auto) operator()(Xs &&... xs) const noexcept {
    using X = first_arg_t<F>;

    return make_pipeable([&](auto... args) noexcept {
      return [=](X && x) -> decltype(auto) { return _f(x, args.value...); };
    }(make_argument(std::forward<Xs>(xs)...)));
  }

private:
   F _f;
};

template <typename F, IS_NON_UNARY_FN(F)>
constexpr auto make_pipeable(F f) noexcept {
  return pipeable_generator<F>{std::move(f)};
}

}  // namespace pipe


using namespace pipe;

//
// evaluation overload of the pipe operator
//
template <typename X, typename F, IS_NOT_PIPEABLE(X)>
constexpr decltype(auto) operator|(X && x, const pipeable<F> & f) noexcept {
  return f(std::forward<X>(x));
}

//
// composition overload of the pipe operator
//
template <typename G, typename F, IS_PIPEABLE(G)>
constexpr decltype(auto) operator|(const G &           g,
                                   const pipeable<F> & f) noexcept {
  using X = pipe::first_arg_t<G>;

  return make_pipeable([=](X && x) noexcept {
    return g(std::forward<X>(x)) | f;
  });
}

#endif  // LIBPIPE_INCLUDE_PIPE_H_

