#include <string>

class String{
private:
    size_t messagesize;
    
public:
    
    String(size_t messagesize_);
    
    std::string* createstring();
};

String::String(size_t messagesize_):
messagesize(messagesize_)
{
    
    
}

std::string* String::createstring(){
    std::string* str = new std::string(messagesize, 'h');

    return str;
}