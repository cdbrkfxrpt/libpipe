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

#ifndef LIBPIPE_INCLUDE_DEFINES_H_
#define LIBPIPE_INCLUDE_DEFINES_H_

#define IS_PLAIN_FN(T) REQUIRES(pipe::arity<T>() == 0)
#define IS_UNARY_FN(T) REQUIRES(pipe::arity<T>() == 1)
#define IS_NON_UNARY_FN(T) REQUIRES(pipe::arity<T>() > 1)

#define IS_PIPEABLE(T) REQUIRES(pipe::is_pipeable_v<T>)
#define IS_NOT_PIPEABLE(T) REQUIRES(!pipe::is_pipeable_v<T>)

#define REQUIRES(...) std::enable_if_t<(__VA_ARGS__)>...

#endif  // LIBPIPE_INCLUDE_DEFINES_H_

