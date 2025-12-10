# Svc::BufferManager

Component is passive

## 1 Introduction
['A component for managing memory buffers']

![](ContextDiagram.png)

*DataTake Context Diagram*


## 2 Requirements
Requirement | Description | Rationale 
-- | -- | -- 
1 |  The BufferManager shall do great things<br> |  Because great is good<br> 


## 3 Design

### 3.1 General ports
Port name | Type | Kind | Priority | Array | queue full | Description 
-- | -- | -- | -- | -- | -- | -- 
schedIn | Svc::Sched | :red_circle: sync input | None | 1 | assert | Schedule input port
bufferGetCallee | Fw::BufferGet | guarded input | None | 1 | assert | Mutex locked Buffer callee input port
bufferSendIn | Fw::BufferSend | guarded input | None | 1 | assert | Mutex locked Buffer send in input port



### 3.2 Special ports
Port name | Kind | Priority | queue full | Description 
-- | -- | -- | -- | -- 
timeCaller | time get | None | assert | Port for getting the time
textEventOut | text event | None | assert | Port for emitting text events
eventOut | event | None | assert | Port for emitting events
tlmOut | telemetry | None | assert | Port for emitting Telemetry



### 3.3 Class structure
![](ClassDiagram.png)

*Class Diagram*

### 3.4 State
![](StateMachineDiagram.png)

### 3.5 Sequences

## 4 Dictionary

### 4.1 Enumerations
Enum | Values 
-- | -- 



### 4.2 Command Dictionary
Command | sync/async | queue full | Args | Description 
-- | -- | -- | -- | -- 



### 4.3 Telemetry Dictionary
Telemetry | Type | Update | Description 
-- | -- | -- | -- 
HiBuffs | U32 | on change | The high water mark of allocated buffers
NoBuffs | U32 | on change | The number of requests that couldn't return a buffer
CurrBuffs | U32 | on change | The current number of allocated buffers
EmptyBuffs | U32 | on change | The number of empty buffers returned
TotalBuffs | U32 | on change | The total buffers allocated



### 4.4 EVR Dictionary
EVR | Severity | Args | Description 
-- | -- | -- | -- 
NoBuffsAvailable | warning high | size: FwSizeType<br /> | The BufferManager was unable to allocate a requested buffer
NullEmptyBuffer | warning high |  | The buffer manager received a null pointer and zero-sized buffer as a return. Probably undetected failed buffer allocation



## 5 Checklist

## 6 Unit Testing

