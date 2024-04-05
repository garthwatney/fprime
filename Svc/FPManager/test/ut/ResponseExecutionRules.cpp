#include "Tester.hpp"
#include "ResponseExecutionRules.hpp"
#include "Svc/FPManager/FPManagerImpl.hpp"

#include "Svc/FPManager/RESP_1.hpp"
#include "Svc/FPManager/RESP_2.hpp"
#include "Svc/FPManager/SAFING.hpp"

#include "Svc/FPManager/CommandSerializer.hpp"
#include <Fw/Test/UnitTest.hpp>
#include "Svc/FPManager/FPResponses.hpp"



namespace Svc {


// ----------------------------------------------------------------------
// Test Cases
// ----------------------------------------------------------------------

void Tester ::
   executeResponses(void)
 {
     TEST_CASE(6400.1.1,"Execute Responses");

     // Instantiate the Rules
         ExecuteRESP_1 executeRESP_1;
         ExecuteRESP_2 executeRESP_2;
         ExecuteSAFING executeSAFING;

     // Run the Rules
         executeRESP_1.apply(*this);
         executeRESP_2.apply(*this);
         executeSAFING.apply(*this);

 }
 
 void Tester ::
   indicateResponsesDisabled(void)
 {
     TEST_CASE(6400.1.2,"Indicate Responses when Disabled");


     // Instantiate the Rules
         IndicateRESP_1DroppedDisbled indicateRESP_1DroppedDisabled;
         IndicateRESP_2DroppedDisbled indicateRESP_2DroppedDisabled;
         IndicateSAFINGDroppedDisbled indicateSAFINGDroppedDisabled;

     // Run the Rules
         indicateRESP_1DroppedDisabled.apply(*this);
         indicateRESP_2DroppedDisabled.apply(*this);
         indicateSAFINGDroppedDisabled.apply(*this);

 }
 
 void Tester ::
   indicateResponsesQueued(void)
 {
     TEST_CASE(6400.1.3,"Indicate Responses when Queued");


     // Instantiate the Rules
         IndicateRESP_1DroppedQueued indicateRESP_1DroppedQueued;
         IndicateRESP_2DroppedQueued indicateRESP_2DroppedQueued;
         IndicateSAFINGDroppedQueued indicateSAFINGDroppedQueued;

     // Run the Rules
         indicateRESP_1DroppedQueued.apply(*this);
         indicateRESP_2DroppedQueued.apply(*this);
         indicateSAFINGDroppedQueued.apply(*this);

 }
 
 
void Tester ::
   indicateResponsesExecuting(void)
 {
     TEST_CASE(6400.1.,"Indicate Responses when already executing");


     // Instantiate the Rules
         IndicateRESP_1DroppedExecuting indicateRESP_1DroppedExecuting;
         IndicateRESP_2DroppedExecuting indicateRESP_2DroppedExecuting;
         IndicateSAFINGDroppedExecuting indicateSAFINGDroppedExecuting;

     // Run the Rules
         indicateRESP_1DroppedExecuting.apply(*this);
         indicateRESP_2DroppedExecuting.apply(*this);
         indicateSAFINGDroppedExecuting.apply(*this);

     
 }
 

// ----------------------------------------------------------------------
// Rules
// ----------------------------------------------------------------------
  

ExecuteRESP_1::ExecuteRESP_1() :
       STest::Rule<Svc::Tester>("ExecuteRESP_1")
{

}

bool ExecuteRESP_1::precondition(const Tester& tester) {
    return (true);
}

void ExecuteRESP_1::action(Tester& tester) {
    Fw::ComBuffer comBuffer, returnedBuffer;
    Fw::SerializeStatus status;
    Fw::Time time;

    tester.clearHistory();

    // Send Enable RESP_1 response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::RESP_1, RespEnableState::RID_ENABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send Execute RESP_1 response command
    tester.sendCmd_FP_EXECUTE_FAULT_RESPONSE(0, 0, FPResponses::RESP_1);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_EXECUTE_FAULT_RESPONSE);
    
    // Check EVR that RESP_1 has queued
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE(__FILE__, __LINE__, 0, FPResponses::RESP_1);
    tester.clearHistory();

    // Run 1 iteration
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();

    // Check EVR that RESP_1 response has started
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED(__FILE__, __LINE__, 0, FPResponses::RESP_1);
    
        
    // Check command #1 for RESP_1 response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_NO_OP(comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    
        
    // Check command #2 for RESP_1 response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_TEST_CMD_1(1048575, 3.1415, 96,comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    
        
    // Check command #3 for RESP_1 response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_TEST_CMD_1(2248575, 2.718, 97,comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    
        
    // Check command #4 for RESP_1 response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_TEST_CMD_1(3048575, 2.997, 98,comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    
        
    // Check command #5 for RESP_1 response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_CLEAR_TRACKING(comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    


    // Check no more RESP_1 commands
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 0);

    // Check EVR that RESP_1 response has completed
     tester.assertEvents_size(__FILE__, __LINE__, 1);
     tester.assertEvents_FP_RESPONSE_COMPLETED_size(__FILE__, __LINE__, 1);
     tester.assertEvents_FP_RESPONSE_COMPLETED(__FILE__, __LINE__, 0, FPResponses::RESP_1);

     // Check telemetry response completion counter
     tester.assertTlm_size(__FILE__, __LINE__, 3);
     tester.assertTlm_FP_RESP_1CompleteCount(__FILE__, __LINE__, 0, 1);
     tester.assertTlm_FP_LastResponseComplete(__FILE__, __LINE__, 0, FPResponses::RESP_1);

}

IndicateRESP_1DroppedDisbled::IndicateRESP_1DroppedDisbled() :
       STest::Rule<Svc::Tester>("IndicateRESP_1DroppedDisbled")
{
    
}

bool IndicateRESP_1DroppedDisbled::precondition(const Tester& tester) {
    return (true);
}

void IndicateRESP_1DroppedDisbled::action(Tester& tester) {

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

    // Send Disable RESP_1 response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::RESP_1, RespEnableState::RID_DISABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send RESP_1 indication
    tester.invoke_to_ResponseIn(0, FPResponses::RESP_1);
    tester.dispatchAll();

    // Check EVR that RESP_1 was dropped because the response is disabled
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_DISABLED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_DISABLED(__FILE__, __LINE__, 0, FPResponses::RESP_1);

    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 2);
    tester.assertTlm_FP_RESP_1AlertCount(__FILE__, __LINE__, 0, 1);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::RESP_1);

}

IndicateRESP_1DroppedQueued::IndicateRESP_1DroppedQueued() :
       STest::Rule<Svc::Tester>("IndicateRESP_1DroppedQueued")
{
    
}

bool IndicateRESP_1DroppedQueued::precondition(const Tester& tester) {
    return (true);
}

void IndicateRESP_1DroppedQueued::action(Tester& tester) {

    tester.clearHistory();

    // Send clear command to clear all the FP counters
    tester.sendCmd_FP_RESET_COUNTERS(0, 0);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESET_COUNTERS);
    tester.clearHistory();


    // Disable all the responses
     tester.sendCmd_FP_ALL_RESPONSE_IDS_ON_OFF(0, 0, RespAllEnableState::RID_DISABLE_ALL);
     tester.dispatchAll();
     tester.checkCmdOK(tester.component.OPCODE_FP_ALL_RESPONSE_IDS_ON_OFF);
     tester.clearHistory();

    // Send Enable RESP_1 response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::RESP_1, RespEnableState::RID_ENABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send RESP_1 indication
    tester.invoke_to_ResponseIn(0, FPResponses::RESP_1);
    tester.dispatchAll();

    // Check EVR that RESP_1 was placed in the queue
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE(__FILE__, __LINE__, 0, FPResponses::RESP_1);

    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 2);
    tester.assertTlm_FP_RESP_1AlertCount(__FILE__, __LINE__, 0, 1);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::RESP_1);

    tester.clearHistory();

    // Send RESP_1 indication again
    tester.invoke_to_ResponseIn(0, FPResponses::RESP_1);
    tester.dispatchAll();

    // Check EVR that RESP_1 was dropped because the response is already queued
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED(__FILE__, __LINE__, 0, FPResponses::RESP_1);

    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 2);
    tester.assertTlm_FP_RESP_1AlertCount(__FILE__, __LINE__, 0, 2);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::RESP_1);
    
    // Flush the queue and bring all responses back to quiescent state
    do {
        tester.invoke_to_Run(0, 0);
        tester.dispatchAll();
    } while (tester.component.fpState != FPManagerImpl::IDLE);

}

IndicateRESP_1DroppedExecuting::IndicateRESP_1DroppedExecuting() :
       STest::Rule<Svc::Tester>("IndicateRESP_1DroppedExecuting")
{
    
}

bool IndicateRESP_1DroppedExecuting::precondition(const Tester& tester) {
    return (true);
}

void IndicateRESP_1DroppedExecuting::action(Tester& tester) {

    tester.clearHistory();

    // Send clear command to clear all the FP counters
    tester.sendCmd_FP_RESET_COUNTERS(0, 0);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESET_COUNTERS);
    tester.clearHistory();


    // Disable all the responses
     tester.sendCmd_FP_ALL_RESPONSE_IDS_ON_OFF(0, 0, RespAllEnableState::RID_DISABLE_ALL);
     tester.dispatchAll();
     tester.checkCmdOK(tester.component.OPCODE_FP_ALL_RESPONSE_IDS_ON_OFF);
     tester.clearHistory();

    // Send Enable RESP_1 response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::RESP_1, RespEnableState::RID_ENABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send RESP_1 indication
    tester.invoke_to_ResponseIn(0, FPResponses::RESP_1);
    tester.dispatchAll();
    
    // Check EVR that RESP_1 response has queued
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE(__FILE__, __LINE__, 0, FPResponses::RESP_1);

    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();

    // Check EVR that RESP_1 response has started
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED(__FILE__, __LINE__, 0, FPResponses::RESP_1);

    // Send RESP_1 indication again
    tester.invoke_to_ResponseIn(0, FPResponses::RESP_1);
    tester.dispatchAll();

    // Check EVR that RESP_1 was dropped because the response is running (same EVR as if it was queued)
    tester.assertEvents_size(__FILE__, __LINE__, 2);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED(__FILE__, __LINE__, 0, FPResponses::RESP_1);


    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 3);
    // Count should now be 2.
    tester.assertTlm_FP_RESP_1AlertCount(__FILE__, __LINE__, 0, 2);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::RESP_1);
    
    // Flush the queue and bring all responses back to quiescent state
    do {
        tester.invoke_to_Run(0, 0);
        tester.dispatchAll();
    } while (tester.component.fpState != FPManagerImpl::IDLE);

}



ExecuteRESP_2::ExecuteRESP_2() :
       STest::Rule<Svc::Tester>("ExecuteRESP_2")
{

}

bool ExecuteRESP_2::precondition(const Tester& tester) {
    return (true);
}

void ExecuteRESP_2::action(Tester& tester) {
    Fw::ComBuffer comBuffer, returnedBuffer;
    Fw::SerializeStatus status;
    Fw::Time time;

    tester.clearHistory();

    // Send Enable RESP_2 response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::RESP_2, RespEnableState::RID_ENABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send Execute RESP_2 response command
    tester.sendCmd_FP_EXECUTE_FAULT_RESPONSE(0, 0, FPResponses::RESP_2);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_EXECUTE_FAULT_RESPONSE);
    
    // Check EVR that RESP_2 has queued
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE(__FILE__, __LINE__, 0, FPResponses::RESP_2);
    tester.clearHistory();

    // Run 1 iteration
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();

    // Check EVR that RESP_2 response has started
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED(__FILE__, __LINE__, 0, FPResponses::RESP_2);
    
        
    // Check command #1 for RESP_2 response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_NO_OP(comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    
        
    // Check command #2 for RESP_2 response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_TEST_CMD_1(1048575, 3.1415, 100,comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    
        
    // Check command #3 for RESP_2 response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_NO_OP(comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    
        
    // Check command #4 for RESP_2 response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_TEST_CMD_1(4222225, 2.045, 101,comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    


    // Check no more RESP_2 commands
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 0);

    // Check EVR that RESP_2 response has completed
     tester.assertEvents_size(__FILE__, __LINE__, 1);
     tester.assertEvents_FP_RESPONSE_COMPLETED_size(__FILE__, __LINE__, 1);
     tester.assertEvents_FP_RESPONSE_COMPLETED(__FILE__, __LINE__, 0, FPResponses::RESP_2);

     // Check telemetry response completion counter
     tester.assertTlm_size(__FILE__, __LINE__, 3);
     tester.assertTlm_FP_RESP_2CompleteCount(__FILE__, __LINE__, 0, 1);
     tester.assertTlm_FP_LastResponseComplete(__FILE__, __LINE__, 0, FPResponses::RESP_2);

}

IndicateRESP_2DroppedDisbled::IndicateRESP_2DroppedDisbled() :
       STest::Rule<Svc::Tester>("IndicateRESP_2DroppedDisbled")
{
    
}

bool IndicateRESP_2DroppedDisbled::precondition(const Tester& tester) {
    return (true);
}

void IndicateRESP_2DroppedDisbled::action(Tester& tester) {

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

    // Send Disable RESP_2 response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::RESP_2, RespEnableState::RID_DISABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send RESP_2 indication
    tester.invoke_to_ResponseIn(0, FPResponses::RESP_2);
    tester.dispatchAll();

    // Check EVR that RESP_2 was dropped because the response is disabled
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_DISABLED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_DISABLED(__FILE__, __LINE__, 0, FPResponses::RESP_2);

    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 2);
    tester.assertTlm_FP_RESP_2AlertCount(__FILE__, __LINE__, 0, 1);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::RESP_2);

}

IndicateRESP_2DroppedQueued::IndicateRESP_2DroppedQueued() :
       STest::Rule<Svc::Tester>("IndicateRESP_2DroppedQueued")
{
    
}

bool IndicateRESP_2DroppedQueued::precondition(const Tester& tester) {
    return (true);
}

void IndicateRESP_2DroppedQueued::action(Tester& tester) {

    tester.clearHistory();

    // Send clear command to clear all the FP counters
    tester.sendCmd_FP_RESET_COUNTERS(0, 0);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESET_COUNTERS);
    tester.clearHistory();


    // Disable all the responses
     tester.sendCmd_FP_ALL_RESPONSE_IDS_ON_OFF(0, 0, RespAllEnableState::RID_DISABLE_ALL);
     tester.dispatchAll();
     tester.checkCmdOK(tester.component.OPCODE_FP_ALL_RESPONSE_IDS_ON_OFF);
     tester.clearHistory();

    // Send Enable RESP_2 response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::RESP_2, RespEnableState::RID_ENABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send RESP_2 indication
    tester.invoke_to_ResponseIn(0, FPResponses::RESP_2);
    tester.dispatchAll();

    // Check EVR that RESP_2 was placed in the queue
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE(__FILE__, __LINE__, 0, FPResponses::RESP_2);

    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 2);
    tester.assertTlm_FP_RESP_2AlertCount(__FILE__, __LINE__, 0, 1);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::RESP_2);

    tester.clearHistory();

    // Send RESP_2 indication again
    tester.invoke_to_ResponseIn(0, FPResponses::RESP_2);
    tester.dispatchAll();

    // Check EVR that RESP_2 was dropped because the response is already queued
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED(__FILE__, __LINE__, 0, FPResponses::RESP_2);

    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 2);
    tester.assertTlm_FP_RESP_2AlertCount(__FILE__, __LINE__, 0, 2);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::RESP_2);
    
    // Flush the queue and bring all responses back to quiescent state
    do {
        tester.invoke_to_Run(0, 0);
        tester.dispatchAll();
    } while (tester.component.fpState != FPManagerImpl::IDLE);

}

IndicateRESP_2DroppedExecuting::IndicateRESP_2DroppedExecuting() :
       STest::Rule<Svc::Tester>("IndicateRESP_2DroppedExecuting")
{
    
}

bool IndicateRESP_2DroppedExecuting::precondition(const Tester& tester) {
    return (true);
}

void IndicateRESP_2DroppedExecuting::action(Tester& tester) {

    tester.clearHistory();

    // Send clear command to clear all the FP counters
    tester.sendCmd_FP_RESET_COUNTERS(0, 0);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESET_COUNTERS);
    tester.clearHistory();


    // Disable all the responses
     tester.sendCmd_FP_ALL_RESPONSE_IDS_ON_OFF(0, 0, RespAllEnableState::RID_DISABLE_ALL);
     tester.dispatchAll();
     tester.checkCmdOK(tester.component.OPCODE_FP_ALL_RESPONSE_IDS_ON_OFF);
     tester.clearHistory();

    // Send Enable RESP_2 response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::RESP_2, RespEnableState::RID_ENABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send RESP_2 indication
    tester.invoke_to_ResponseIn(0, FPResponses::RESP_2);
    tester.dispatchAll();
    
    // Check EVR that RESP_2 response has queued
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE(__FILE__, __LINE__, 0, FPResponses::RESP_2);

    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();

    // Check EVR that RESP_2 response has started
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED(__FILE__, __LINE__, 0, FPResponses::RESP_2);

    // Send RESP_2 indication again
    tester.invoke_to_ResponseIn(0, FPResponses::RESP_2);
    tester.dispatchAll();

    // Check EVR that RESP_2 was dropped because the response is running (same EVR as if it was queued)
    tester.assertEvents_size(__FILE__, __LINE__, 2);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED(__FILE__, __LINE__, 0, FPResponses::RESP_2);


    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 3);
    // Count should now be 2.
    tester.assertTlm_FP_RESP_2AlertCount(__FILE__, __LINE__, 0, 2);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::RESP_2);
    
    // Flush the queue and bring all responses back to quiescent state
    do {
        tester.invoke_to_Run(0, 0);
        tester.dispatchAll();
    } while (tester.component.fpState != FPManagerImpl::IDLE);

}



ExecuteSAFING::ExecuteSAFING() :
       STest::Rule<Svc::Tester>("ExecuteSAFING")
{

}

bool ExecuteSAFING::precondition(const Tester& tester) {
    return (true);
}

void ExecuteSAFING::action(Tester& tester) {
    Fw::ComBuffer comBuffer, returnedBuffer;
    Fw::SerializeStatus status;
    Fw::Time time;

    tester.clearHistory();

    // Send Enable SAFING response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::SAFING, RespEnableState::RID_ENABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send Execute SAFING response command
    tester.sendCmd_FP_EXECUTE_FAULT_RESPONSE(0, 0, FPResponses::SAFING);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_EXECUTE_FAULT_RESPONSE);
    
    // Check EVR that SAFING has queued
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE(__FILE__, __LINE__, 0, FPResponses::SAFING);
    tester.clearHistory();

    // Run 1 iteration
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();

    // Check EVR that SAFING response has started
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED(__FILE__, __LINE__, 0, FPResponses::SAFING);
    
        
    // Check command #1 for SAFING response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_NO_OP(comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    
        
    // Check command #2 for SAFING response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_TEST_CMD_1(1048575, 3.1415, 100,comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    
        
    // Check command #3 for SAFING response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_NO_OP(comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    
        
    // Check command #4 for SAFING response
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 1);
    returnedBuffer = tester.fromPortHistory_comCmdOut->at(0).data;
    
    
    
    
    status = CommandSerializer::CMD_TEST_CMD_1(4222225, 2.045, 101,comBuffer);  
    FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
    ASSERT_EQ(comBuffer, returnedBuffer);
    


    // Check no more SAFING commands
    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();
    ASSERT_EQ(tester.fromPortHistory_comCmdOut->size(), 0);

    // Check EVR that SAFING response has completed
     tester.assertEvents_size(__FILE__, __LINE__, 1);
     tester.assertEvents_FP_RESPONSE_COMPLETED_size(__FILE__, __LINE__, 1);
     tester.assertEvents_FP_RESPONSE_COMPLETED(__FILE__, __LINE__, 0, FPResponses::SAFING);

     // Check telemetry response completion counter
     tester.assertTlm_size(__FILE__, __LINE__, 3);
     tester.assertTlm_FP_SAFINGCompleteCount(__FILE__, __LINE__, 0, 1);
     tester.assertTlm_FP_LastResponseComplete(__FILE__, __LINE__, 0, FPResponses::SAFING);

}

IndicateSAFINGDroppedDisbled::IndicateSAFINGDroppedDisbled() :
       STest::Rule<Svc::Tester>("IndicateSAFINGDroppedDisbled")
{
    
}

bool IndicateSAFINGDroppedDisbled::precondition(const Tester& tester) {
    return (true);
}

void IndicateSAFINGDroppedDisbled::action(Tester& tester) {

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

    // Send Disable SAFING response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::SAFING, RespEnableState::RID_DISABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send SAFING indication
    tester.invoke_to_ResponseIn(0, FPResponses::SAFING);
    tester.dispatchAll();

    // Check EVR that SAFING was dropped because the response is disabled
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_DISABLED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_DISABLED(__FILE__, __LINE__, 0, FPResponses::SAFING);

    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 2);
    tester.assertTlm_FP_SAFINGAlertCount(__FILE__, __LINE__, 0, 1);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::SAFING);

}

IndicateSAFINGDroppedQueued::IndicateSAFINGDroppedQueued() :
       STest::Rule<Svc::Tester>("IndicateSAFINGDroppedQueued")
{
    
}

bool IndicateSAFINGDroppedQueued::precondition(const Tester& tester) {
    return (true);
}

void IndicateSAFINGDroppedQueued::action(Tester& tester) {

    tester.clearHistory();

    // Send clear command to clear all the FP counters
    tester.sendCmd_FP_RESET_COUNTERS(0, 0);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESET_COUNTERS);
    tester.clearHistory();


    // Disable all the responses
     tester.sendCmd_FP_ALL_RESPONSE_IDS_ON_OFF(0, 0, RespAllEnableState::RID_DISABLE_ALL);
     tester.dispatchAll();
     tester.checkCmdOK(tester.component.OPCODE_FP_ALL_RESPONSE_IDS_ON_OFF);
     tester.clearHistory();

    // Send Enable SAFING response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::SAFING, RespEnableState::RID_ENABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send SAFING indication
    tester.invoke_to_ResponseIn(0, FPResponses::SAFING);
    tester.dispatchAll();

    // Check EVR that SAFING was placed in the queue
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE(__FILE__, __LINE__, 0, FPResponses::SAFING);

    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 2);
    tester.assertTlm_FP_SAFINGAlertCount(__FILE__, __LINE__, 0, 1);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::SAFING);

    tester.clearHistory();

    // Send SAFING indication again
    tester.invoke_to_ResponseIn(0, FPResponses::SAFING);
    tester.dispatchAll();

    // Check EVR that SAFING was dropped because the response is already queued
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED(__FILE__, __LINE__, 0, FPResponses::SAFING);

    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 2);
    tester.assertTlm_FP_SAFINGAlertCount(__FILE__, __LINE__, 0, 2);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::SAFING);
    
    // Flush the queue and bring all responses back to quiescent state
    do {
        tester.invoke_to_Run(0, 0);
        tester.dispatchAll();
    } while (tester.component.fpState != FPManagerImpl::IDLE);

}

IndicateSAFINGDroppedExecuting::IndicateSAFINGDroppedExecuting() :
       STest::Rule<Svc::Tester>("IndicateSAFINGDroppedExecuting")
{
    
}

bool IndicateSAFINGDroppedExecuting::precondition(const Tester& tester) {
    return (true);
}

void IndicateSAFINGDroppedExecuting::action(Tester& tester) {

    tester.clearHistory();

    // Send clear command to clear all the FP counters
    tester.sendCmd_FP_RESET_COUNTERS(0, 0);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESET_COUNTERS);
    tester.clearHistory();


    // Disable all the responses
     tester.sendCmd_FP_ALL_RESPONSE_IDS_ON_OFF(0, 0, RespAllEnableState::RID_DISABLE_ALL);
     tester.dispatchAll();
     tester.checkCmdOK(tester.component.OPCODE_FP_ALL_RESPONSE_IDS_ON_OFF);
     tester.clearHistory();

    // Send Enable SAFING response command
    tester.sendCmd_FP_RESPONSE_ID_ON_OFF(0, 0, FPResponses::SAFING, RespEnableState::RID_ENABLE);
    tester.dispatchAll();
    tester.checkCmdOK(tester.component.OPCODE_FP_RESPONSE_ID_ON_OFF);
    tester.clearHistory();

    // Send SAFING indication
    tester.invoke_to_ResponseIn(0, FPResponses::SAFING);
    tester.dispatchAll();
    
    // Check EVR that SAFING response has queued
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_IN_QUEUE(__FILE__, __LINE__, 0, FPResponses::SAFING);

    tester.clearHistory();
    tester.invoke_to_Run(0, 0);
    tester.dispatchAll();

    // Check EVR that SAFING response has started
    tester.assertEvents_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_STARTED(__FILE__, __LINE__, 0, FPResponses::SAFING);

    // Send SAFING indication again
    tester.invoke_to_ResponseIn(0, FPResponses::SAFING);
    tester.dispatchAll();

    // Check EVR that SAFING was dropped because the response is running (same EVR as if it was queued)
    tester.assertEvents_size(__FILE__, __LINE__, 2);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED_size(__FILE__, __LINE__, 1);
    tester.assertEvents_FP_RESPONSE_DROPPED_QUEUED(__FILE__, __LINE__, 0, FPResponses::SAFING);


    // Check telemetry response indication counter
    tester.assertTlm_size(__FILE__, __LINE__, 3);
    // Count should now be 2.
    tester.assertTlm_FP_SAFINGAlertCount(__FILE__, __LINE__, 0, 2);
    tester.assertTlm_FP_LastResponseAlert(__FILE__, __LINE__, 0, FPResponses::SAFING);
    
    // Flush the queue and bring all responses back to quiescent state
    do {
        tester.invoke_to_Run(0, 0);
        tester.dispatchAll();
    } while (tester.component.fpState != FPManagerImpl::IDLE);

}



}
