
#include <iostream>

#include "SyntaxAnalisys.h"

int main()
{
    LexicAnalys a;
    std::string str;
    //std::cin >> str;
    a.analysis("test.txt");

    SyntaxisAnalisys sync(a.getTokens());

    sync.startAnalisys();

} 

