#include <iostream>
#include "LightControllerStateDelegate.h"
#include "LightControllerBaseState.h"
#include "LightControllerConfigurator.h"
#include "LightControllerLampOff.h"

using LightControllerNamespace::LightControllerStateDelegate;
using LightControllerNamespace::LightControllerBaseState;
using LightControllerNamespace::Parameters;
using LightControllerNamespace::LightControllerConfigurator;
using LightControllerNamespace::LightControllerLampOff;



class MyLightController: public LightControllerStateDelegate
{
private:
    LightControllerBaseState * current;

private:
    virtual void lampTurnedOff(const Parameters& parameters) 
    {
        std::cout << "Ligth turned off..." << std::endl;
    }

    virtual void lampTurnedOn(const Parameters& parameters) 
    {
        std::cout << "Ligth turned on... " << std::endl;
    }

    virtual void lampAlreadyTurnedOff(const Parameters& parameters) 
    {
        std::cout << "Hey! Lamp is already turned off!" << std::endl;
    }

    virtual void lampAlreadyTurnedOn(const Parameters& parameters)
    {
        std::cout << "Hey! Lamp is already turned on!" << std::endl;
    }

    virtual void LightControllerChangeState(LightControllerBaseState * newState) 
    {
        current = newState;
    };

    
public:
    MyLightController()
    {
        current = LightControllerLampOff::sharedInstance();
        LightControllerConfigurator::sharedInstance()->setAllStatesDelegate(this);
    }
 
    void turnLightOn()
    {
        current->turnOn();
    }    

    void turnLightOff()
    {
        current->turnOff();
    }
};


int main(int argc, const char * argv[])
{
    MyLightController lightController;

    lightController.turnLightOn();
    lightController.turnLightOn();
    lightController.turnLightOff();
    lightController.turnLightOn();
    lightController.turnLightOff();
    lightController.turnLightOff();
 
    return 0;
}
