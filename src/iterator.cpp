#include <type_traits>
#include <vector>
#include <array>
#include <iostream>
#include <cassert>


namespace impl {

using t1 = std::vector<int>;
using t2 = std::array<int, 3>;

static t1 v{1, 2, 3, 4};
static t2 a{42, 43, 44};


template<typename It>
class FooIter {
    using val_type = typename It::value_type;
    It it;
  public:
    explicit FooIter(It it)
            :it(it) { }

    FooIter& operator++()
    {
        ++it;
        return *this;
    }

    bool operator==(const FooIter& rhs) const
    {
        return it==rhs.it;
    }
    bool operator!=(const FooIter& rhs) const
    {
        return !(operator==(rhs));
    }

    val_type operator*() const
    {
        return *it;
    }
};

template<typename Container>
class Foo {
    using it_type = typename Container::iterator;

    Container& c;
  public:
    explicit Foo(Container& c)
            :c(c) { }
    [[nodiscard]] constexpr std::size_t size() const { return c.size(); }
    [[nodiscard]] FooIter<it_type> begin() const { return std::begin(c); }
    [[nodiscard]] FooIter<it_type> end() const { return std::end(c); }
};

void iterateOver()
{
//    for (auto& e: Foo<t1>(v)) { std::cout << e << "; "; }
//    for (auto& e: Foo<t2>(a)) { std::cout << e << "; "; }
}

void testSize()
{
    assert((Foo<t1>{v}).size()==4);
    assert((Foo<t2>{a}).size()==3);
}

}

int main()
{
    impl::testSize();
    impl::iterateOver();
}