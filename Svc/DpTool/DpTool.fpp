module Svc {

  @ A component for testing  data product code gen
  active component DpTool {

    # ----------------------------------------------------------------------
    # Types
    # ----------------------------------------------------------------------

    enum Status {
        YES
        NO
        MAYBE
    } default MAYBE

    @ Data for a DataRecord
    struct Data {
      @ A U16 field
      u16Field: U16
    }

    struct ArrayData {
      arrField: [10] U16
    }

    @ Data for a ComplexRecord
    struct Complex {
      @ A struct in the struct
      f1: Data
      @ A simple U32 field
      f2: U32
    }

    @ Data for a EnumRecord
    struct EnumStruct {
      status: Status
    }


    # ----------------------------------------------------------------------
    # Special ports
    # ----------------------------------------------------------------------

    @ Data product get port
    product get port productGetOut

    @ Data product request port
    product request port productRequestOut

    @ Data product receive port
    async product recv port productRecvIn

    @ Data product send port
    product send port productSendOut

    @ Time get port
    time get port timeGetOut

    # ----------------------------------------------------------------------
    # General ports
    # ----------------------------------------------------------------------

    @ A schedIn port to run the data product generation
    async input port schedIn: Svc.Sched

    # ----------------------------------------------------------------------
    # Records
    # ----------------------------------------------------------------------

    @ Record 1
    product record U32Record: U32 id 0

    @ Record 2
    product record ComplexRecord: Complex id 1

    @ Record 3
    product record U8ArrayRecord: U8 array id 2

    @ Record 4
    product record U32ArrayRecord: U32 array id 3

    @ Record 5
    product record DataArrayRecord: Data array id 4

    @ Record 6
    product record EnumRecord: EnumStruct id 5

    @ Record 7
    product record F32Record: F32 id 6

    @ Record 8
    product record F64Record: F64 id 7

    @ Record 9
    product record BoolRecord: bool id 8

    @ Record 10
    product record I8Record: I8 id 9

    @ Record 11
    product record I16Record: I16 id 10

    @ Record 12
    product record I32Record: I32 id 11

    @ Record 13
    product record I64Record: I64 id 12

    @ Record 14
    product record FppArrayRecord: ArrayData id 13

    # ----------------------------------------------------------------------
    # Containers
    # ----------------------------------------------------------------------

    @ Container 1
    product container Container1 id 14 default priority 10

  }

}
