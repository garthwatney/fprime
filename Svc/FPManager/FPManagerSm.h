
// ======================================================================
// \title  FPManagerSm.h
// \author Auto-generated
// \brief  header file for state machine FPManagerSm
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================
           
#ifndef FPMANAGERSM_H_
#define FPMANAGERSM_H_

namespace Svc {
  class SMEvents;
}

namespace Svc {

class FPManagerSmIf {
  public:
    virtual bool FPManagerSm_emptyQueue() = 0;
    virtual bool FPManagerSm_responseRunning() = 0;
    virtual void FPManagerSm_startNextResponse() = 0;
    virtual void FPManagerSm_reportResponseComplete() = 0;
                                                                  
};

class FPManagerSm {
                                 
  private:
    FPManagerSmIf *parent;
                                 
  public:
                                 
    FPManagerSm(FPManagerSmIf* parent) : parent(parent) {}
  
    enum FPManagerSmStates {
      IDLE,
      RUNNING,
    };

    enum FPManagerSmEvents {
      RTI_SIG,
    };
    
    enum FPManagerSmStates state;

    void * extension;

    void init();
    void update(const Svc::SMEvents *e);

};

}

#endif
