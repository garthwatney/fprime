/**
 * \file Response.hpp
 * \author Garth Watney
 * \brief Response header base class for all response instances
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
#ifndef _RESPONSE_HPP
#define _RESPONSE_HPP

#include <Fw/Types/BasicTypes.hpp>
#include "Fw/Com/ComBuffer.hpp"


namespace Svc {

//! class Response
class Response {

public:

    //! Class constructor
    //!
    Response();

    //! Class destructor
    //!
    virtual ~Response();

    //! Pure virtual function
    //! Each response instance will implement it's own run function.
    //!
    //! Return true if there is a command, else return False
    //! The serialized command is in the returned comBuffer
    //!
    virtual bool run(Fw::ComBuffer& comBuffer) = 0;

    //! Initialization
    //! Sets the id, enabled or disabled state and the priority of the response
    void init(U8 response, bool enabled, NATIVE_INT_TYPE priority);

    //! Set the response to enable or disable
    //!
    void setEnabled(bool enabled);

    //! Return true if the response is either executing or on the queue,
    //! else return false.
    //!
    bool isPending();

    //! Return true if the response is enabled,
    //! else return false.
    //!
    bool isEnabled();

    //! Return the number of times this response has been alerted
    //!
    U32 getNumberAlerts();

    //! Return the number times this response has completed execution
    //!
    U32 getNumberCompletes();

    //! Return the response ID
    //!
    U8 getId();

    //! Clear the response counters
    //!
    void clear();

    //! Alerts the response
    //!
    void alert();

    //! Start executing a response
    //!
    void start();

    //! Queue a response on the priority response pending queue
    void queit();

PROTECTED:

    //! Number of times this response has completed execution
    //!
    U32 numberCompletes;

    //! The response ID
    //!
    U8 response;

    //! Executing state of the response
    //!
    bool executing;

    //! Queued state of the response
    //!
    bool queued;

    //! Enabled state of the response
    //!
    bool enabled;

    //! Priority of the response
    //!
    NATIVE_INT_TYPE priority;

    //! Number of times this response has been alerted
    //!
    U32 numberAlerts;

};

}

#endif
