#include <iostream>


int main()
{
    constexpr auto ch = [](auto fn, const auto&& ...item) { return (fn(item...)); };

    static_assert(ch(std::multiplies{}, 2, 5)==10);
    static_assert(ch(std::plus{}, 2, 5)==7);
    static_assert(ch(std::minus{}, 43, 1)==42);

    std::cout << "\nend";

    return 0;
}
