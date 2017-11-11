
#include <cassert>
#include <type_traits>
#include <utility>

#define REQUIRES(...) class = std::enable_if_t<(__VA_ARGS__)>

/* ------------------------------------------------------------------------- *
 * wrapper struct and generator
 * ------------------------------------------------------------------------- */
template <class T>
struct wrapper
{
  T value;

  template <class X, REQUIRES(std::is_convertible<T, X>())>
  constexpr wrapper(X && x) : value(std::forward<X>(x)) {}

  constexpr T get() const { return std::move(value); }
};


template <class T>
constexpr wrapper<T> make_wrapper(T && x) {
  return {std::forward<T>(x)};
}

/* ------------------------------------------------------------------------- *
 * pipe_closure struct and generator
 * ------------------------------------------------------------------------- */
template <class F>
struct pipe_closure : F
{
  template <class... Xs>
  constexpr pipe_closure(Xs &&... xs) : F(std::forward<Xs>(xs)...) {}

  constexpr auto operator()() {}
};

template <class F>
constexpr pipe_closure<F> make_pipe_closure(F f) {
  return {std::move(f)};
}

/* ------------------------------------------------------------------------- *
 * pipeable struct
 * ------------------------------------------------------------------------- */
template <class F>
struct pipeable
{
  template <class... Ts>
  constexpr auto operator()(Ts &&... xs) const {
    return make_pipe_closure([](auto... ws) {
      return [=](auto && x) -> decltype(auto) { return F()(x, ws.get()...); };
    }(make_wrapper(std::forward<Ts>(xs)...)));
  }
};

template <class F>
constexpr pipeable<F> make_pipeable() {
  return {};
}

/* ------------------------------------------------------------------------- *
 * operator| overloads
 * ------------------------------------------------------------------------- */
//
// functions with several arguments
//
template <class T, class F>
constexpr decltype(auto) operator|(T && x, const pipe_closure<F> & p) {
  return p(std::forward<T>(x));
}

//
// unary functions
//
template <class T, class F>
constexpr decltype(auto) operator|(T && x, const pipeable<F> &) {
  return F()(std::forward<T>(x));
}


/* ------------------------------------------------------------------------- *
 * example functors
 * ------------------------------------------------------------------------- */
struct add_one_f
{
  template <class T>
  constexpr auto operator()(T x) const {
    return x + 1;
  }
};

struct product_f
{
  template <class T>
  constexpr auto operator()(T x, T y) const {
    return x * y;
  }
};


/* ------------------------------------------------------------------------- *
 * main
 * ------------------------------------------------------------------------- */
int main() {
  auto add_one = make_pipeable<add_one_f>();
  auto product = make_pipeable<product_f>();

  constexpr int res1 = 40 | add_one | add_one;
  static_assert(res1 == 42, "unary functor pipes not working.");

  constexpr int res2 = 21 | product(2);
  static_assert(res2 == 42, "functor pipes not working.");

  constexpr int res3 = 19 | add_one | product(2) | add_one | add_one;
  static_assert(res3 == 42, "functor pipes not working.");

  int val = 9;
  assert((val | add_one | product(4) | add_one | add_one) == 42);

  // constexpr auto f = product(2) | add_one;
  // static_assert(f(21) == 42, "pipe composition not working");

  return 0;
}

