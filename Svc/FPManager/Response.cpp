/**
 * \file Response.cpp
 * \author Garth Watney
 * \brief Response implementation base class for all response instances
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

#include <Svc/FPManager/Response.hpp>
#include <Svc/FPManager/FPManagerImpl.hpp>
#include <Fw/Types/Assert.hpp>


namespace Svc {

  Response::Response() :
          numberCompletes(0),
          response(0),
          executing(false),
          queued(false),
          enabled(false),
          priority(0),
          numberAlerts(0)
  {
  }

  Response::~Response() {

  }

  void Response::init(U8 response, bool enabled, NATIVE_INT_TYPE priority) {
      this->response = response;
      this->enabled = enabled;
      this->priority = priority;
      this->queued = false;
      this->executing = false;
      this->numberAlerts = 0;
      this->numberCompletes = 0;
  }

  void Response::setEnabled(bool enabled) {
      this->enabled = enabled;
  }

  bool Response::isPending() {
      return ( this->executing || this->queued );
  }


  bool Response::isEnabled() {
      return this->enabled;
  }

  U32 Response::getNumberAlerts() {
      return this->numberAlerts;
  }

  U32 Response::getNumberCompletes() {
      return this->numberCompletes;
  }

  void Response::clear() {
      this->numberAlerts = 0;
      this->numberCompletes = 0;
  }

  void Response::start() {
      this->queued = false;
      this->executing = true;
  }

  void Response::queit() {
      FW_ASSERT(this->executing == false);

      bool qRes = FPManagerImpl::respPendingQue.push(reinterpret_cast<U8*>(&this->response), sizeof(this->response), this->priority);
      FW_ASSERT(qRes);

      this->queued = true;
  }

  U8 Response::getId() {
     return this->response;
  }

  void Response::alert() {
      this->numberAlerts++;
  }

}
