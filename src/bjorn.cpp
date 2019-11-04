#include <cassert>
#include <tuple>


namespace tmp {

template<typename T>
auto is_in(const T& t)
{
    return [t](const auto& ... vs) { return ((t==vs) || ...); };
}

template<typename T>
struct is {
  T t;
  template<typename ...Ts>
  bool any_of(const Ts& ...ts) const { return ((t==ts) || ...); }
};
template<typename T> is(T) -> is<T>;

struct or_element {
  template<typename F, typename ...Ts>
  static bool apply(const F& f, const std::tuple<Ts...>& t)
  {
      return std::apply([&](const auto& ...ts) { return (f(ts) || ...); }, t);
  }
};

struct and_element {
  template<typename F, typename ...Ts>
  static bool apply(const F& f, const std::tuple<Ts...>& t)
  {
      return std::apply([&](const auto& ...ts) { return (f(ts) && ...); }, t);
  }
};

template<typename Op, typename ...Ts>
struct op_t : private std::tuple<Ts...> {
  using std::tuple<Ts...>::tuple;
  template<typename T>
  bool operator==(const T& t) const
  {
      return Op::apply([&t](const auto& v) { return v==t; }, *this);
  }
};


template<typename ...Ts>
struct any_of : op_t<or_element, Ts...> { using op_t<or_element, Ts...>::op_t; };
template<typename ...Ts> any_of(Ts...) -> any_of<Ts...>;

template<typename ...Ts>
struct each_of : op_t<and_element, Ts...> { using op_t<and_element, Ts...>::op_t; };
template<typename ...Ts> each_of(Ts...) -> each_of<Ts...>;

}


namespace lambda {

constexpr auto tuple = [](auto&& ... ts) { return [=](const auto& func) { return func(ts...); }; };

constexpr auto and_elements = [](auto&& func) {
  return [func = std::forward<decltype(func)>(func)](auto ...elements) {
    return (func(elements) && ...);
  };
};

constexpr auto bind_rh = [](auto func, auto rh) { return [=](auto lh) { return func(lh, rh); }; };
constexpr auto greater_than = [](auto rh) { return bind_rh(std::greater{}, rh); };
constexpr auto less_than = [](auto rh) { return bind_rh(std::less{}, rh); };
constexpr auto equal_to = [](auto rh) { return bind_rh(std::equal_to{}, rh); };


template<typename Func, typename Tuple>
class op_t {
    Tuple _tup;
    Func _func;

    template<typename F>
    [[nodiscard]] constexpr auto apply(F&& f) const { return _tup(_func(std::forward<F>(f))); }

  public:
    constexpr op_t(Func func, Tuple tup)
            :_tup(std::move(tup)), _func(std::move(func)) { }

    template<typename T>
    constexpr auto operator==(const T& t) const { return apply(equal_to(t)); }
    template<typename T>
    constexpr auto operator>(const T& t) const { return apply(greater_than(t)); }
    template<typename T>
    constexpr auto operator<(const T& t) const { return apply(less_than(t)); }
};

constexpr auto each_of = [](auto...ts) { return op_t(and_elements, tuple(ts...)); };
}

int main()
{
    {
        enum state_t { Idle, Connecting, Connected, Disconnecting, Disconnected };
        using namespace tmp;
        assert(is_in(Idle)(Idle, Disconnected));
        assert(!is_in(Idle)(Disconnected, Connecting));
        assert(is{Idle}.any_of(Idle, Connected, Disconnecting));

        assert(any_of(Idle, Disconnecting)==Disconnecting);
        assert(any_of(41, 42, 43)==42);
        assert(each_of(42, 42, 42)==42);
    }

    {
        using namespace lambda;
        static_assert(tuple(43, 44, 45)(and_elements(greater_than(42))));
        static_assert(each_of(43, 44, 45)>42);
        static_assert(each_of(43, 44, 45)<46);
        static_assert(each_of(42, 42, 42)==42);
    }
}
