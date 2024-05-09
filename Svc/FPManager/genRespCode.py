#!/usr/bin/env python3

"""
Filename: genRespCode.py
Author:  Garth Watney

This script reads the text file 'ResponseList.txt' which contains a list of all the Response names.
For each response in the Response list, the script expects to see and read a response command list 
which contains a list of commands that will be encoded for each response.
For example if 'FaileSafe1' is a name of a response in the ResponseList.txt, then the script will try
and read the file 'FailSafe1.txt'.  If this does not exist, then the script prints an error message and
exits.  This text file then contains a list of commands and arguments.  Any valid command found in the
application dictionary is valid.

The script then uses the python command dictionary found in Gse/generated/LITS/commands to get the
opcodes and arguments for each command needed by the responses.

Using all this information, the script then builds the following products:

- FPResponses.hpp - an enumeration list of all the response names
- FPResponseTable.cpp - an instantiation of all the responses and table of default response states
- CMakeLists.txt - contains all the response classes
- TlmDict.fppi - telemetry channels for each response
- CommandOpcodes.hpp - an enumeration list of the needed command mnemonics and their opcodes
- CommandSerializer.hpp - header for serialized commands
- CommandSerializer.cpp - implementation for for serialized commands
- response hpp file for each response - response header class
- response cpp file for each response - response implementation class
- test/ut/Tester.hpp - makes friends of all the response rules
- test/ut/ResponseExecutionRules.hpp - response rule headers for unit testing
- test/ut/ResponseExecutionRules.cpp - response rule implementation for testing
- TestProcedureTable.adoc

"""

import sys
import os
from Cheetah.Template import Template
import xml.etree.ElementTree as ET


def write_CommandSerializer(thisList):
    
    """
    Using the python command dictionary, and a list of command opcode mnemonics,
    generate the command opcodes and a serialized function for each command.
    """



    # Read the dictionary
    tree = ET.parse('../../Ref/build-artifacts/Linux/Ref/dict/RefTopologyAppDictionary.xml')
    root = tree.getroot()

    cmd_dict = {} 

    for cmdElements in root.iter('command'):
      cmd = cmdElements.attrib
      name = cmd['mnemonic']
      opcode = cmd['opcode']
      argList = []
      for args in cmdElements.iter('arg'):
        argList.append(args.attrib)
      cmd_dict[name] = (name, opcode, argList)
     
    class argForTemplate:
        pass
    
    tmplMaps = []
    for command in thisList:
        newArg = argForTemplate()
        opcode = cmd_dict[command][1]
        args = cmd_dict[command][2]
        argList = []
        for arg in args:
            argName = arg['name']
            argType = arg['type']
            argList.append((argName, argType))
        
        newArg.command = command
        newArg.opcode = opcode
        newArg.argList = argList
        tmplMaps.append(newArg)
        
    headerFile = open("CommandSerializer.hpp", 'w')
    template = Template(file = "CommandSerializer.hpp.tmpl")
    template.cmds = tmplMaps
    headerFile.write(str(template))
    headerFile.close()
    print("Writing CommandSerializer.hpp")
    
    cppFile = open("CommandSerializer.cpp", 'w')
    template = Template(file = "CommandSerializer.cpp.tmpl")
    template.cmds = tmplMaps
    cppFile.write(str(template))
    cppFile.close()
    print("Writing CommandSerializer.cpp")
    
    opcodeFile = open("CommandOpcodes.hpp", 'w')
    template = Template(file = "CommandOpcodes.hpp.tmpl")
    template.cmds = tmplMaps
    opcodeFile.write(str(template))
    opcodeFile.close()
    print("Writing CommandOpcodes.hpp")
       

def readResponseList():
    """
    Open the ResponseList.txt and return a list of response names
    """
    with open('ResponseList.txt') as f:
        thisList = [x.strip() for x in f.readlines()]
                        
    return thisList

def write_FPResponses_hpp(thisList):
    """
    Given a list of response names, generate the enumeration list of responses
    """

    upperCaseList = [x.upper() for x in thisList]
    thisFile = open("FPResponses.hpp", 'w')
    template = Template(file = "FPResponses.hpp.tmpl")
    template.responses = upperCaseList
    thisFile.write(str(template))
    thisFile.close()
    print("Writing FPResponses.hpp")
    
def write_FPResponseTable_cpp(thisList):
    """
    Given a list of response names and their priority, generate the FPResponseTable.cpp which contains
    the instantiated responses and a table of default response states
    """
   
    thisFile = open("FPResponseTable.cpp", 'w')
    template = Template(file = "FPResponseTable.cpp.tmpl")
    template.priorityResponses = thisList
    thisFile.write(str(template))
    thisFile.close()
    print("Writing FPResponseTable.cpp")
    
def write_CMakeLists(thisList):
    """
    Given a list of response names, generate the CMakeLists file which contains all the response class code
    """
   
    thisFile = open("CMakeLists.txt", 'w')
    template = Template(file = "CMakeLists.txt.tmpl")
    template.responses = thisList
    thisFile.write(str(template))
    thisFile.close()
    print("Writing CMakeLists.txt")
    
def readResponseTextFiles(inList):
    """
    Given a list of response names, read the command text file for each response.
    If the text file does not exist, then print out an error message and exit the script.
    Read all the text files and create a list of all the response commands and their arguments
    """
 
    thisList = []
    for response in inList:
        cmdMap = {}
        try:
            with open(response+'.txt') as f:
                cmdList = f.readlines()
        except:
            print("*** ERROR: You are missing an expected file:  " + response + '.txt')
            sys.exit()
            
        #Augment the cmdList with the comBuffer argument
        newCmdList = []
        for cmd in cmdList:
            # Ignore the command if it's commented out
            if cmd[0] == '#':
                continue
            pos = cmd.find('(')
            if len(cmd[pos:].replace(' ', '').strip()) > 2:
                # Then there are arguments, add the ,comBuffer
                cmd = cmd.replace(')', ',comBuffer)')
            else:
                cmd = cmd.replace(')', 'comBuffer)')
            newCmdList.append(cmd.strip())
            
            
        cmdMap[response] = newCmdList
        thisList.append(cmdMap)
        
    return thisList


def readResponseTextFilesRaw(inList):
    """
    Given a list of response names, read the command text file for each response.
    If the text file does not exist, then print out an error message and exit the script.
    Read all the text files and create a list of all the response commands and their arguments but
    return the commands just as specified in the txt files without any other processing
    """
 
    thisList = []
    for response in inList:
        cmdMap = {}
        try:
            with open(response+'.txt') as f:
                cmdList = f.readlines()
        except:
            print("*** ERROR: You are missing an expected file:  " + response + '.txt')
            sys.exit()
            
        #Augment the cmdList with the comBuffer argument
        newCmdList = []
        for cmd in cmdList:        
            newCmdList.append(cmd.strip())
            
            
        cmdMap[response] = newCmdList
        thisList.append(cmdMap)
        
    return thisList

def getCommandMnemonics(commandList):
    """
    Given a response command list, generate a list of all the command mnemonics
    """

    thisList = []
    for response in commandList:
        cmds = list(response.values())[0]
        for cmd in cmds:
            thisList.append(cmd.split('(')[0])
                     
    # Remove a list without any duplicates
    return list(set(thisList))

def write_RespClassFiles(thisList):
    """
    Given a response command list, generate a class header and cpp file for each response which
    contains the run() implementation function 
    """
    
    for response in thisList:
        baseFileName = list(response.keys())[0]
        commandList = response[baseFileName]
        cppFile = baseFileName + ".cpp"
        hppFile = baseFileName + ".hpp"
                
        thisFile = open(hppFile, 'w')
        template = Template(file = "ResponseComponent.hpp.tmpl")
        template.response = baseFileName 
        thisFile.write(str(template))
        thisFile.close()
        print("Writing " + hppFile)
        
        thisFile = open(cppFile, 'w')
        template = Template(file = "ResponseComponent.cpp.tmpl")
        template.response = baseFileName            
        template.commandList = commandList
                    
        thisFile.write(str(template))
        thisFile.close()
        print("Writing " + cppFile)
        
        
def write_UnitTest(thisList):     
        """
        Given a response command list, generate unit test rules for unit testing
        """
           
        thisFile = open("test/ut/ResponseExecutionRules.hpp", 'w')
        template = Template(file = "test/ut/ResponseExecutionRules.hpp.tmpl")
        template.responseList = thisList
        thisFile.write(str(template))
        thisFile.close()
        print("Writing test/ut/ResponseExecutionRules.hpp")
        
        thisFile = open("test/ut/ResponseExecutionRules.cpp", 'w')
        template = Template(file = "test/ut/ResponseExecutionRules.cpp.tmpl")
        template.responseList = thisList
        thisFile.write(str(template))
        thisFile.close()
        print("Writing test/ut/ResponseExecutionRules.cpp")
        
               
def write_Tlm_xml(thisList):
    """
    Given a list of responses, generate the telemetry xml which contains telemetry channels for each response
    """
    
    thisFile = open("TlmDict.fppi", 'w')
    template = Template(file = "TlmDict.fppi.tmpl")
    template.responses = thisList
    thisFile.write(str(template))
    thisFile.close()
    print("Writing TlmDict.fppi")
    
def write_Tester_hpp(thisList):
    """
     Given a list of responses, generate friends in the Tester.hpp so that the response classes can have access
     to the Tester private data
    """
    
    thisFile = open("test/ut/Tester.hpp", 'w')
    template = Template(file = "test/ut/Tester.hpp.tmpl")
    template.responses = thisList
    thisFile.write(str(template))
    thisFile.close()
    print("Writing test/ut/Tester.hpp")
    
     
def write_TestProcedureTable(thisResponses, thisCommands):
    """
     Given a list of responses and a mapping of the commands to these responses, generate a table to be
     used in the integrated test procedure.  
    """
    thisFile = open("TestProcedureTable.adoc", 'w')
    template = Template(file = "TestProcedureTable.adoc.tmpl")
    template.responseList = thisResponses
    template.commandList = thisCommands
    thisFile.write(str(template))
    thisFile.close()
    print("TestProcedureTable.adoc")      
 
def main(argv=None):

    priorityList = readResponseList()
        
    responseList = [x.split()[0] for x in priorityList]
            
    write_FPResponses_hpp(responseList)
    
    write_FPResponseTable_cpp(priorityList)
    
    write_CMakeLists(responseList)
    
    write_Tlm_xml(responseList)
    
    commandList = readResponseTextFiles(responseList)

    commandMnemonicsList = getCommandMnemonics(commandList)
                    
    write_CommandSerializer(commandMnemonicsList)
    
    write_RespClassFiles(commandList)
    
    write_Tester_hpp(responseList)
        
    write_UnitTest(commandList)
    
    txtCmdList = readResponseTextFilesRaw(responseList)
    
    write_TestProcedureTable(responseList, txtCmdList)
    

if __name__ == "__main__":
    sys.exit(main())
