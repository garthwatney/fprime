// ======================================================================
// \title  MonitorMgr.cpp
// \author watney
// \brief  cpp file for MonitorMgr component implementation class
// ======================================================================

#include "FpConfig.hpp"
#include <stdlib.h>
#include "Svc/MonitorMgr/MonitorMgr.hpp"

namespace Svc {

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  MonitorMgr ::
    MonitorMgr(const char* const compName) :
      MonitorMgrComponentBase(compName),
      faultCtr(10)
  {

  }

  MonitorMgr ::
    ~MonitorMgr()
  {

  }

  void MonitorMgr::run_handler(
    NATIVE_INT_TYPE portNum,
    NATIVE_UINT_TYPE context) 
  {
    if (this->faultCtr < 10) {
      U32 fault = rand() % 3 + 1;
      ResponseOut_out(0, fault);
      this->faultCtr ++;
    }
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void MonitorMgr::START_FAULT_cmdHandler(
    FwOpcodeType opCode, //!< The opcode
    U32 cmdSeq)
  {
    this->faultCtr = 0;
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }


}
