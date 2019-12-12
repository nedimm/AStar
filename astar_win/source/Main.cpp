#include <stdio.h>
#include "../include/Application.hpp"

int main(int argc, char** argv)
{
    Application app;
    if (argc == 1)
    {
        app.readParametersFromCSV();
    }else
    {
        app.readParametersFromCommandLine(argv);
    }
    app.start();
    return 0;
}
