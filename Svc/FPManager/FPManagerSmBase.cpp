
// ======================================================================
// \title  FPManagerSmBase.cpp
// \author Auto-generated
// \brief  Cpp file for the state machine base class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================            
#include "Svc/FPManager/FPManagerSmBase.hpp"
#include "Svc/FPManager/FPManagerSm.h"
                                
Svc::FPManagerSmBase::FPManagerSmBase(const char* const compName):
    FPManagerComponentBase(compName)
    ,fpManagerSm(this)
{
                                
}                               

void Svc::FPManagerSmBase::init(
            NATIVE_INT_TYPE queueDepth,
            NATIVE_INT_TYPE instance)
{
    FPManagerComponentBase::init(queueDepth, instance);
                                
    // Initialize the state machine
    fpManagerSm.init();
    
} 

void Svc::FPManagerSmBase:: sendEvent(U32 eventSignal, StateMachine::SmId id) {
                                
    Svc::SMEvents event;
    event.seteventSignal(eventSignal);
    event.setsmId(id);
    sendEvents_internalInterfaceInvoke(event);
}

void Svc::FPManagerSmBase::sendEvents_internalInterfaceHandler(const Svc::SMEvents& ev)
{
    U16 id = ev.getsmId();
    switch (id) {
                                
        case StateMachine::FPMANAGERSM:
            this->fpManagerSm.update(&ev);
            break;
        default:
            FW_ASSERT(0);
    }

}
            