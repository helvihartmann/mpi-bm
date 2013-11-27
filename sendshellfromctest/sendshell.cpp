#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
    //system("./hello.sh");
    execl("sendinput.sh", "sendinput.sh", NULL);
    //execl("hello", "hello", NULL);
}
