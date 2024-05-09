// ====================================================================== 
// \title  FPManager/test/ut/Tester.hpp
// \author watney
// \brief  hpp file for FPManager unit test
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged. Any commercial use must be negotiated with the Office
// of Technology Transfer at the California Institute of Technology.
// 
// This software may be subject to U.S. export control laws and
// regulations.  By accepting this document, the user agrees to comply
// with all U.S. export laws and regulations.  User has the
// responsibility to obtain export licenses, or other export authority
// as may be required before exporting such information to foreign
// countries or providing access to foreign persons.
// ====================================================================== 

#ifndef TESTER_HPP
#define TESTER_HPP

#include "FPManagerGTestBase.hpp"
#include "Svc/FPManager/FPManagerImpl.hpp"
#define QUEUE_DEPTH 10



namespace Svc {

  class Tester :
    public FPManagerGTestBase
  {

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

    public:

      //! Construct object Tester
      //!
      Tester(void);

      //! Destroy object Tester
      //!
      ~Tester(void);

      void textLogIn(
                  const FwEventIdType id, //!< The event ID
                  Fw::Time& timeTag, //!< The time
                  const Fw::LogSeverity severity, //!< The severity
                  const Fw::TextLogString& text //!< The event string
              );

    public:

      void dispatchAll(void);

      // ---------------------------------------------------------------------- 
      // Tests
      // ---------------------------------------------------------------------- 

      //! Test functions
      //!
      
      void executeResponses(void);
      void indicateResponsesDisabled(void);
      void indicateResponsesQueued(void);
      void indicateResponsesExecuting(void);
      void exercisePriorityQ(void);
      void checkMisc(void);

      //! Helper functions
      //!
      void checkCmdOK(U16 opcode);

    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler for from_comCmdOut
      //!
      void from_comCmdOut_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::ComBuffer &data, /*!< Buffer containing packet data*/
          U32 context /*!< Call context value; meaning chosen by user*/
      );

      //! Handler for from_pingOut
      //!
      void from_pingOut_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          U32 key /*!< Value to return to pinger*/
      );

      // ----------------------------------------------------------------------
      // Helper methods
      // ----------------------------------------------------------------------

      //! Connect ports
      //!
      void connectPorts(void);

      //! Initialize components
      //!
      void initComponents(void);

      // ----------------------------------------------------------------------
      // Variables
      // ----------------------------------------------------------------------

      //! The component under test
      //!
      FPManagerImpl component;

      friend class GenFaultIn;
      friend class ResponseTest;
      friend class CheckPriorityQ;
      friend class PackedTlm;
      friend class QueOverflow;
      friend class SendPing;
      friend class BadArgs;
      friend class BadResp;
      friend class ExecuteQueued;


      friend class ExecuteRESP_1;
      friend class IndicateRESP_1DroppedDisbled;
      friend class IndicateRESP_1DroppedQueued;
      friend class IndicateRESP_1DroppedExecuting;
      friend class ExecuteRESP_2;
      friend class IndicateRESP_2DroppedDisbled;
      friend class IndicateRESP_2DroppedQueued;
      friend class IndicateRESP_2DroppedExecuting;
      friend class ExecuteSAFING;
      friend class IndicateSAFINGDroppedDisbled;
      friend class IndicateSAFINGDroppedQueued;
      friend class IndicateSAFINGDroppedExecuting;
      
      
      
  };




} // end namespace Svc

#endif
