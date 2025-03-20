#include <string>
#include <vector> 
#include <iostream>
#include <array>

int main(int argc, char**argv)
{
    std::cout << "Executing: " << argv[0] << std::endl;

    int firstNum;
    int secondNum;
     
    if (argc >= 3) {
        firstNum  = atoi( argv[1] );
        secondNum = atoi( argv[2] );
        std::cout << "First integer: " << firstNum << " Second integer: "<< secondNum << std::endl;
    }
    else {
        std::cout << "Give me a number, any full number!" << std::endl;
        std::cin >> firstNum;
        std::cout << "Give me another number, any full number!" << std::endl;
        std::cin >> secondNum;
    }
    std::cout << secondNum * firstNum << std::endl;
    
    return 0;
}
