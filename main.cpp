#include <iostream>
#include <string>


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


int main(int argc, char const *argv[])
{
    #if 1
    auto [age, name] = bind::getStruct();
    std::cout << age<< " "<< name << std::endl;
    #endif

    return 0;
}
