#include <math.h>
#include "Tester.hpp"
#include "Svc/FPManager/FPManagerImpl.hpp"
#include "Svc/FPManager/CommandSerializer.hpp"
#include "Svc/FPManager/CommandOpcodes.hpp"
#include "TestRules.hpp"

namespace Svc {

CheckPriorityQ::CheckPriorityQ() :
       STest::Rule<Svc::Tester>("CheckPriorityQ")
{

}

bool CheckPriorityQ::precondition(const Tester& tester) {
    return (true);
}

void CheckPriorityQ::action(Tester& tester) {
    U32 priorityId = FPResponses::RESP_2;  // The response with the highest priority
    U32 respId;

    tester.clearHistory();

    // Send clear command to clear all the FP counters
    tester.sendCmd_FP_RESET_COUNTERS(0, 0);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESET_COUNTERS);
    tester.clearHistory();


    // Enable all the responses
     tester.sendCmd_FP_ALL_RESPONSE_IDS_ON_OFF(0, 0, RespAllEnableState::RID_ENABLE_ALL);
     tester.dispatchAll();
     tester.checkCmdOK(tester.component.OPCODE_FP_ALL_RESPONSE_IDS_ON_OFF);
     tester.clearHistory();

    // Load the priority queue by invoking the ResponseIn port with all the response requests
     for (int i = 0; i < FPResponses::NUMBER_RESPONSES; i++) {
         respId = i + 1;
         tester.invoke_to_ResponseIn(0, respId);
         tester.dispatchAll();

         // Check EVR that the response ID was placed in the queue
         tester.assertEvents_size(__FILE__, __LINE__, 1);
         tester.assertEvents_FP_RESPONSE_IN_QUEUE_size(__FILE__, __LINE__, 1);
         tester.assertEvents_FP_RESPONSE_IN_QUEUE(__FILE__, __LINE__, 0, respId);
         tester.clearHistory();
     }

     // Check that the highest priority is pulled from the queue first
     tester.invoke_to_Run(0, 0);
     tester.dispatchAll();
     tester.assertEvents_size(__FILE__, __LINE__, 1);
     tester.assertEvents_FP_RESPONSE_STARTED_size(__FILE__, __LINE__, 1);
     tester.assertEvents_FP_RESPONSE_STARTED(__FILE__, __LINE__, 0, priorityId);
     tester.clearHistory();

     // Check the response is run to completion
     do {
         tester.invoke_to_Run(0, 0);
         tester.dispatchAll();
     } while (FPManagerImpl::responseTable[priorityId-1].response->isPending());

     tester.assertEvents_size(__FILE__, __LINE__, 1);
     tester.assertEvents_FP_RESPONSE_COMPLETED_size(__FILE__, __LINE__, 1);
     tester.assertEvents_FP_RESPONSE_COMPLETED(__FILE__, __LINE__, 0, priorityId);
     tester.clearHistory();

     // Check that all other responses start and complete
     do {
         tester.invoke_to_Run(0, 0);
         tester.dispatchAll();
     } while ( (!tester.component.respPendingQue.isEmpty()) || (tester.component.fpManagerSm.state != FPManagerSm::IDLE) );

     tester.assertEvents_FP_RESPONSE_STARTED_size(__FILE__, __LINE__, FPResponses::NUMBER_RESPONSES-1);
     tester.assertEvents_FP_RESPONSE_COMPLETED_size(__FILE__, __LINE__, FPResponses::NUMBER_RESPONSES-1);

     int evCtr = 0;
     for (int i = 0; i < FPResponses::NUMBER_RESPONSES; i++) {
         respId = i + 1;
         if (respId != priorityId) {
             tester.assertEvents_FP_RESPONSE_STARTED(__FILE__, __LINE__, evCtr, respId);
             tester.assertEvents_FP_RESPONSE_COMPLETED(__FILE__, __LINE__, evCtr, respId);
             evCtr++;
         }
     }

}

PackedTlm::PackedTlm() :
        STest::Rule<Svc::Tester>("PackedTlm")
{

}

bool PackedTlm::precondition(const Tester& tester) {
    return (true);
}

void PackedTlm::action(Tester& tester) {

    tester.clearHistory();

    // Enable all responses
    tester.sendCmd_FP_ALL_RESPONSE_IDS_ON_OFF(0, 0, RespAllEnableState::RID_ENABLE_ALL);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_ALL_RESPONSE_IDS_ON_OFF);

    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();

    // Figure out the packed telemetry value
    U32 expectedTlmVal = pow(2, static_cast<int>(FPResponses::NUMBER_RESPONSES)) - 1;

    // Check packed telemetry
    tester.assertTlm_size(__FILE__, __LINE__, 3);
    tester.assertTlm_FP_ResponsePackedState(__FILE__, __LINE__, 0, expectedTlmVal);



    // Disable one response at a time and check the packed telemetry word
    for (int i = 0; i < FPResponses::NUMBER_RESPONSES; i++) {
        // Disable a response
        U32 respId = i + 1;
        tester.clearHistory();
        tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, respId, RespEnableState::RID_DISABLE);
        tester.dispatchAll();
        tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);

        U32 k = FPResponses::NUMBER_RESPONSES - i;
        printf("expectedTlm = %x\n", expectedTlmVal);
        // turn off the kth bit of expectedTlmVal
        expectedTlmVal = (expectedTlmVal & ~(1 << (k - 1)));
        // Check packed telemetry
        tester.assertTlm_size(__FILE__, __LINE__, 1);
        tester.assertTlm_FP_ResponsePackedState(__FILE__, __LINE__, 0, expectedTlmVal);
    }
}


    QueOverflow::QueOverflow() :
        STest::Rule<Svc::Tester>("QueOverflow")
    {

    }

    bool QueOverflow::precondition(const Tester& tester) {
        return (true);
    }

    void QueOverflow::action(Tester& tester) {

        tester.clearHistory();

        // Check the Responses Dropped EVR throttle limit
        for (int j = 0; j < 10; j++) {

            tester.clearHistory();

            // Flood the input queue with response notifications and cause a drop
            for (int i = 0; i < QUEUE_DEPTH + 1; i++) {
                tester.invoke_to_ResponseIn(0, 1);
            }

            tester.dispatchAll();

            tester.invoke_to_Run(0, 0);
            tester.dispatchAll();

            tester.assertEvents_FP_RESPONSES_DROPPED_size(__FILE__, __LINE__, 1);
            tester.assertEvents_FP_RESPONSES_DROPPED(__FILE__, __LINE__, 0, j+1);

        }

        // And just 1 more time to see if the EVR really throttles itself.
        // Flood the input queue with response notifications and cause a drop
        tester.clearHistory();
        for (int i = 0; i < QUEUE_DEPTH + 1; i++) {
            tester.invoke_to_ResponseIn(0, 1);
        }

        tester.dispatchAll();

        tester.invoke_to_Run(0, 0);
        tester.dispatchAll();

        tester.assertEvents_FP_RESPONSES_DROPPED_size(__FILE__, __LINE__, 0);

        // Send command to unthrottle EVR
        tester.sendCmd_FP_RESET_EVR_THROTTLING(0, 0);
        tester.dispatchAll();
        tester.checkCmdOK(tester.component.OPCODE_FP_RESET_EVR_THROTTLING);

        // Flood again
        // Flood the input queue with response notifications and cause a drop
        for (int i = 0; i < QUEUE_DEPTH + 1; i++) {
            tester.invoke_to_ResponseIn(0, 1);
        }

        tester.dispatchAll();

        tester.invoke_to_Run(0, 0);
        tester.dispatchAll();

        tester.assertEvents_FP_RESPONSES_DROPPED_size(__FILE__, __LINE__, 1);
        tester.assertEvents_FP_RESPONSES_DROPPED(__FILE__, __LINE__, 0, 12);

}

    SendPing::SendPing() :
        STest::Rule<Svc::Tester>("SendPing")
    {

    }

    bool SendPing::precondition(const Tester& tester) {
      return true;
    }

    void SendPing::action(Tester& tester) {
      U32 ping;
      tester.invoke_to_pingIn(0, 42);
      tester.dispatchAll();

      ASSERT_EQ(tester.fromPortHistory_pingOut->size(), 1);

      ping = tester.fromPortHistory_pingOut->at(0).key;

      ASSERT_EQ(ping, static_cast<U32>(42));

    }

    BadArgs::BadArgs() :
        STest::Rule<Svc::Tester>("BadArgs")
    {

    }

    bool BadArgs::precondition(const Tester& tester) {
       return true;
     }

     void BadArgs::action(Tester& tester) {
         tester.clearHistory();

         // Send Execute response command
         tester.sendCmd_FP_EXECUTE_FAULT_RESPONSE(0, 0, FPResponses::NUMBER_RESPONSES + 1);
         tester.dispatchAll();
         tester.assertCmdResponse_size(__FILE__, __LINE__, 1);
         tester.assertCmdResponse(__FILE__, __LINE__, 0, tester.component.OPCODE_FP_EXECUTE_FAULT_RESPONSE, 0, Fw::CmdResponse::VALIDATION_ERROR);

         tester.assertEvents_FP_BAD_COMMAND_ARGUMENT_size(__FILE__, __LINE__, 1);
         tester.assertEvents_FP_BAD_COMMAND_ARGUMENT(__FILE__, __LINE__, 0, FPResponses::NUMBER_RESPONSES + 1);

         tester.clearHistory();

         // Send enable response command
         tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, 0, RespEnableState::RID_ENABLE);
         tester.dispatchAll();
         tester.assertCmdResponse_size(__FILE__, __LINE__, 1);
         tester.assertCmdResponse(__FILE__, __LINE__, 0, tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF, 0, Fw::CmdResponse::VALIDATION_ERROR);

         tester.assertEvents_FP_BAD_COMMAND_ARGUMENT_size(__FILE__, __LINE__, 1);
         tester.assertEvents_FP_BAD_COMMAND_ARGUMENT(__FILE__, __LINE__, 0, 0);


     }

     BadResp::BadResp() :
        STest::Rule<Svc::Tester>("BadResp")
    {

    }

     bool BadResp::precondition(const Tester& tester) {
         return true;
       }

       void BadResp::action(Tester& tester) {
           tester.clearHistory();

           // Send a good response
           tester.invoke_to_cmdResponseIn(0, OPCODES::CMD_NO_OP, 0, Fw::CmdResponse::OK);
           tester.dispatchAll();

           // Check no EVR's
           tester.assertEvents_size(__FILE__, __LINE__, 0);

           // Send a bad response
           tester.invoke_to_cmdResponseIn(0, OPCODES::CMD_NO_OP, 0, Fw::CmdResponse::VALIDATION_ERROR);
           tester.dispatchAll();

           // Check EVR
           tester.assertEvents_size(__FILE__, __LINE__, 1);
           tester.assertEvents_FP_BAD_RESPONSE(__FILE__, __LINE__, 0, OPCODES::CMD_NO_OP);


       }

       ExecuteQueued::ExecuteQueued() :
        STest::Rule<Svc::Tester>("ExecuteQueued")
    {

    }

       bool ExecuteQueued::precondition(const Tester& tester) {
            return true;
          }

       void ExecuteQueued::action(Tester& tester) {
              tester.clearHistory();

              // Send Enable response command
              tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, 1, RespEnableState::RID_ENABLE);
              tester.dispatchAll();
              tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
              tester.clearHistory();

              // Send Execute response command
              tester.sendCmd_FP_EXECUTE_FAULT_RESPONSE(0, 0, 1);
              tester.dispatchAll();
              tester.checkCmdOK(tester.component.OPCODE_FP_EXECUTE_FAULT_RESPONSE);

              tester.clearHistory();

              // Send the Execute response command again
              tester.sendCmd_FP_EXECUTE_FAULT_RESPONSE(0, 0, 1);
              tester.dispatchAll();
              tester.checkCmdOK(tester.component.OPCODE_FP_EXECUTE_FAULT_RESPONSE);

              // Check EVR: Already queued
               tester.assertEvents_size(__FILE__, __LINE__, 1);
               tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED(__FILE__, __LINE__, 0, 1);


       }

}
