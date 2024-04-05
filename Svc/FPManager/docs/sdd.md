<title>Svc::FPManger Component SDD</title>
# Svc::FPManager Component

## 1. Introduction

The `Svc::FPManager` is designed to be a general-purpose fault protection manager component for multiple missions. <br>
Incoming Response indications are mapped to Responses.  Each Response is implemented as a hard-coded set of application commands and arguments.  Any command and argument in the command dictionary is allowed. <br>
The FPMamanger implements a priority queue of pending responses.  When a response is running, the commands are pushed out to the Command Dispatcher component at a 10 Hz rate.

## 2. Requirements

The requirements for `Svc::FPManager` are:
<table>
<thead>
<tr>
<th>Requirement</th>
<th>Description</th>
<th>Rational</th>
<th>Verification Method</th>
</tr>
</thead>
<tbody>

<tr>
<td><b>L5-FP-001</b></td>
<td>
The `Svc::FPManager` component shall map each incoming Response indication  to a Fault Protection Response. 
</td>
<td>
A Response indication in this context is a request to run a response.  <br>
There is a one to one mapping of Response indications to responses <br>
We expect around 12 to 15 unique responses.
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-002</b></td>
<td>
The `Svc::FPManager` component shall drop incoming Response indications if the component input queue is full.  This shall result in an EVR and telemetered dropped count.
</td>
<td>
It's difficult to size the queue correctly if there is a flood of incoming Response indicators. <br>
We don't want to assert the sotware for this case.
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-003</b></td>
<td>
The `Svc::FPManager` component shall add the mapped Response to an execution queue <br>
If the Response is disabled, then the Response shall be dropped. <br>
If the Response is already in the queue, then the Response shall be dropped. <br>
An EVR shall be generated indicating how the response request was handled - dropped and why, or placed on the queue
</td>
<td>
If we are already planning on running the response then we don't need to queue it.
Responses can be enabled or disabled
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-004</b></td>
<td>
The `Svc::FPManager` component shall upon a command enable or disable a response.
</td>
<td>
For testing purposes we might not want a response to execute
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-005</b></td>
<td>
The `Svc::FPManager` component shall prioritize Responses in the queue.   
</td>
<td>
Some Responses have a higher priority than other responses and shall be placed in the queue by their priority order.
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-006</b></td>
<td>
The `Svc::FPManager` component shall execute Responses consecutively to completion. 
</td>
<td>
There are no parallel or interrupting responses.
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-007</b></td>
<td>
The `Svc::FPManager` component shall encapsulate each Response as a series of Application commands with arguments.
</td>
<td>
Each Response is a canned hard-coded list of commands.
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-008</b></td>
<td>
The `Svc::FPManager` component shall issue commands for a Response at a 10 Hz rate and shall not wait for response completion notifications. <br>
All bad command response completion notifications shall result in an EVR Warning.
</td>
<td>
Fault Protection commands need to go out without delays 
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-009</b></td>
<td>
The `Svc::FPManager` component shall generate an EVR Warning High when a Response starts execution and when a Response completes exeuction.
</td>
<td>
The user can track the behavior of the FP Manager.
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-010</b></td>
<td>
The `Svc::FPManager` component shall send the Response enable or disable state as packed telemetry channels.
</td>
<td>
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-011</b></td>
<td>
The `Svc::FPManager` component shall maintain a counter for the number of times each incoming Response indication  was alerted and send the value out as a telemetry channel.
</td>
<td>
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-012</b></td>
<td>
The `Svc::FPManager` component shall maintain a counter for the number of times each Response was completed and send the value out as a telemetry channel.
</td>
<td>
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-013</b></td>
<td>
The `Svc::FPManager` component shall send the last executed Response as a telemetry channel.
</td>
<td>
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-014</b></td>
<td>
The `Svc::FPManager` component shall send the last reported Response indication as a telemetry channel.
</td>
<td>
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-015</b></td>
<td>
The `Svc::FPManager` component shall upon a command clear the counters for the number of times each incoming Resoibse indication was alerted and the number of times each Response was completed and the last executed Response and the last Fault alert. 
</td>
<td>
This command resets the state of the FPManager but not the input execution queue.
</td>
<td>
Unit test
</td>
</tr>

<tr>
<td><b>L5-FP-016</b></td>
<td>
The `Svc::FPManager` component shall upon a command, execute a response only if the command is not running or is not in the response pending queue.  If the response is rejected for this reason, an informational EVR is generated.
</td>
<td>
We don't want to execute a response if the response is pending.
</td>
<td>
Unit test
</td>
</tr>

</tbody>
</table>

## 3. Design

### 3.1 Component Context

The `Svc::FPManager` context diagram:

![`Svc::FPManager` Component Context Diagram](img/FPManagerContextDiagram.png)

### 3.2 Component Interface

| Fault Protection Manager Component Model |
| --- |
| [FPManager.fpp](../FPManager.fpp) |


### 3.3 Port Description Table

The Svc::FPManager component has the standard command, telemetry and evr ports in addition to:

| Port Data Type |
| --- |
| [`Svc::FP`](../../../Svc/FPport/FP.fpp) |

### 3.4 Functional Description

The FPManager component has one instance of a Priority Queue for the pending Responses.  <br>
Each response in the system inherits from the base Response class.  A response encapsulates the hard-coded sequence of commands.  The inherited response classes - Resp1 .. Resp15 are auto-generated from .txt files in the Component directory.

![`Svc::FPManager` Component Class Diagram](img/FPManagerClass.png)

<b>FaultIn_handler()</b> <br>
Update the response indicator counter (total number of times a response was alerted) <br>
If the specified response is disabled, then generate an EVR indicating the fault was dropped because it was disabled. Return. <br> 
If the response is either currently running, or is already in the execution queue, then generate a Warning EVR indicating the fault was dropped because it is pending execution. Return. <br>
Place the response in the execution queue.

<b>Run_handler()</b> <br>
If the FPManager state == Idle (Not executing a response) then: <br>
If the execution queue is empty then return. <br>
else pull the next response off the execution queue (priority based) and save the response as the currently executing response<br>
Generate an EVR indicating this response has started execution. <br>
Set the FPManager state to Executing.  Return. <br>

Pull the next command from the currently executing response command list.  Dispatch the command to the output comCmdOut port. <br>
If there are no more commands to be dispatched for this response then update the last response value, update the response counter and generate an EVR indicating this response has completed execution. <br>
Set the FPManager state to Idle.  Return. <br>

<b>cmdResponseIn_handler()</b> <br>
If the command response is OK then return. <br>
Generate a Warning EVR indicating the command opcode with a bad command response


<b>FPM_FAULT_ID_ON_OFF_handler()</b> <br>
Set the specified fault in the Fault table to either enable or disable.

<b>FPM_ALL_FAULT_IDS_ON_OFF_handler()</b> <br>
Set all the faults in the Fault table to either enable or disable


<b>FPM_EXECUTE_FAULT_RESPONSE_handler()</b> <br>
If the specified response is either currently running, or is already in the exeuction queue, or the Response is disabled, then generate a Warning EVR and return, <br>
else place the specified response in the execution queue.


<b>FPM_RESET_COUNTERS_handler()</b> <br>
Reset all the fault counters (total number of times a fault was alerted)<br>
Reset all the response counters (total numbrer of times a response completed)<br>
Clear the last fault value (last fault that was alerted)<br>
Clear the last response value (last response that was completed)<br>


<b>FPM_RESET_EVR_THROTTLING_handler()</b> <br>
Reset the EVR throttling


### 3.5 State

`Svc::FPManager` has an Idle and Executing state. This indicates if the FPManager is currently executing a response (Executing) or not (Idle).

### 3.6 Algorithms

`Svc::FPManager` has no significant algorithms.

## 4. Dictionary

[Link](FPManager.html)

The Commands are defined in [Cmd.xml](../Cmd.xml). <br>
The Telemetry is defined in [Tlm.xml](../Tlm.xml).<br>
The EVR's are defined in [Evr.xml](../Evr.xml).<br>

## 5. Module Checklists

Document | Link
-------- | ----
Design Checklist | [Link](Checklist_Design.xlsx)
Code Checklist | [Link](Checklist_Code.xlsx)
Unit Test Checklist | [Link](Checklist_Unit_Test.xlsx)

## 6. Unit Testing

[Unit Test Output](UnitTestOutput.txt)

[Coverage Summary](SvcFPManager_gcov.txt)


## 6.1 Unit Test Overview
The unit test rules that are specified in ResponseExecutionRules.hpp/.cpp are auto-generated via 'make code' in the component top directory.<br>
These rules look at what commands are specified in each response and executes the responses through to completion. <br>
Other rules specified in TestRules.hpp/.cpp are manually written and exercise the Fault Protection manager component through both nominal and off-nomonal conditions.

### 6.1.1 Execute Responses (Test Case 6400.1.1)
This test case commands each response to be executed. All the commands that are encoded in the response are checked. The following rules are run consecutively: 
<ol>
   <li>Execute Resp1
   <li>Execute Resp2
   <li>Execute Resp3
   <li>Execute Resp4
   <li>Execute Resp5
   <li>Execute Resp6
   <li>Execute Resp7
   <li>Execute Resp8
   <li>Execute Resp9
   <li>Execute Resp10
</ol>

### 6.1.2 Indicate Responses when disabled (Test Case 6400.1.2)
This test case disables each response and then sends a response indication and verifies that the response indication is dropped because the response is disabled.  The following rules are run consecutively:
<ol>
   <li>Indicate Resp1 dropped because disabled
   <li>Indicate Resp2 dropped because disabled
   <li>Indicate Resp3 dropped because disabled
   <li>Indicate Resp4 dropped because disabled
   <li>Indicate Resp5 dropped because disabled
   <li>Indicate Resp6 dropped because disabled
   <li>Indicate Resp7 dropped because disabled
   <li>Indicate Resp8 dropped because disabled
   <li>Indicate Resp9 dropped because disabled
   <li>Indicate Resp10 dropped because disabled
</ol>

### 6.1.2 Indicate Responses when queued (Test Case 6400.1.3)
This test case enables each response and then sends two response indications.   The first response indication is placed in the queue while the second response indication is dropped because the response is already queued.  The following rules are run consecutively:
<ol>
   <li>Indicate Resp1 dropped because queued
   <li>Indicate Resp2 dropped because queued
   <li>Indicate Resp3 dropped because queued
   <li>Indicate Resp4 dropped because queued
   <li>Indicate Resp5 dropped because queued
   <li>Indicate Resp6 dropped because queued
   <li>Indicate Resp7 dropped because queued
   <li>Indicate Resp8 dropped because queued
   <li>Indicate Resp9 dropped because queued
   <li>Indicate Resp10 dropped because queued
</ol>

### 6.1.3 Indicate Responses when executing (Test Case 6400.1.4)
This test case enables each response and then sends two response indications.   The first response indication is placed in the queue and starts to execute,  while the second response indication is dropped because the response is executing. The following rules are run consecutively:
<ol>
   <li>Indicate Resp1 dropped because executing
   <li>Indicate Resp2 dropped because executing
   <li>Indicate Resp3 dropped because executing
   <li>Indicate Resp4 dropped because executing
   <li>Indicate Resp5 dropped because executing
   <li>Indicate Resp6 dropped because executing
   <li>Indicate Resp7 dropped because executing
   <li>Indicate Resp8 dropped because executing
   <li>Indicate Resp9 dropped because executing
   <li>Indicate Resp10 dropped because executing
</ol>

### 6.1.4 Exercise response priority (Test Case 6400.1.5)
This test case sets all the responses to low priority except the 5th response. The pending response priority queue is loaded to full by indicating each response.  Check that the highest priority response (#5) is the first to be pulled from the queue and started execution.  Verify that all the queue entries then get pulled in the same input order and executed.  This test case is run:
<ol>
   <li> Check Priority Queue
</ol>

### 6.1.5 Misc testing (Test Case 6400.1.6)
This test case checks the packed response state telemetry and various off-nominal conditions.  The following rules are run consecutively:
<ol>
   <li>Packed response state telemetry
   <li>Overflow response indicator messages are dropped
   <li>Ping response
   <li>Bad response identification arguments
   <li>Bad response completion messages
   <li>Response execution when already executing
</ol>



## 7. Change Log

Date | Description
---- | -----------
1/12/2016 | Requirements and Design review 
1/12/2016 | Code review

