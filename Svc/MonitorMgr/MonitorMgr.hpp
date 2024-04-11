// ======================================================================
// \title  MonitorMgr.hpp
// \author watney
// \brief  hpp file for MonitorMgr component implementation class
// ======================================================================

#ifndef Svc_MonitorMgr_HPP
#define Svc_MonitorMgr_HPP

#include "Svc/MonitorMgr/MonitorMgrComponentAc.hpp"

namespace Svc {

  class MonitorMgr :
    public MonitorMgrComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Component construction and destruction
      // ----------------------------------------------------------------------

      //! Construct MonitorMgr object
      MonitorMgr(
          const char* const compName //!< The component name
      );

      //! Destroy MonitorMgr object
      ~MonitorMgr();

    PRIVATE:


      void run_handler(
            NATIVE_INT_TYPE portNum,
            NATIVE_UINT_TYPE context); 


      // ----------------------------------------------------------------------
      // Handler implementations for commands
      // ----------------------------------------------------------------------

      void START_FAULT_cmdHandler(
          FwOpcodeType opCode, //!< The opcode
          U32 cmdSeq,
          U32 numFaults) override;

      U32 faultCtr;
      U32 numFaults;

  };

}

#endif
