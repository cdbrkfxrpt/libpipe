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

#ifndef LIBPIPE_INCLUDE_TYPE_RETRIEVAL_H_
#define LIBPIPE_INCLUDE_TYPE_RETRIEVAL_H_

#define STORE(T) std::enable_if<true, T>

#include <type_traits>


namespace pipe {
namespace dt {


//
// base first_arg struct deferring to specializations
//
template <typename T>
struct first_arg : first_arg<decltype(&T::operator())>
{};

//
// overloads for non-member functions: base and noexcept
//
template <typename R, typename Arg, typename... Args>
struct first_arg<R (*)(Arg, Args...)> : STORE(Arg)
{};

template <typename R, typename Arg, typename... Args>
struct first_arg<R (*)(Arg, Args...) noexcept> : STORE(Arg)
{};

//
// overloads for member functions: base, const, noexcept and const noexcept
//
template <typename R, typename C, typename Arg, typename... Args>
struct first_arg<R (C::*)(Arg, Args...)> : STORE(Arg)
{};

template <typename R, typename C, typename Arg, typename... Args>
struct first_arg<R (C::*)(Arg, Args...) const> : STORE(Arg)
{};

template <typename R, typename C, typename Arg, typename... Args>
struct first_arg<R (C::*)(Arg, Args...) noexcept> : STORE(Arg)
{};

template <typename R, typename C, typename Arg, typename... Args>
struct first_arg<R (C::*)(Arg, Args...) const noexcept> : STORE(Arg)
{};

}  // namespace dt


template <typename T>
using first_arg_t = typename dt::first_arg<T>::type;

}  // namespace pipe


#endif  // LIBPIPE_INCLUDE_TYPE_RETRIEVAL_H_

