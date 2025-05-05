#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "waspcore/version.h"
#include "waspcore/wasp_bug.h"
#include "waspcore/Object.h"

#include "waspmcnpi/MCNPInterpreter.h"
#include "waspmcnpi/Model.h"

using namespace wasp;

int main(int argc, char* argv[])
{
    if (argc == 2 &&
        (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--version"))
    {
        std::cout << wasp_version_info::name << " "
                  << wasp_version_info::full_version << std::endl;
        return 0;
    }

    if (argc == 1)
    {
        std::cout << "Workbench Analysis Sequence Processor (WASP)" << std::endl
                  << argv[0] << " : MCNP to JSON (materials) Converter"
                  << std::endl;
        std::cout << " Usage : " << argv[0] << " path/to/MCNP/formatted/input"
                  << std::endl;
        std::cout << " Usage : " << argv[0]
                  << " --version\t(print version info)" << std::endl;
        return 1;
    }


    DataArray da_error;
    for (int j = 1; j < argc; ++j)
    {
        std::stringstream        errors;

        // First attempt to parse the document
        DefaultMCNPInterpreter interpreter(errors);
        bool parsed = interpreter.parseFile(argv[j]);                
        if (!parsed)
        {
            da_error.push_back(errors.str());
            continue;
        }
  
        // Next try to construct the MCNP model
        mcnpi::Model model;
        bool success = model.build_limited(interpreter.root(), errors);
        if (!success)
        {
            da_error.push_back(errors.str());
            continue;
        }
        // Output the materials in JSON format
        model.describe_materials_json(std::cout);       
    }
    if(!da_error.empty())
    {
        da_error.format_json(std::cout);
        return -1;
    }
    return 0;
}
