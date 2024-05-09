     @ Total number of times RESP_1 was alerted
     telemetry FP_RESP_1AlertCount: U16 id 0

     @ Total number of times RESP_2 was alerted
     telemetry FP_RESP_2AlertCount: U16 id 1

     @ Total number of times SAFING was alerted
     telemetry FP_SAFINGAlertCount: U16 id 2


     @ Total numpber of times RESP_1 was completed
     telemetry FP_RESP_1CompleteCount: U16 id 3

     @ Total numpber of times RESP_2 was completed
     telemetry FP_RESP_2CompleteCount: U16 id 4

     @ Total numpber of times SAFING was completed
     telemetry FP_SAFINGCompleteCount: U16 id 5


    @ The last response that was alerted
    telemetry FP_LastResponseAlert: U32 id 6

    
    @ The last response that was completed
    telemetry FP_LastResponseComplete: U32 id 7


    @ The enable/disable state of each response packed into a word
    telemetry FP_ResponsePackedState: U32 id 8

    