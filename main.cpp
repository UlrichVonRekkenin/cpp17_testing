#include <iostream>
#include <string>
#include <cassert>
#include <type_traits>


namespace meta {
    template<typename A, typename B>
    struct is_same : std::false_type {};

    template<typename A>
    struct is_same<A, A> : std::true_type {};

    template<typename T>
    struct just_type { using type = T; };

    template<class T, class U>
    constexpr bool operator==(just_type<T>, just_type<U>)
    {
        return false;
    }

    template<class T>
    constexpr bool operator==(just_type<T>, just_type<T>)
    {
        return true;
    }

    template<typename... Ts>
    struct type_pack {};
    using empty_pack = type_pack<>;

    template<class... Ts>
    constexpr size_t size(type_pack<Ts...>)
    {
        return sizeof...(Ts);
    }

    template<class... Ts, class... Us>
    constexpr bool operator==(type_pack<Ts...>, type_pack<Us...>)
    {
        return false;
    }

    template<class... Ts>
    constexpr bool operator==(type_pack<Ts...>, type_pack<Ts...>)
    {
        return true;
    }

    template<class... Ts, class... Us>
    constexpr bool operator!=(type_pack<Ts...>, type_pack<Us...>)
    {
        return true;
    }

    template<class... Ts>
    constexpr bool operator!=(type_pack<Ts...>, type_pack<Ts...>)
    {
        return false;
    }

    template<class T, class... Ts>
    constexpr just_type<T> head(type_pack<T, Ts...>) 
    {
        return {};
    }

    template<class T, class... Ts>
    constexpr type_pack<Ts...> tail(type_pack<T, Ts...>)
    {
        return {};
    }

    template<class... Ts, class T>
    constexpr type_pack<T, Ts...> push_front(type_pack<Ts...>, just_type<T>)
    {
        return {};
    }

    template<class T, class... Ts>
    constexpr type_pack<T, Ts...> push_front(type_pack<Ts...>)
    {
        return {};
    }

    template<class... Ts, class T>
    constexpr type_pack<Ts..., T> push_back(type_pack<Ts...>, just_type<T>)
    {
        return {};
    }

    template<class T, class... Ts>
    constexpr type_pack<Ts..., T> push_back(type_pack<Ts...>)
    {
        return {};
    }

    template<class T, class... Ts>
    constexpr bool contains_bad(type_pack<Ts...>)
    {
        bool bs[] = {std::is_same<T, Ts>::value...};
        bool res = {false};

        for (const auto b: bs){
            res |= b;
        }

        return res;
    }

    template<class T, class... Ts>
    constexpr bool contains(type_pack<Ts...>)
    {
        return (... || std::is_same_v<T, Ts>);
    }

    template<class T, class... Ts>
    constexpr size_t find(type_pack<Ts...> tp)
    {
        bool bs[] = {std::is_same_v<T, Ts>...};

        for (size_t i = 0; i < size(tp); ++i)
        {
            if (bs[i])  
            {
                return i;
            }
        }
    return size(tp);
    }
}

namespace bind {
    struct MyStruct
    {
        int age;
        std::string name;
    };
    
    MyStruct getStruct(){
        return {42, "foobar"};
    }
}


template<typename... T>
int folding(T... args){
    return (args + ... + 0);
}


int main(int argc, char const *argv[])
{
    #if 1
    using namespace meta;
    static_assert(is_same<int, int>::value);
    static_assert(!is_same<int, double>::value);

    constexpr auto type = just_type<int>{};
    using type_t = typename decltype(type)::type;
    static_assert(is_same<type_t, int>::value);

    using foo = type_pack<double, int, char*>; 
    std::cout << "sizeof = " << size(empty_pack{}) << "; " << size(foo{});
    static_assert(foo{} != empty_pack{});

    static_assert(head(foo{}) == just_type<double>{});
    static_assert(tail(foo{}) == type_pack<int, char*>{});

    static_assert(push_front<double>(type_pack<int, double>{}) == 
        type_pack<double, int, double>{});

    static_assert(push_back<double>(type_pack<int, double>{}) == 
        type_pack<int, double, double>{});

    static_assert(contains<int>(type_pack<double, int, char*>{}));
    static_assert(!contains<int>(empty_pack{}));

    static_assert(find<int>(type_pack<double, char*, int>{}) == 2);
    #endif

    #if 0
    auto [age, name] = bind::getStruct();
    std::cout << age<< " "<< name << std::endl;
    #endif

    #if 0
    std::cout << folding(40, 2) << std::endl
    << folding(39, 1, 2) << std::endl
    << folding();
    #endif

    return 0;
}
