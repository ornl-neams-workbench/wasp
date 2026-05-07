#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "waspcore/version.h"
#include "waspfiller/FillerInterpreter.h"

using namespace wasp;

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Workbench Analysis Sequence Processor (Filler)" << std::endl
                  << argv[0]
                  << " : An application for listing Filler formatted input."
                  << std::endl;
        std::cout << " Usage : " << argv[0] << " path/to/Filler/formatted/input"
                  << std::endl;
        return 1;
    }
    for (int j = 1; j < argc; ++j)
    {
        std::ifstream input(argv[j]);
        if (input.fail() || input.bad())
        {
            std::cout << "Failed to open/read " << argv[j] << std::endl;
            input.close();
            return 1;
        }
        FillerInterpreter<> interpreter;
        bool              parsed = interpreter.parse(input);
        if (!parsed)
        {
            return -1;
        }
        interpreter.root().paths(std::cout);
    }
    return 0;
}
