`libpipe`
--

... is a small C++ library implementing the pipe operator syntax. You can use
it with any callables you want - functions, lambdas and classes implementing the
`operator()` are all fine.


State
==
Unfinished. Works so far with callables which aren't overloaded in their
argument types, i.e. those taking `auto` or templated parameters as arguments
do not work. Also, the `constexpr` support isn't fully there yet.


How to build etc.
==
You will need a C++17 capable compiler. Other than that, `libpipe` has no
dependencies outside the STL and is header only. An aggregate header is
provided.

The first step after cloning is then probably to build and run the tests:

```bash
mkdir build && cd build
cmake ..
make
./main
```

Most tests are done via `static_assert`, so if it builds, that's already a big
win. Some tests are just `assert`s, so if you get no output from running,
you're good to go.

Next, you'll want to integrate `libpipe` into your project. In order to do
this, you have to add the `include` directory to your include path (CMake let's
you do this per target via `target_include_directories (target_name
include_directories)`, in `Make` you'd just set the `-i` compiler flag, what
other build systems do I don't know).

Then just do

```c++
#include "libpipe.h"
```

and you're good to go. `make_pipeable` and `operator|` will be at your
disposal. See usage below.


Usage
==
There's a few things that work and a few things that don't. Be advised, and
don't blame me for the compiler errors. Sorry though.

### What it works with
Anything that can be called with one or more arguments, that is:

- Functions
- Lambdas
- Classes implementing `operator()`

Of course, functions which take no arguments make little sense, but there is
another restriction: the types of the arguments must be known at compile time,
so template functions do not work and lambdas which take `auto` arguments also
don't. Furthermore, while lambdas _can_ be capturing (as in: `[=]() {}`, where
the `[=]` captures all before values by copy), but you should be careful with
that: the capturing happens _when the lambda is first instantiated, not when
the pipeable object is made or used_. Safe usage is shown below.

### By example
To give you an idea how it is used, what works and what doesn't:

```c++
//
// functions and lambdas must have known argument types, no templates or auto.
//
constexpr int add_fn(int x, int s) {
  return x + s;
}

// sidenote: lambdas are constexpr by default, in case you didn't know - neato!
auto divide_fn = [](int x, int d) { return x / d; };

//
// classes implementing operator() can be templates themselves (so the argument
// type is encapsulated in the object type later), but must not have
// template/auto arguments either.
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
// callables are set up to be pipeable in the following way. you can of course
// build this into the type via `operator pipeable<decltype(*this)>` or
// something like that, i haven't actually tried.
//
constexpr auto add       = make_pipeable(add_fn);
constexpr auto divide    = make_pipeable(divide_fn);
constexpr auto increment = make_pipeable(increment_fn{});
constexpr auto multiply  = make_pipeable(multiply_fn<int>{});
constexpr auto modulo    = make_pipeable(modulo_fn{});

// pipe-evaluating a unary function with an argument
const auto fortytwo = 41 | increment;
static_assert(fortytwo == 42);

// pipe-evaluating a non-unary function with an argument and a parameter
const auto alsofortytwo = 21 | multiply(2);
static_assert(alsofortytwo == 42);

// chaining pipeables
const auto anotherfortytwo =
  19 | increment | multiply(4) | add(4) | divide(2);
static_assert(anotherfortytwo == 42);

const auto againfortytwo = 21 | add(21);
static_assert(againfortytwo == 42);

//
// composing pipes to receive a new pipeable object (which can of course also
// be used as a function). this does not work at compile time yet - i'm working
// on it.
//
auto alwaysfortytwo =
  multiply(10) | add(4) | modulo(10) | multiply(10) | increment | increment;

assert(alwaysfortytwo(1983447) == 42);
```

As mentioned above, lambdas _can be capturing_, but you must be careful with
it. Typically, avoid:

```c++
auto some_lambda = [&m](int x) noexcept { return m * x; };
// ... a bunch of code
auto some_lambda_pipeable = make_pipeable(some_lambda);
```

Remember: the capture happens _when you created the lambda_, which means that
things can already have gone out of scope by the time you're trying to use them
in your pipeable. It's not like this is not the case with lambdas anyway, I am
just mentioning it because `libpipe` creates a smoke screen around the lambdas
and the compiler errors you currently get are less than beautiful to work with
as it stands.

Safe uses for capturing lambdas are:

```c++
// capture by value
auto some_lambda = [=m](int x) noexcept { return m * x; };

// using a function to generate a pipeable object
auto multiply(int m) noexcept {
  return make_pipeable([m](int x) noexcept { return m * x; });
}
```

So much for the things that _do work_. The following _wouldn't work_:

```c++
// auto argument must be deduced, compiler sees some_lambda as overloaded
auto some_lambda = [](auto x) { return x * x; }

// functor has templated call arguments, compiler sees it as overloaded
struct some_func
{
  template <typename T>
  constexpr auto operator(T t) { return t * t; }
};

// functor overloads operator()
struct some_other_func
{
  constexpr auto operator(int i) { return i * i; }
  constexpr auto operator(float i) { return i * i; }
};
```

The _return type can be `auto`, though_. There's no harm in that. The
arguments, however, _must be known at compile time_, which means that the
function _must not be overloaded (or seen as overloaded due to type deduction
by the compiler)_.

