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

#ifndef LIBPIPE_INCLUDE_ARITY_H_
#define LIBPIPE_INCLUDE_ARITY_H_

#include <type_traits>


namespace pipe {
namespace dt {

//
// base arity struct deferring to specializations
//
template <typename T>
struct arity_s : arity_s<decltype(&T::operator())>
{};

//
// overloads for non-member functions: base and noexcept
//
template <typename R, typename... Args>
struct arity_s<R (*)(Args...)>
  : std::integral_constant<unsigned, sizeof...(Args)>
{};

template <typename R, typename... Args>
struct arity_s<R (*)(Args...) noexcept>
  : std::integral_constant<unsigned, sizeof...(Args)>
{};

//
// overloads for member functions: base, const, noexcept and const noexcept
//
template <typename R, typename C, typename... Args>
struct arity_s<R (C::*)(Args...)>
  : std::integral_constant<unsigned, sizeof...(Args)>
{};

template <typename R, typename C, typename... Args>
struct arity_s<R (C::*)(Args...) const>
  : std::integral_constant<unsigned, sizeof...(Args)>
{};

template <typename R, typename C, typename... Args>
struct arity_s<R (C::*)(Args...) noexcept>
  : std::integral_constant<unsigned, sizeof...(Args)>
{};

template <typename R, typename C, typename... Args>
struct arity_s<R (C::*)(Args...) const noexcept>
  : std::integral_constant<unsigned, sizeof...(Args)>
{};

}  // namespace dt


/*!
 * \brief Function which determines the number of arguments of a callable
 *        object (function, functor, lambda).
 * \param F Only type of parameter is of interest for functionality.
 * \returns Number of arguments the function takes.
 *
 * **Be Aware**
 * `arity` DOES NOT WORK with overloaded functions (which includes functions
 * using template arguments).
 *
 * **Usage**
 * \include arity.cc
 */
template <typename F>
constexpr auto arity() noexcept {
  return decltype(dt::arity_s<F>{})::value;
}

}  // namespace lp

#endif  // LIBPIPE_INCLUDE_ARITY_H_

