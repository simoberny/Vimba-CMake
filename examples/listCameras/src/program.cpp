#include <iostream>

#include "ListCameras.h"

int main( int argc, char* argv[] ){
    std::cout << "\n";
    std::cout << "//////////////////////////////////////\n";
    std::cout << "/// Vimba API List Cameras Example ///\n";
    std::cout << "//////////////////////////////////////\n\n";

    AVT::VmbAPI::ListCameras::Print();
}
