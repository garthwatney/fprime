module Svc {

    struct SMEvents {
        smId : U32
        eventSignal: U32
        payload: [128] U8
    }

}
