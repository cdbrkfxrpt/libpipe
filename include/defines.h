//
// copyright 2017- florian eich <florian.eich@gmail.com>
//
// licensed under the apache license, version 2.0 (the "license");
// you may not use this file except in compliance with the license.
// you may obtain a copy of the license at
//
//    http://www.apache.org/licenses/license-2.0
//
// unless required by applicable law or agreed to in writing, software
// distributed under the license is distributed on an "as is" basis,
// without warranties or conditions of any kind, either express or implied.
// see the license for the specific language governing permissions and
// limitations under the license.
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

