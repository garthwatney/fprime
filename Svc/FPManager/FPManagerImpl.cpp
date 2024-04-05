/**
 * \file FPManagerImpl.cpp
 * \author Garth Watney
 * \brief FPManagerImpl component implementation
 *
 *
 * \copyright
 * Copyright 2009-2015, by the California Institute of Technology.
 * ALL RIGHTS RESERVED.  United States Government Sponsorship
 * acknowledged. Any commercial use must be negotiated with the Office
 * of Technology Transfer at the California Institute of Technology.
 * <br /><br />
 * This software may be subject to U.S. export control laws and
 * regulations.  By accepting this document, the user agrees to comply
 * with all U.S. export laws and regulations.  User has the
 * responsibility to obtain export licenses, or other export authority
 * as may be required before exporting such information to foreign
 * countries or providing access to foreign persons.
 */

#include <Svc/FPManager/FPManagerImpl.hpp>
#include <Fw/Types/BasicTypes.hpp>
#include <Fw/Types/Assert.hpp>
#include <Fw/Com/ComBuffer.hpp>
#include <stdio.h>

namespace Svc {

Os::BufferQueue FPManagerImpl::respPendingQue;


FPManagerImpl::FPManagerImpl(const char* compName) :
                FPManagerComponentBase(compName),
                fpState(IDLE),
                saveDroppedMessages(0),
                responsePackedState(0),
                autoThrottleEvrClrCtr(0),
                currentResponse(0)

{

}


void FPManagerImpl::init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance) {
    FPManagerComponentBase::init(queueDepth);

    U8 responseId;
    bool enabled;

    // Enable/Disable all the responses according to the defaults in the table
    for (NATIVE_INT_TYPE i=0; i<FPResponses::NUMBER_RESPONSES; i++) {

        responseId = this->responseTable[i].responseId;
        if (this->responseTable[i].enabled == RespEnableState::RID_ENABLE) {
            enabled = true;
        } else {
            enabled = false;
        }

        NATIVE_INT_TYPE priority = this->responseTable[i].priority;

        this->responseTable[i].response->init(responseId, enabled, priority);

    }

    // Create the priority pending response queue
    bool qRes = this->respPendingQue.create(FPResponses::NUMBER_RESPONSES, sizeof(U8));
    FW_ASSERT(qRes);
    FW_ASSERT(this->respPendingQue.isEmpty());

    // Send out the response enable/disable state telemetry
    packRespStateTelemetry();


}


FPManagerImpl::~FPManagerImpl(void) {
}


void FPManagerImpl::ResponseIn_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        const U32 fault //!< Unique identifier for a fault condition
) {

    // Handle an incoming Response indication by placing the response ID
    // in the response pending queue - provided it's not already pending.

    // Ensure the Response identification is within the expected range.
    FW_ASSERT( (fault <= FPResponses::NUMBER_RESPONSES) && (fault > 0));

    // Get the response pointer off the response table
    Response *respPtr = this->responseTable[fault-1].response;

    // Alert the response that it has been notified
    respPtr->alert();

    // Push out telemetry indicator
    FW_ASSERT(tlmResponseIndicators[fault-1] != NULL);
    (this->*(tlmResponseIndicators[fault-1]))(respPtr->getNumberAlerts(), Fw::Time());

    tlmWrite_FP_LastResponseAlert(fault);

    // Check if the response is disabled
    if ( !respPtr->isEnabled() ) {
        log_WARNING_HI_FP_RESPONSE_DROPPED_DISABLED(fault);
        return;
    }

    // Check if the response is pending
    if ( respPtr->isPending() ) {
        log_WARNING_HI_FP_RESPONSE_DROPPED_QUEUED(fault);
        return;
    }

    // Push the Response onto the pending response queue
    respPtr->queit();
    log_WARNING_HI_FP_RESPONSE_IN_QUEUE(fault);


}


void FPManagerImpl::Run_handler(
    NATIVE_INT_TYPE portNum, /*!< The port number*/
    NATIVE_UINT_TYPE context /*!< optional argument */
) {
    // This Run handler is invoked at 10 Hz.
    // Check the state of the component and either do nothing or
    // pull a response off the queue or keep pushing out commands
    // from a currently executing response.

    // Push out periodic telemetry
    tlmWrite_FP_ResponsePackedState(this->responsePackedState);

    this->autoThrottleEvrClrCtr++;

    // Periodically automatically perform an EVR Throttle Clear so new warnings will be masked by events that happened in the past
    // and have been forgotten
    if ((this->autoThrottleEvrClrCtr % AUTO_CLR_EVR_RATE) == 0) {
        log_WARNING_HI_FP_RESPONSES_DROPPED_ThrottleClear();
    }

    // But first check if there are any more dropped messages
    U32 droppedMessages = this->getNumMsgsDropped();

    if (droppedMessages > this->saveDroppedMessages) {
        this->saveDroppedMessages = droppedMessages;
        log_WARNING_HI_FP_RESPONSES_DROPPED(droppedMessages);
    }

    if ( this->fpState == IDLE) {
        U8 response;
        // Pop from the pending response queue
        NATIVE_UINT_TYPE size = sizeof(response);
        NATIVE_INT_TYPE priority;


        // If the response pending queue is not empty then pop the queue and start a response
        if (!this->respPendingQue.isEmpty()) {
            bool qRes = respPendingQue.pop(reinterpret_cast<U8*>(&response), size, priority);
            FW_ASSERT(qRes);
        } else {
            return;
        }

        FW_ASSERT( (response <= FPResponses::NUMBER_RESPONSES) && (response > 0), response);

        // Set the current response
        this->currentResponse = this->responseTable[response-1].response;

        // Start the response
        this->currentResponse->start();
        log_WARNING_HI_FP_RESPONSE_STARTED(response);


        this->fpState = RUNNING;

    } else {
        // fpState is RUNNING
        // Pull the next command from the currently executing response command list
        // Dispatch the command to the output comCmdOut port
        // If there are no more commands to be dispatched for this response then update the last response
        // value, update the response counter and generate an EVR indicating this response has completed
        // execution, then return back command response OK via the CmdStatus output port.  Set the fpState
        // to Idle.

        Fw::ComBuffer comBuffer;

        FW_ASSERT(this->currentResponse != 0);

        if ( this->currentResponse->run(comBuffer) ) {
            comCmdOut_out(0, comBuffer, 0);
        } else {
            // Response has completed
            U32 id = this->currentResponse->getId();
            log_WARNING_HI_FP_RESPONSE_COMPLETED(id);

            // Push out telemetry completion
            FW_ASSERT( (id <= FPResponses::NUMBER_RESPONSES) && (id > 0), id);
            FW_ASSERT(tlmResponseComplete[id-1] != NULL);
            (this->*(tlmResponseComplete[id-1]))(this->currentResponse->getNumberCompletes(), Fw::Time());

            tlmWrite_FP_LastResponseComplete(id);


            this->fpState = IDLE;
            this->currentResponse = 0;

        }
    }
}

void FPManagerImpl::cmdResponseIn_handler(
        NATIVE_INT_TYPE portNum, //!< The port number
        FwOpcodeType opCode, //!< Command Op Code
        U32 cmdSeq,
        const Fw::CmdResponse &response //!< Return status of command send out
) {

    if (response == Fw::CmdResponse::OK) {
        return;
    }

    log_WARNING_HI_FP_BAD_RESPONSE(opCode);

}


void FPManagerImpl::FP_RESPONSE_ID_ON_OFF_cmdHandler(
        FwOpcodeType opCode,   //!< The opcode
        U32 cmdSeq,            //!< The command sequence number
        U32 response,           //!< FP monitors to enable/disable
        RespEnableState fpm_response_id_on_off //!< Disable/Enables
) {	

    // Check that the input argument is in the correct range
    if ((response > FPResponses::NUMBER_RESPONSES) || (response == 0)) {
        log_WARNING_HI_FP_BAD_COMMAND_ARGUMENT(response);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::VALIDATION_ERROR);
        return;
    }

    // Get the response pointer off the response table
    Response *respPtr = this->responseTable[response-1].response;

    // Set the response to either enable or disable
    if (fpm_response_id_on_off == RespEnableState::RID_ENABLE) {
        respPtr->setEnabled(true);
    } else {
        respPtr->setEnabled(false);
    }

    // Send out packed response enable/disable telemetry
    packRespStateTelemetry();

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);

}

void FPManagerImpl::FP_ALL_RESPONSE_IDS_ON_OFF_cmdHandler(
    FwOpcodeType opCode, //!< The opcode
    U32 cmdSeq, //!< The command sequence number
    RespAllEnableState fpm_all_response_ids_on_off //!< Disables or Enables all fault ids
) {

    bool enable;

    // Set all the responses to either enable or disable
    if (fpm_all_response_ids_on_off == RespAllEnableState::RID_ENABLE_ALL) {
        enable = true;
    } else {
        enable = false;
    }

    for (NATIVE_INT_TYPE i = 0; i < FPResponses::NUMBER_RESPONSES; i++) {
        this->responseTable[i].response->setEnabled(enable);
    }

    // Send out packed response enable/disable telemetry
    packRespStateTelemetry();

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}



void FPManagerImpl::FP_EXECUTE_FAULT_RESPONSE_cmdHandler(
        FwOpcodeType opCode, //!< The opcode
        U32 cmdSeq, //!< The command sequence number
        U32 response //!< Response to affect
) {

    // Check that the input argument is in the correct range
    if ((response > FPResponses::NUMBER_RESPONSES) || (response == 0)) {
        log_WARNING_HI_FP_BAD_COMMAND_ARGUMENT(response);
        this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::VALIDATION_ERROR);
        return;
    }

    // Check if the response is pending
    if ( this->responseTable[response-1].response->isPending() ) {
        log_WARNING_HI_FP_RESPONSE_DROPPED_QUEUED(response);
    } else {
        // Set the current response
        this->currentResponse = this->responseTable[response-1].response;

        // Queue the response
        this->currentResponse->queit();
        log_WARNING_HI_FP_RESPONSE_IN_QUEUE(response);


    }

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}



void FPManagerImpl::FP_RESET_COUNTERS_cmdHandler(
    FwOpcodeType opCode, //!< The opcode
    U32 cmdSeq //!< The command sequence number
) {

    // Reset all the responses indicators and completion counters
    for (NATIVE_UINT_TYPE i = 0; i < FPResponses::NUMBER_RESPONSES; i++) {
        responseTable[i].response->clear();
        (this->*(tlmResponseIndicators[i]))(0, Fw::Time());
        (this->*(tlmResponseComplete[i]))(0, Fw::Time());
    }

    // Reset the last response completed
    tlmWrite_FP_LastResponseComplete(0);

    // Reset the last response indicated
    tlmWrite_FP_LastResponseAlert(0);

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}


void FPManagerImpl::FP_RESET_EVR_THROTTLING_cmdHandler(
        FwOpcodeType opCode, //!< The opcode
        U32 cmdSeq //!< The command sequence number
) {
    log_WARNING_HI_FP_RESPONSES_DROPPED_ThrottleClear();
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}


void FPManagerImpl::pingIn_handler(
      const NATIVE_INT_TYPE portNum,
      U32 key
  )
{
    this->pingOut_out(0,key);
}


void FPManagerImpl::packRespStateTelemetry(void) {
    // Check that we don't exceed the number of bits in the telemetry channel
    FW_ASSERT(FPResponses::NUMBER_RESPONSES <= 32);

    U32 tlmChanVal = 0;

    for (NATIVE_UINT_TYPE i = 0; i < FPResponses::NUMBER_RESPONSES; i++) {
            tlmChanVal = tlmChanVal << 1;  // Make space for the next response state
            if (responseTable[i].response->isEnabled()) {
                tlmChanVal = tlmChanVal | 1;
            }
    }

    // Push out the telemety channel
    tlmWrite_FP_ResponsePackedState(tlmChanVal);

    // Save the telemetry channel to be pushed out periodically
    this->responsePackedState = tlmChanVal;

}

}
