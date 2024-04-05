/**
 * \file FPManagerImpl.hpp
 * \author Garth Watney
 * \brief FPManagerImpl component header
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
#ifndef FPMANAGER_IMPL_HPP
#define FPMANAGER_IMPL_HPP

#include <Svc/FPManager/FPManagerComponentAc.hpp>
#include <Fw/Types/BasicTypes.hpp>
#include <Svc/FPManager/FPManagerImplCfg.hpp>
#include <Svc/FPManager/Response.hpp>
#include <Svc/FPManager/FPResponses.hpp>
#include <Os/Pthreads/BufferQueue.hpp>


namespace Svc {

//! class FPManagerImpl
//!
//!

class FPManagerImpl : public FPManagerComponentBase {
public:

    //!  Component constructor
    //!
    FPManagerImpl(const char* compName);

    //!  Component destructor
    //!
    ~FPManagerImpl(void);

    //! Priority levels for the response pending queue
    typedef enum ResponsePriority {
        HIGH_PRIORITY = 2,
        LOW_PRIORITY = 1,
    } ResponsePriority;

    //! State of the FPManager component
    //!
    typedef enum FPState {
        IDLE,
        RUNNING
    } FPState;

    //! Rate in which Throttled EVR's will be automatically cleared
    //!
    typedef enum AutoThrottleClearRate{
        AUTO_CLR_EVR_RATE = 3000 // 5 minutes @ 10Hz
    }AutoThrottleClearRate;	    

    //!  Component Initialization function
    //!
    void init(NATIVE_INT_TYPE queueDepth, NATIVE_INT_TYPE instance);


    //! Priority response pending queue
    //!
    static Os::BufferQueue respPendingQue;

PRIVATE:

    //! Handler for input port FaultIn
    //
    void ResponseIn_handler(
            NATIVE_INT_TYPE portNum, //!< The port number
            const U32 fault //!<  response ID received from components requesting action
    );

    //! Handler for input port Run
    //
    void Run_handler(
        NATIVE_INT_TYPE portNum, /*!< The port number*/
        NATIVE_UINT_TYPE context /*!< The call order*/
    );

    //! cmdResponseIn handler
    //
    void cmdResponseIn_handler(
            NATIVE_INT_TYPE portNum, //!< The port number
            FwOpcodeType opCode, //!< Command Op Code
            U32 cmdSeq, //!< This is used to pass back the 'Context' of the command
            const Fw::CmdResponse &response //!< Indicates the completion status of the command
    );

    //! pingIn handler
    //!
    void pingIn_handler(
        const NATIVE_INT_TYPE portNum, /*!< The port number*/
        U32 key /*!< Value to return to pinger*/
    );


    //! Command handler
    //! Enable or Disable a single response
    //!
    void FP_RESPONSE_ID_ON_OFF_cmdHandler(
            FwOpcodeType opCode, //!< The opcode
            U32 cmdSeq, //!< The command sequence number
            U32 response, //!< fault to affect
            RespEnableState fpm_response_id_on_off //!< Disables or Enables selected fault id
    );

    //! Command handler
    //! Enable or Disable all responses
    //!
    void FP_ALL_RESPONSE_IDS_ON_OFF_cmdHandler(
        FwOpcodeType opCode, //!< The opcode
        U32 cmdSeq, //!< The command sequence number
        RespAllEnableState fpm_all_response_ids_on_off //!< Disables or Enables all fault ids
    );

    //! Command handler
    //! Execute a response
    //!
    void FP_EXECUTE_FAULT_RESPONSE_cmdHandler(
            FwOpcodeType opCode, //!< The opcode
            U32 cmdSeq, //!< The command sequence number
            U32 response //!< Response to affect
    );

    //! Command handler
    //! Reset the counters
    //!
    void FP_RESET_COUNTERS_cmdHandler(
        FwOpcodeType opCode, //!< The opcode
        U32 cmdSeq //!< The command sequence number
    );

    //! Command handler
    //! Reset EVR throttling
    //!
    void FP_RESET_EVR_THROTTLING_cmdHandler(
            FwOpcodeType opCode, //!< The opcode
            U32 cmdSeq //!< The command sequence number
    );

    //! Helper function
    //! Pack response enable/disable state into a telemetry channel
    //!
    void packRespStateTelemetry(void);

    typedef struct ResponseTableType {
         U8 responseId;
         Response* response;
         RespEnableState enabled;
         ResponsePriority priority;
     } ResponseTableType;

     //! Private data declarations

     //! Fault Protection state - running or idle
     FPState fpState;

     //! Keep track of how many times a message is dropped
     //! because the component's input queue is full
     //!
     U32 saveDroppedMessages;

     //! Packed response state telemetry
     U32 responsePackedState;

     //! Counter used for EVR Throttle clearing
     U32 autoThrottleEvrClrCtr;

     //! Static constant table of responses with default state
     //!
     static const ResponseTableType responseTable[FPResponses::NUMBER_RESPONSES];

     //! Pointer to the currently executing response
     //!
     Response* currentResponse;

     //! Telemetry function types
     //!
     typedef void (Svc::FPManagerComponentBase:: *TlmWrite_FuncPtr1)(U16 numberAlerts, Fw::Time);

     //! Table of response indicator telemetry functions
     //!
     static const TlmWrite_FuncPtr1 tlmResponseIndicators[FPResponses::NUMBER_RESPONSES];

     //! Table of response completion telemetry functions
     //!
     static const TlmWrite_FuncPtr1 tlmResponseComplete[FPResponses::NUMBER_RESPONSES];

};

}

#endif
