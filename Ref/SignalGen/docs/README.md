# Ref::SignalGen

Component is queued

## 1 Introduction
['A component for generating periodic signals']

![](ContextDiagram.png)

*DataTake Context Diagram*


## 2 Requirements
Requirement | Description | Rationale 
-- | -- | -- 
1 |  The SignalGen shall do great things<br> |  Because great is good<br> 


## 3 Design

### 3.1 General ports
Port name | Type | Kind | Priority | Array | queue full | Description 
-- | -- | -- | -- | -- | -- | -- 
schedIn | Svc::Sched | :red_circle: sync input | None | 1 | assert | 



### 3.2 Special ports
Port name | Kind | Priority | queue full | Description 
-- | -- | -- | -- | -- 
timeCaller | time get | None | assert | Time get port
productGetOut | product get | None | assert | Data product get port
productRecvIn | product recv | None | assert | Data product receive port
productSendOut | product send | None | assert | Data product send port
productRequestOut | product request | None | assert | Data product request port
logOut | event | None | assert | Event port
cmdRegOut | command reg | None | assert | Command registration port
logTextOut | text event | None | assert | Text event port
cmdIn | command recv | None | assert | Command received port
tlmOut | telemetry | None | assert | Telemetry port
cmdResponseOut | command resp | None | assert | Command response port



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
DpReqType | IMMEDIATE<br />ASYNC<br



### 4.2 Command Dictionary
Command | sync/async | queue full | Args | Description 
-- | -- | -- | -- | -- 
Settings | :large_blue_circle: async | assert | Frequency: U32<br />Amplitude: F32<br />Phase: F32<br />SigType: AstNode(data='SignalType', _id=1263)<br /> | Signal Generator Settings
Toggle | :large_blue_circle: async | assert |  | Toggle Signal Generator On/Off.
Skip | :large_blue_circle: async | assert |  | Skip next sample
Dp | :large_blue_circle: async | assert | reqType: DpReqType<br />records: U32<br />priority: U32<br /> | Signal Generator Settings



### 4.3 Telemetry Dictionary
Telemetry | Type | Update | Description 
-- | -- | -- | -- 
PairHistory | AstNode(data='SignalPairSet', _id=1315) | always | Last 10 (time, value) pairs of the signal
History | AstNode(data='SignalSet', _id=1309) | always | Last 10 Y values of the signal
Info | AstNode(data='SignalInfo', _id=1321) | always | Composite field of signal information, containing histories, pairs etc
PairOutput | AstNode(data='SignalPair', _id=1303) | always | Single (time, value) pair of the signal
DpBytes | U32 | always | DP bytes written
Type | AstNode(data='SignalType', _id=1293) | always | Type of the output signal: SINE, TRIANGLE, etc.
Output | F32 | always | Single Y value of the output
DpRecords | U32 | always | DP records written



### 4.4 EVR Dictionary
EVR | Severity | Args | Description 
-- | -- | -- | -- 
DpMemReceived | activity low | size: U32<br /> | 
DpsNotConnected | warning high |  | 
DpMemoryFail | warning high |  | 
SettingsChanged | activity low | Frequency: U32<br />Amplitude: F32<br />Phase: F32<br />SignalType: AstNode(data='SignalType', _id=1349)<br /> | Signal Generator Settings Changed
DpComplete | activity low | records: U32<br />bytes: U32<br /> | 
DpMemRequested | activity low | size: U32<br /> | 
DpRecordFull | warning low | records: U32<br />bytes: U32<br /> | 
InSufficientDpRecords | warning high |  | 
DpStarted | activity low | records: U32<br /> | 



## 5 Checklist

## 6 Unit Testing

