
#ifndef FPMANAGER_SM_BASE_HPP
#define FPMANAGER_SM_BASE_HPP
// ======================================================================
// \title  FPManagerSmBase.hpp
// \author Auto-generated
// \brief  Header file for the state machine base class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================            
#include "Svc/FPManager/FPManagerComponentAc.hpp"
#include "Svc/FPManager/FPManagerSm.h"
                                
namespace Svc {
    namespace StateMachine {
        typedef enum {
            FPMANAGERSM,
        } SmId;                           
    };

    class FPManagerSmBase : public FPManagerComponentBase
        ,public FPManagerSmIf
                                
    {
        public:
            FPManagerSmBase(const char* const compName);
            void init(
                        NATIVE_INT_TYPE queueDepth,
                        NATIVE_INT_TYPE instance
            );
            
            // Interface to send an event to the state-machine
            void sendEvent(U32 eventSignal, StateMachine::SmId id);

            // Internal Interface handler for sendEvents
            void sendEvents_internalInterfaceHandler(const Svc::SMEvents& ev);
                                
            // Instantiate the state machines
            FPManagerSm fpManagerSm;
            
                                
    };
}
#endif

            