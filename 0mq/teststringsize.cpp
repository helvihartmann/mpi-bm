// string::size
#include <iostream>
#include <string>

int main ()
{
    std::string str (6, 'h');
    std::cout << "The size of str is " << str.size() << " bytes.\n";
    std::cout << str << std::endl;
    return 0;
}