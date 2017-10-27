//
//  Component.hpp
//  
//
//  Created by Jake Given on 10/26/17.
//
//

#ifndef Component_hpp
#define Component_hpp

#include <stdio.h>

class Component{
private:
    String type;
    std::vector<String> DataInputs;
    std::vector<String> ControlInputs;
    std::vector<String> DataOutputs;
    std::vector<String> ControlOutputs;
    String description;
    int dataWidth;
    
    
public:
    Component(String line);
    
    
};

#endif /* Component_hpp */
