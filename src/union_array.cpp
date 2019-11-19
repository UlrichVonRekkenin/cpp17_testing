#include <array>
#include <experimental/array>
#include <type_traits>
#include <cassert>


namespace impl {

template<typename T, std::size_t Size = sizeof(T)/sizeof(uint16_t) ? sizeof(T)/sizeof(uint16_t) : 1>
union Converter {
  T value;
  std::array<uint16_t, Size> arr;

  explicit Converter(T value)
          :value(value) { }

  template<std::size_t N>
  explicit Converter(std::array<uint16_t, N>&& array)
          : arr(std::move(array))
  {
      static_assert(sizeof(array.size())==Size);
  }
};

template<typename T> Converter(T) -> Converter<T>;

}


int main()
{
    using namespace impl;

    assert(Converter{uint8_t{42}}.arr.size()==1);
    assert(Converter{uint16_t{42}}.arr.size()==1);
    assert(Converter{uint32_t{42}}.arr.size()==2);
    assert(Converter{uint64_t{42}}.arr.size()==4);
    assert(Converter{42.00f}.arr.size()==2);
    assert(Converter{42.00}.arr.size()==4);

//    auto a = std::experimental::make_array(1, 2, 3, 4);
//    assert(Converter<double>(a).arr.size()==4);
}