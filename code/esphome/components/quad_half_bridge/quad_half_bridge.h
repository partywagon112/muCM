#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"

# Namespace definition
namespace esphome {
namespace my_external_component {

class QuadHalfBridge : public Component {

 public:
     // Constructor
     QuadHalfBridge(
        InternalGPIOPin *aEN,  
        output::FloatOutput *aSTATE,
        InternalGPIOPin *bEN,  
        output::FloatOutput *bSTATE,
        InternalGPIOPin *cEN,  
        output::FloatOutput *cSTATE,
        InternalGPIOPin *dEN,  
        output::FloatOutput *dSTATE);
    
     // Configuration function
     void set_conf_x(float conf_x);
    
     // Standard component functions to override
     void setup() override;
     void dump_config() override;
    
protected:
    InternalGPIOPin *aEN;
    output::FloatOutput *aSTATE;
    InternalGPIOPin *bEN;
    output::FloatOutput *bSTATE;
    InternalGPIOPin *cEN;
    output::FloatOutput *cSTATE;
    InternalGPIOPin *dEN;
    output::FloatOutput *dSTATE;
};

}  // namespace cc1101
}  // namespace esphome