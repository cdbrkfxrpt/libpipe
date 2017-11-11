
#include <type_traits>
#include <utility>

#define REQUIRES(...) std::enable_if_t<(__VA_ARGS__)>...

//
// struct wraps the arguments to the functor
//
template <typename T>
struct argument_wrapper
{
  T value;

  constexpr argument_wrapper(T && t) : value(std::forward<T>(t)) {}

  constexpr T get() const noexcept { return value; }
};

template <typename T>
constexpr argument_wrapper<T> make_argument_wrapper(T && t) noexcept {
  return {std::forward<T>(t)};
}

//
// struct wraps the functor with its arguments
//
template <typename F>
struct pipeable_binder : F
{
  template <typename... Ts>
  constexpr pipeable_binder(Ts &&... ts) : F(std::forward<Ts>(ts)...) {}
};

template <typename F>
constexpr pipeable_binder<F> make_pipeable_binder(F f) noexcept {
  return {std::move(f)};
}

//
// composition struct
//
// template <typename F, typename G>
// struct pipeable_compositor : F, G
// {
//   template <typename... Ts>
//   constexpr pipeable_compositor(Ts &&... ts) : F(G(std::forward<Ts>(ts)...))
//   {}
// };

// template <typename F, typename G>
// constexpr pipeable_compositor<F, G> make_pipeable_compositor(F f,
//                                                              G g) noexcept {
//   return {std::move(f), std::move(g)};
// }

//
// pipeable struct and generator
//
template <typename F>
struct pipeable
{
  template <typename... Args>
  constexpr auto operator()(Args &&... args) const noexcept {
    return make_pipeable_binder([](auto... xs) {
      return [=](auto && x) -> decltype(auto) { return F()(x, xs.get()...); };
    }(make_argument_wrapper(std::forward<Args>(args)...)));
  }
};

template <typename F>
constexpr pipeable<F> make_pipeable() noexcept {
  return {};
}

//
// operator| overloads
//
template <typename T, typename F>
constexpr auto operator|(T && t, const pipeable<F> &) {
  return F()(std::forward<T>(t));
}

template <typename T, typename F>
constexpr auto operator|(T && t, const pipeable_binder<F> & p) {
  return p(std::forward<T>(t));
}

template <typename T, typename F, typename G>
constexpr auto operator|(const pipeable<G> &, const pipeable<F> &) {
  return make_pipeable_binder([](T && t) { return F(G(std::forward<T>(t))); });
}

template <typename... Ts, typename F, typename G>
constexpr auto operator|(const pipeable_binder<G> & g,
                         const pipeable_binder<F> & f) {
  return make_pipeable_binder(
    [&](Ts &&... ts) { return f(g(std::forward<Ts>(ts)...)); });
}

//
// functors
//
struct increment_f
{
  template <typename T>
  constexpr auto operator()(T value) const noexcept {
    return ++value;
  }
};

struct add_f
{
  template <typename T>
  constexpr auto operator()(T value, T increment) const noexcept {
    return value + increment;
  }
};

struct multiply_f
{
  template <typename T>
  constexpr auto operator()(T value, T factor) const noexcept {
    return value * factor;
  }
};

struct divide_f
{
  template <typename T>
  constexpr auto operator()(T value, T divisor) const noexcept {
    return value / divisor;
  }
};

struct modulo_f
{
  template <typename T>
  constexpr auto operator()(T value, T divisor) const noexcept {
    return value % divisor;
  }
};

//
// create pipeables from functors and make them global.
// it would probably be better to have them be transformable to pipeables.
//
auto increment = make_pipeable<increment_f>();
auto add       = make_pipeable<add_f>();
auto multiply  = make_pipeable<multiply_f>();
auto divide    = make_pipeable<divide_f>();
auto modulo    = make_pipeable<modulo_f>();

/* ------------------------------------------------------------------------- *
 * main
 * ------------------------------------------------------------------------- */
int main() {
  const auto fortytwo = 40 | increment | increment;
  static_assert(fortytwo == 42, "unary functor pipes not working.");

  const auto alsofortytwo = 19 | increment | multiply(2) | add(2);
  static_assert(alsofortytwo == 42, "functor pipes not working.");

  const auto alwaysfortytwo = modulo(10) | multiply(4) | increment | increment;
  static_assert(alwaysfortytwo(1983447) == 42, "pipe composition not working");

  return 0;
}

