// ======================================================================
// \title  SmTestComponentAc.cpp
// \author Generated by fpp-to-cpp
// \brief  cpp file for SmTest component base class
// ======================================================================

#include "FppTest/state_machine/SmTestComponent.hpp"
#include "Fw/Types/Assert.hpp"
#include "Fw/Types/ExternalString.hpp"
#if FW_ENABLE_TEXT_LOGGING
#include "Fw/Types/String.hpp"
#endif

namespace FppTest {

  namespace {
    enum MsgTypeEnum {
      SMTEST_COMPONENT_EXIT = Fw::ActiveComponentBase::ACTIVE_COMPONENT_EXIT,
      SCHEDIN_SCHED,
      INT_IF_SENDEVENTS,
    };

    // Get the max size by constructing a union of the async input, command, and
    // internal port serialization sizes
    union BuffUnion {
      BYTE schedInPortSize[Svc::InputSchedPort::SERIALIZED_SIZE];
      // Size of sendEvents argument list
      BYTE sendEventsIntIfSize[
        Svc::SMEvents::SERIALIZED_SIZE
      ];
    };

    // Define a message buffer class large enough to handle all the
    // asynchronous inputs to the component
    class ComponentIpcSerializableBuffer :
      public Fw::SerializeBufferBase
    {

      public:

        enum {
          // Max. message size = size of data + message id + port
          SERIALIZATION_SIZE =
            sizeof(BuffUnion) +
            sizeof(FwEnumStoreType) +
            sizeof(FwIndexType)
        };

        Fw::Serializable::SizeType getBuffCapacity() const {
          return sizeof(m_buff);
        }

        U8* getBuffAddr() {
          return m_buff;
        }

        const U8* getBuffAddr() const {
          return m_buff;
        }

      private:
        // Should be the max of all the input ports serialized sizes...
        U8 m_buff[SERIALIZATION_SIZE];

    };
  }

  // ----------------------------------------------------------------------
  // Component initialization
  // ----------------------------------------------------------------------

  void SmTestComponentBase ::
    init(
        FwQueueSizeType queueDepth,
        FwEnumStoreType instance
    )
  {
    // Initialize base class
    Fw::ActiveComponentBase::init(instance);
    device1.init();
    device2.init();

    // Connect input port schedIn
    for (
      FwIndexType port = 0;
      port < static_cast<FwIndexType>(this->getNum_schedIn_InputPorts());
      port++
    ) {
      this->m_schedIn_InputPort[port].init();
      this->m_schedIn_InputPort[port].addCallComp(
        this,
        m_p_schedIn_in
      );
      this->m_schedIn_InputPort[port].setPortNum(port);

#if FW_OBJECT_NAMES == 1
      Fw::ObjectName portName;
      portName.format(
        "%s_schedIn_InputPort[%" PRI_PlatformIntType "]",
        this->m_objName.toChar(),
        port
      );
      this->m_schedIn_InputPort[port].setObjName(portName.toChar());
#endif
    }

    Os::Queue::QueueStatus qStat = this->createQueue(
      queueDepth,
      ComponentIpcSerializableBuffer::SERIALIZATION_SIZE
    );
    FW_ASSERT(
      Os::Queue::QUEUE_OK == qStat,
      static_cast<FwAssertArgType>(qStat)
    );
  }

  // ----------------------------------------------------------------------
  // Getters for typed input ports
  // ----------------------------------------------------------------------

  Svc::InputSchedPort* SmTestComponentBase ::
    get_schedIn_InputPort(FwIndexType portNum)
  {
    FW_ASSERT(
      portNum < this->getNum_schedIn_InputPorts(),
      static_cast<FwAssertArgType>(portNum)
    );

    return &this->m_schedIn_InputPort[portNum];
  }

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  SmTestComponentBase ::
    SmTestComponentBase(const char* compName) :
      Fw::ActiveComponentBase(compName),
      device1(this),
      device2(this)
  {
    

  }

  SmTestComponentBase ::
    ~SmTestComponentBase()
  {

  }

  // ----------------------------------------------------------------------
  // Getters for numbers of typed input ports
  // ----------------------------------------------------------------------

  FwIndexType SmTestComponentBase ::
    getNum_schedIn_InputPorts() const
  {
    return static_cast<FwIndexType>(FW_NUM_ARRAY_ELEMENTS(this->m_schedIn_InputPort));
  }

  // ----------------------------------------------------------------------
  // Port handler base-class functions for typed input ports
  //
  // Call these functions directly to bypass the corresponding ports
  // ----------------------------------------------------------------------

  void SmTestComponentBase ::
    schedIn_handlerBase(
        FwIndexType portNum,
        U32 context
    )
  {
    // Make sure port number is valid
    FW_ASSERT(
      portNum < this->getNum_schedIn_InputPorts(),
      static_cast<FwAssertArgType>(portNum)
    );

    // Call pre-message hook
    schedIn_preMsgHook(
      portNum,
      context
    );
    ComponentIpcSerializableBuffer msg;
    Fw::SerializeStatus _status = Fw::FW_SERIALIZE_OK;

    // Serialize message ID
    _status = msg.serialize(
      static_cast<FwEnumStoreType>(SCHEDIN_SCHED)
    );
    FW_ASSERT(
      _status == Fw::FW_SERIALIZE_OK,
      static_cast<FwAssertArgType>(_status)
    );

    // Serialize port number
    _status = msg.serialize(portNum);
    FW_ASSERT(
      _status == Fw::FW_SERIALIZE_OK,
      static_cast<FwAssertArgType>(_status)
    );

    // Serialize argument context
    _status = msg.serialize(context);
    FW_ASSERT(
      _status == Fw::FW_SERIALIZE_OK,
      static_cast<FwAssertArgType>(_status)
    );

    // Send message
    Os::Queue::QueueBlocking _block = Os::Queue::QUEUE_NONBLOCKING;
    Os::Queue::QueueStatus qStatus = this->m_queue.send(msg, 0, _block);

    FW_ASSERT(
      qStatus == Os::Queue::QUEUE_OK,
      static_cast<FwAssertArgType>(qStatus)
    );
  }

  // ----------------------------------------------------------------------
  // Pre-message hooks for typed async input ports
  //
  // Each of these functions is invoked just before processing a message
  // on the corresponding port. By default, they do nothing. You can
  // override them to provide specific pre-message behavior.
  // ----------------------------------------------------------------------

  void SmTestComponentBase ::
    schedIn_preMsgHook(
        FwIndexType portNum,
        U32 context
    )
  {
    // Default: no-op
  }

  // ----------------------------------------------------------------------
  // Internal interface base-class functions
  // ----------------------------------------------------------------------

  void SmTestComponentBase ::
    sendEvents_internalInterfaceInvoke(const Svc::SMEvents& ev)
  {
    ComponentIpcSerializableBuffer msg;
    Fw::SerializeStatus _status = Fw::FW_SERIALIZE_OK;

    // Serialize the message ID
    _status = msg.serialize(static_cast<FwEnumStoreType>(INT_IF_SENDEVENTS));
    FW_ASSERT (
      _status == Fw::FW_SERIALIZE_OK,
      static_cast<FwAssertArgType>(_status)
    );

    // Fake port number to make message dequeue work
    _status = msg.serialize(static_cast<FwIndexType>(0));
    FW_ASSERT (
      _status == Fw::FW_SERIALIZE_OK,
      static_cast<FwAssertArgType>(_status)
    );

    _status = msg.serialize(ev);
    FW_ASSERT(
      _status == Fw::FW_SERIALIZE_OK,
      static_cast<FwAssertArgType>(_status)
    );

    // Send message
    Os::Queue::QueueBlocking _block = Os::Queue::QUEUE_NONBLOCKING;
    Os::Queue::QueueStatus qStatus = this->m_queue.send(msg, 0, _block);

    FW_ASSERT(
      qStatus == Os::Queue::QUEUE_OK,
      static_cast<FwAssertArgType>(qStatus)
    );
  }

  // ----------------------------------------------------------------------
  // Message dispatch functions
  // ----------------------------------------------------------------------

  Fw::QueuedComponentBase::MsgDispatchStatus SmTestComponentBase ::
    doDispatch()
  {
    ComponentIpcSerializableBuffer msg;
    FwQueuePriorityType priority = 0;

    Os::Queue::QueueStatus msgStatus = this->m_queue.receive(
      msg,
      priority,
      Os::Queue::QUEUE_BLOCKING
    );
    FW_ASSERT(
      msgStatus == Os::Queue::QUEUE_OK,
      static_cast<FwAssertArgType>(msgStatus)
    );

    // Reset to beginning of buffer
    msg.resetDeser();

    FwEnumStoreType desMsg = 0;
    Fw::SerializeStatus deserStatus = msg.deserialize(desMsg);
    FW_ASSERT(
      deserStatus == Fw::FW_SERIALIZE_OK,
      static_cast<FwAssertArgType>(deserStatus)
    );

    MsgTypeEnum msgType = static_cast<MsgTypeEnum>(desMsg);

    if (msgType == SMTEST_COMPONENT_EXIT) {
      return MSG_DISPATCH_EXIT;
    }

    FwIndexType portNum = 0;
    deserStatus = msg.deserialize(portNum);
    FW_ASSERT(
      deserStatus == Fw::FW_SERIALIZE_OK,
      static_cast<FwAssertArgType>(deserStatus)
    );

    switch (msgType) {
      // Handle async input port schedIn
      case SCHEDIN_SCHED: {
        // Deserialize argument context
        U32 context;
        deserStatus = msg.deserialize(context);
        FW_ASSERT(
          deserStatus == Fw::FW_SERIALIZE_OK,
          static_cast<FwAssertArgType>(deserStatus)
        );
        // Call handler function
        this->schedIn_handler(
          portNum,
          context
        );

        break;
      }

      // Handle internal interface sendEvents
      case INT_IF_SENDEVENTS: {
        Svc::SMEvents ev;
        deserStatus = msg.deserialize(ev);

        // Internal interface should always deserialize
        FW_ASSERT(
          Fw::FW_SERIALIZE_OK == deserStatus,
          static_cast<FwAssertArgType>(deserStatus)
        );

        // Make sure there was no data left over.
        // That means the buffer size was incorrect.
        FW_ASSERT(
          msg.getBuffLeft() == 0,
          static_cast<FwAssertArgType>(msg.getBuffLeft())
        );

        // Call handler function
        this->sendEvents_internalInterfaceHandler(
          ev
        );

        break;
      }

      default:
        return MSG_DISPATCH_ERROR;
    }

    return MSG_DISPATCH_OK;
  }

  // ----------------------------------------------------------------------
  // Calls for messages received on typed input ports
  // ----------------------------------------------------------------------

  void SmTestComponentBase ::
    m_p_schedIn_in(
        Fw::PassiveComponentBase* callComp,
        FwIndexType portNum,
        U32 context
    )
  {
    FW_ASSERT(callComp);
    SmTestComponentBase* compPtr = static_cast<SmTestComponentBase*>(callComp);
    compPtr->schedIn_handlerBase(
      portNum,
      context
    );
  }

void SmTestComponentBase :: 
  sendEvents_internalInterfaceHandler(const Svc::SMEvents& ev)
{
    U16 id = ev.getsmId();
    switch (id) {
                                
        case StateMachine::DEVICE1:
            this->device1.update(&ev);
            break;
        case StateMachine::DEVICE2:
            this->device2.update(&ev);
            break;
        default:
            FW_ASSERT(0);
    }
}
            

}