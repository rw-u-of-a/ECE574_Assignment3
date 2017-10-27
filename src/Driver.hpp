//
//  Driver.hpp
//  
//
//  Created by Jake Given on 10/27/17.
//
//

#ifndef Driver_hpp
#define Driver_hpp

#include <stdio.h>
#include "vmodule.h"

using namespace std;
class Driver{
public:
    Driver();
    Run(string netlistFilename, string circuitFilename);
    vmodule G;
    map<string, map<int,double>> latencies;
    
};

#endif /* Driver_hpp */
