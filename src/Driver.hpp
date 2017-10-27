//
//  Driver.hpp
//  
//
//  Created by Jake Given on 10/27/17.
//
//

#ifndef Driver_hpp
#define Driver_hpp
#include "vmodule.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
class Driver{
public:
    Driver();
    void Run(string netlistFilename, string circuitFilename);
    vmodule G;
    map<string, map<int,double> > latencies;
    string getType(string line);
    double CalculateLatency();
    
};

#endif /* Driver_hpp */
