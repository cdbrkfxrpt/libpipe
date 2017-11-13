`libpipe`
--

... is a small C++ library implementing the pipe operator syntax. You can use
it with any functors you want - functions, lambdas and classes implementing the
`operator()` are all fine.


State
==

Unfinished. Works so far with non-capturing lambdas and if you don't want to
functors which are overloaded, such as those taking `auto` or templated
parameters as arguments. Also, the `constexpr` support isn't fully there yet.

