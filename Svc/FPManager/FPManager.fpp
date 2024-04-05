module Svc {

  enum RespEnableState {
    RID_DISABLE
    RID_ENABLE
  }

  enum RespAllEnableState { 
        RID_DISABLE_ALL
        RID_ENABLE_ALL
  }

  active component FPManager {

    # ----------------------------------------------------------------------
    # General ports
    # ----------------------------------------------------------------------
    async input port Run: Svc.Sched drop

    # Receives the Response indicator
    async input port ResponseIn: Svc.FP drop

    # Send out commands from a Response
    output port comCmdOut: Fw.Com

    # Receive back command response
    async input port cmdResponseIn: Fw.CmdResponse drop

    async input port pingIn: Svc.Ping

    output port pingOut: Svc.Ping


    # ----------------------------------------------------------------------
    # Special ports
    # ----------------------------------------------------------------------
    command recv port CmdDisp

    command reg port CmdReg

    command resp port CmdStatus

    time get port Time

    event port Log

    text event port LogText

    telemetry port Tlm


    # ----------------------------------------------------------------------
    # Commands 
    # ----------------------------------------------------------------------
    @ Enable or Disable a single Response
    async command FP_RESPONSE_ID_ON_OFF(
          response: U32 @< Fault Protection Response Id to enable/disable
          fpm_response_id_on_off: RespEnableState
    ) opcode 0

    @ Enable or Disable all the Responses
    async command FP_ALL_RESPONSE_IDS_ON_OFF(
          fpm_all_response_ids_on_off: RespAllEnableState
    ) opcode 1

    @ Execute Fault Response
    async command FP_EXECUTE_FAULT_RESPONSE(
          response: U32 @< Fault Protection Fault Response to execute
    ) opcode 2

    @ Reset all the Fault Protection counters
    async command FP_RESET_COUNTERS(
    ) opcode 3

    @ Command to disable throttling and resume normal evr activity
    async command FP_RESET_EVR_THROTTLING(
    ) opcode 4


    # ----------------------------------------------------------------------
    # Events 
    # ----------------------------------------------------------------------
    @ The Response indication is dropped because the Response is disabled 
    event FP_RESPONSE_DROPPED_DISABLED(
          response: U32 @< The response id number
    ) \
    severity warning high \
    id 0 \
    format "Response indication {} dropped because it's disabled"

    @ The Response indication is dropped because already queued
    event FP_RESPONSE_DROPPED_QUEUED(
          response: U32 @< The response id number
    ) \
    severity warning high \
    id 1 \
    format "Response indication {} dropped because it's already queued"

    @ The Response is placed in the execution queue
    event FP_RESPONSE_IN_QUEUE(
          response: U32 @< The response id number
    ) \
    severity warning high \
    id 2 \
    format "Response {} placed in execution queue"

    @ The Response has started execution
    event FP_RESPONSE_STARTED(
      response: U32
      ) \
     severity warning high \
     id 3 \
     format "Response {} started execution"

    @ The Response has completed execution
    event FP_RESPONSE_COMPLETED(
      response: U32
      ) \
     severity warning high \
     id 4 \
     format "Response {} completed execution"

    @ The Response indication has dropped due to component input queue being full
    event FP_RESPONSES_DROPPED(
      numberDropped: U32
      ) \
     severity warning high \
     id 5 \
     format "Response indication dropped due to component queue full: {}" \
     throttle 10

    @ Bad argument passed into command.
    event FP_BAD_COMMAND_ARGUMENT(
      response: U32
      ) \
     severity warning high \
     id 6 \
     format "Bad FP command argument {}"

    @ Received a bad response after a Response sent out a command
    event FP_BAD_RESPONSE(
      $opcode: U32
      ) \
     severity warning high \
     id 7 \
     format "FP response command returned NOT OK for Opcode {}"

    # ----------------------------------------------------------------------
    # Telemetry 
    # ----------------------------------------------------------------------
    include "TlmDict.fppi"

  }
}
