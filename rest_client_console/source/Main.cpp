#include "GdClient.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    GdClient gdc;
    std::cout << "GdClient is alive:" << gdc.alive() << "\n";

    //gdc.httpGetFromBing();
   // gdc.httpGetFromFlickr();
    gdc.getImage();
}
