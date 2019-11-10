#include <iostream>
#include <complex>


namespace deriving {

class Base {
  protected:
    int foo = 42;
};

template<typename T>
class Derived : public Base {
  public:
    explicit Derived(T t)
            :bar(t) { std::cout << "foo = " << foo << "; bar = " << bar << std::endl; }
    T bar;
};

template<typename T> Derived(T) -> Derived<T>;
}


int main()
{
    {
        using namespace deriving;
        Derived{"hello"};
        Derived{std::complex<int>{4, 5}};
    }
}