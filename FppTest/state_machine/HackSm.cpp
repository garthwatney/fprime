
// ======================================================================
// \title  HackSm.cpp
// \author Auto-generated
// \brief  cpp file for state machine HackSm
//
// ======================================================================            
    
#include "stdio.h"
#include "assert.h"
#include "Fw/Types/SMSignalsSerializableAc.hpp"
#include "HackSm.hpp"


void FppTest::HackSm::init(const FwEnumStoreType stateMachineId)
{
    parent->HackSm_turnOff(stateMachineId);
    this->state = OFF;

}


void FppTest::HackSm::update(
    const FwEnumStoreType stateMachineId, 
    const HackSm_Interface::HackSmEvents signal, 
    const Fw::SMSignalBuffer &data
)
{
    switch (this->state) {
    
            /**
            * state OFF
            */
            case OFF:
            
            switch (signal) {

                case HackSm_Interface::HackSmEvents::RTI_SIG:
                        parent->HackSm_turnOn(stateMachineId);
                        this->state = ON;

                    break;
    
                case HackSm_Interface::HackSmEvents::CHECK_SIG:
                        parent->HackSm_doDiag(stateMachineId);
                        this->state = DIAG;

                    break;
    
                default:
                    break;
            }
            break;
    
            /**
            * state ON
            */
            case ON:
            
            switch (signal) {

                case HackSm_Interface::HackSmEvents::RTI_SIG:
                        parent->HackSm_turnOff(stateMachineId);
                        this->state = OFF;

                    break;
    
                case HackSm_Interface::HackSmEvents::CHECK_SIG:
                        parent->HackSm_doDiag(stateMachineId);
                        this->state = DIAG;

                    break;
    
                default:
                    break;
            }
            break;
    
            /**
            * state DIAG
            */
            case DIAG:
            
            switch (signal) {

                case HackSm_Interface::HackSmEvents::RTI_SIG:
                        parent->HackSm_turnOff(stateMachineId);
                        this->state = OFF;

                    break;
    
                default:
                    break;
            }
            break;
    
        default:
        assert(0);
    }
}
