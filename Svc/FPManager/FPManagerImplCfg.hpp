/*
 * FPManagerImplCfg.hpp
 *
 *  Created on: Feb 17, 2016
 *      Author: Jim Hofman
 */

#ifndef FPMANAGER_FPMANAGERIMPLCFG_HPP_
#define FPMANAGER_FPMANAGERIMPLCFG_HPP_

// User-defined values

enum {
	NEW_FAULT_CONTEXT = 0, // Context sent the FPStateMgr from FaultIn_Handler() indicating a new fault came in.
	RESPONSE_COMPLETE_EVENT = 1, // Sent to the FPStateMgr when a response has completed successfully
	DUPLICATE_ACTION_EVENT = 2, // Sent to the FPStateMgr when a duplicate action was requested
	RESPONSE_ERROR_EVENT = 3, // Sent to the FPStateMgr to indicate that there was a command error
};

#endif /* FPMANAGER_FPMANAGERIMPLCFG_HPP_ */
