
// ======================================================================
// \title  FPManagerSm.cpp
// \author Auto-generated
// \brief  cpp file for state machine FPManagerSm
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================            
    
#include "stdio.h"
#include "assert.h"
#include "SMEvents.hpp"
#include "FPManagerSm.h"


void Svc::FPManagerSm::init()
{
    this->state = IDLE;

}


void Svc::FPManagerSm::update(const Svc::SMEvents *e)
{
    switch (this->state) {
    
            /**
            * state IDLE
            */
            case IDLE:
            
            switch (e->geteventSignal()) {

                case RTI_SIG:
                        if ( parent->FPManagerSm_emptyQueue() ) {
                            this->state = IDLE;
                        }
                        else {
                            parent->FPManagerSm_startNextResponse();
                            this->state = RUNNING;
                        }

                    break;
    
                default:
                    break;
            }
            break;
    
            /**
            * state RUNNING
            */
            case RUNNING:
            
            switch (e->geteventSignal()) {

                case RTI_SIG:
                        if ( parent->FPManagerSm_responseRunning() ) {
                            this->state = RUNNING;
                        }
                        else {
                            parent->FPManagerSm_reportResponseComplete();
                            this->state = IDLE;
                        }

                    break;
    
                default:
                    break;
            }
            break;
    
        default:
        assert(0);
    }
}
