// ======================================================================
// \title  FPManager.hpp
// \author watney
// \brief  cpp file for FPManager test harness implementation class
// ======================================================================

#include "Tester.hpp"
#include <Fw/Test/UnitTest.hpp>
#include "TestRules.hpp"

#define INSTANCE 0
#define MAX_HISTORY_SIZE 300
#define QUEUE_DEPTH 10

namespace Svc {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      FPManagerGTestBase("Tester", MAX_HISTORY_SIZE),
      component("FPManager")
  {
    this->initComponents();
    this->connectPorts();
  }

  Tester ::
    ~Tester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

   void Tester ::
     exercisePriorityQ(void)
   {
       TEST_CASE(6400.1.4,"Exercise the resonse pending priority queue");

       // Instantiate the Rules
       CheckPriorityQ checkPriorityQ;

       // Run the Rules
       checkPriorityQ.apply(*this);

   }



   void Tester ::
      checkMisc(void)
    {
        TEST_CASE(6400.1.5,"Check Misc");

        // Instantiate the Rules
        PackedTlm packedTlm;
        QueOverflow queOverflow;
        SendPing sendPing;
        BadArgs badArgs;
        BadResp badResp;
        ExecuteQueued executeQueued;

        // Run the Rules
        packedTlm.apply(*this);
        queOverflow.apply(*this);
        sendPing.apply(*this);
        badArgs.apply(*this);
        badResp.apply(*this);
        executeQueued.apply(*this);

    }


  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  void Tester ::
    from_comCmdOut_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::ComBuffer &data,
        U32 context
    )
  {
    this->pushFromPortEntry_comCmdOut(data, context);
  }

  void Tester ::
    from_pingOut_handler(
        const NATIVE_INT_TYPE portNum,
        U32 key
    )
  {
    this->pushFromPortEntry_pingOut(key);
  }

  // ----------------------------------------------------------------------
  // Helper methods
  // ----------------------------------------------------------------------

   void Tester::
    checkCmdOK(U16 opcode) {
      ASSERT_CMD_RESPONSE_SIZE(1);
      ASSERT_CMD_RESPONSE(0, opcode, 0, Fw::CmdResponse::OK);
  }

  void Tester ::
    dispatchAll()
  {
    while (this->component.m_queue.getNumMsgs() > 0)
        this->component.doDispatch();
  }

  //! Handle a text event
  //!
  void
  Tester::textLogIn(
    const FwEventIdType id, /*!< The event ID*/
    Fw::Time& timeTag, /*!< The time*/
    const Fw::LogSeverity severity, /*!< The severity*/
    const Fw::TextLogString& text /*!< The event string*/
  )
  {
    TextLogEntry e = {id, timeTag, severity, text};
    printTextLogHistoryEntry(e, stdout);
  }

  void Tester ::
    connectPorts()
  {

    // CmdDisp
    this->connect_to_CmdDisp(
        0,
        this->component.get_CmdDisp_InputPort(0)
    );

    // ResponseIn
    this->connect_to_ResponseIn(
        0,
        this->component.get_ResponseIn_InputPort(0)
    );

    // Run
    this->connect_to_Run(
        0,
        this->component.get_Run_InputPort(0)
    );

    // cmdResponseIn
    this->connect_to_cmdResponseIn(
        0,
        this->component.get_cmdResponseIn_InputPort(0)
    );

    // pingIn
    this->connect_to_pingIn(
        0,
        this->component.get_pingIn_InputPort(0)
    );

    // CmdReg
    this->component.set_CmdReg_OutputPort(
        0,
        this->get_from_CmdReg(0)
    );

    // CmdStatus
    this->component.set_CmdStatus_OutputPort(
        0,
        this->get_from_CmdStatus(0)
    );

    // Log
    this->component.set_Log_OutputPort(
        0,
        this->get_from_Log(0)
    );

    // LogText
    this->component.set_LogText_OutputPort(
        0,
        this->get_from_LogText(0)
    );

    // Time
    this->component.set_Time_OutputPort(
        0,
        this->get_from_Time(0)
    );

    // Tlm
    this->component.set_Tlm_OutputPort(
        0,
        this->get_from_Tlm(0)
    );

    // comCmdOut
    this->component.set_comCmdOut_OutputPort(
        0,
        this->get_from_comCmdOut(0)
    );

    // pingOut
    this->component.set_pingOut_OutputPort(
        0,
        this->get_from_pingOut(0)
    );




  }

  void Tester ::
    initComponents()
  {
    this->init();
    this->component.init(
        QUEUE_DEPTH, INSTANCE
    );
  }

} // end namespace Svc
