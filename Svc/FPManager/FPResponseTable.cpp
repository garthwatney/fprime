/**
 * \file FPResponseTable.cpp
 * \author Auto-coded
 * \brief Instantiations of all the responses.
 * and their priorities and initial state (enable/disable).
 * In addition, tables for telemetry generation functions are auto-coded
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
#include <Svc/FPManager/RESP_1.hpp>
#include <Svc/FPManager/RESP_2.hpp>
#include <Svc/FPManager/SAFING.hpp>



namespace Svc {

RESP_1 resp_1;
RESP_2 resp_2;
SAFING safing;

const FPManagerImpl::ResponseTableType FPManagerImpl::responseTable[FPResponses::NUMBER_RESPONSES] = {
        { FPResponses::RESP_1, &resp_1, RespEnableState::RID_ENABLE, LOW_PRIORITY },
        { FPResponses::RESP_2, &resp_2, RespEnableState::RID_ENABLE, HIGH_PRIORITY },
        { FPResponses::SAFING, &safing, RespEnableState::RID_ENABLE, LOW_PRIORITY },
};


const FPManagerImpl::TlmWrite_FuncPtr1 FPManagerImpl::tlmResponseIndicators[FPResponses::NUMBER_RESPONSES] = {
    &FPManagerImpl::tlmWrite_FP_RESP_1AlertCount,
    &FPManagerImpl::tlmWrite_FP_RESP_2AlertCount,
    &FPManagerImpl::tlmWrite_FP_SAFINGAlertCount,
};

const FPManagerImpl::TlmWrite_FuncPtr1 FPManagerImpl::tlmResponseComplete[FPResponses::NUMBER_RESPONSES] = {
    &FPManagerImpl::tlmWrite_FP_RESP_1CompleteCount,
    &FPManagerImpl::tlmWrite_FP_RESP_2CompleteCount,
    &FPManagerImpl::tlmWrite_FP_SAFINGCompleteCount,
};

}
//
