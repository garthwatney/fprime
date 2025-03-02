// ======================================================================
// \title  AssertFatalAdapter.hpp
// \author tcanham
// \brief  cpp file for AssertFatalAdapter test harness implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include "AssertFatalAdapterTester.hpp"
#include "Fw/Types/String.hpp"
#include "Fw/Types/StringUtils.hpp"
#include "config/FppConstantsAc.hpp"

#define INSTANCE 0
#define MAX_HISTORY_SIZE 10

namespace Svc {

    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    AssertFatalAdapterTester::AssertFatalAdapterTester() : AssertFatalAdapterGTestBase("Tester", MAX_HISTORY_SIZE), component( "AssertFatalAdapter")
    {
        this->initComponents();
        this->connectPorts();
    }

    AssertFatalAdapterTester::~AssertFatalAdapterTester() {

    }

    // ----------------------------------------------------------------------
    // Tests
    // ----------------------------------------------------------------------

    void AssertFatalAdapterTester::testAsserts() {

        U32 lineNo;

        // Apply all truncations to file buffer size
        const FwSizeType fileMaxSize =
            FW_MIN(FW_MIN(AssertFatalAdapterEventFileSize, FW_LOG_STRING_MAX_SIZE), FW_ASSERT_TEXT_SIZE);

        char file[Fw::StringBase::BUFFER_SIZE(fileMaxSize)];
        Fw::String fileString;

// Asserts may be turned off resulting in this component doing a no-op
#if FW_ASSERT_LEVEL == FW_NO_ASSERT
        const int expectedSize = 0;
#else
        const int expectedSize = 1;
#endif

#if FW_ASSERT_LEVEL == FW_FILEID_ASSERT
        fileString.format("0x%08" PRIX32, ASSERT_FILE_ID);
#else
        fileString = __FILE__;
#endif
        (void) Fw::StringUtils::string_copy(file, fileString.toChar(), sizeof(file));

        // FW_ASSERT_0

        FW_ASSERT(0);lineNo = __LINE__;
        ASSERT_EVENTS_AF_ASSERT_0_SIZE(expectedSize);
        if (expectedSize > 0) {
            ASSERT_EVENTS_AF_ASSERT_0(0,file,lineNo);
        }

        // FW_ASSERT_1

        FW_ASSERT(0,1);lineNo = __LINE__;
        ASSERT_EVENTS_AF_ASSERT_1_SIZE(expectedSize);
        if (expectedSize > 0) {
            ASSERT_EVENTS_AF_ASSERT_1(0,file,lineNo,1);
        }

        // FW_ASSERT_2

        FW_ASSERT(0,1,2);lineNo = __LINE__;
        ASSERT_EVENTS_AF_ASSERT_2_SIZE(expectedSize);
        if (expectedSize > 0) {
            ASSERT_EVENTS_AF_ASSERT_2(0,file,lineNo,1,2);
        }

        // FW_ASSERT_3

        FW_ASSERT(0,1,2,3);lineNo = __LINE__;
        ASSERT_EVENTS_AF_ASSERT_3_SIZE(expectedSize);
        if (expectedSize > 0) {
             ASSERT_EVENTS_AF_ASSERT_3(0,file,lineNo,1,2,3);
        }

        // FW_ASSERT_4

        FW_ASSERT(0,1,2,3,4);lineNo = __LINE__;
        ASSERT_EVENTS_AF_ASSERT_4_SIZE(expectedSize);
        if (expectedSize > 0) {
            ASSERT_EVENTS_AF_ASSERT_4(0,file,lineNo,1,2,3,4);
        }

        // FW_ASSERT_5

        FW_ASSERT(0,1,2,3,4,5);lineNo = __LINE__;
        ASSERT_EVENTS_AF_ASSERT_5_SIZE(expectedSize);
        if (expectedSize > 0) {
             ASSERT_EVENTS_AF_ASSERT_5(0,file,lineNo,1,2,3,4,5);
        }

        // FW_ASSERT_6

        FW_ASSERT(0,1,2,3,4,5,6);lineNo = __LINE__;
        ASSERT_EVENTS_AF_ASSERT_6_SIZE(expectedSize);
        if (expectedSize > 0) {
            ASSERT_EVENTS_AF_ASSERT_6(0,file,lineNo,1,2,3,4,5,6);
        }
        // Test unexpected assert
#if FW_ASSERT_LEVEL == FW_FILEID_ASSERT
        U32 unexpectedFile = 0xF00;
        const char *const unexpectedFileArg = "0x00000F00";
#else
        const char *const unexpectedFile = "foo";
        const char *const unexpectedFileArg = unexpectedFile;
#endif

        this->component.reportAssert(unexpectedFile,1000,10,1,2,3,4,5,6);
        ASSERT_EVENTS_AF_UNEXPECTED_ASSERT_SIZE(1);
        ASSERT_EVENTS_AF_UNEXPECTED_ASSERT(0,unexpectedFileArg,1000,10);

    }

    // ----------------------------------------------------------------------
    // Helper methods
    // ----------------------------------------------------------------------

    void AssertFatalAdapterTester::connectPorts() {

        // Time
        this->component.set_Time_OutputPort(0, this->get_from_Time(0));

        // Log
        this->component.set_Log_OutputPort(0, this->get_from_Log(0));

        // LogText
        this->component.set_LogText_OutputPort(0, this->get_from_LogText(0));

    }

    void AssertFatalAdapterTester::initComponents() {
        this->init();
        this->component.init(
        INSTANCE);
    }

    void AssertFatalAdapterTester::textLogIn(const FwEventIdType id, //!< The event ID
            const Fw::Time& timeTag, //!< The time
            const Fw::LogSeverity severity, //!< The severity
            const Fw::TextLogString& text //!< The event string
            ) {
        TextLogEntry e = { id, timeTag, severity, text };

        printTextLogHistoryEntry(e, stdout);
    }

} // end namespace Svc
