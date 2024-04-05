// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp"


TEST(Test, indicateResponsesDisabled) {
  Svc::Tester tester;
  tester.indicateResponsesDisabled();
}

TEST(Test, indicateResponsesQueued) {
  Svc::Tester tester;
  tester.indicateResponsesQueued();
}

TEST(Test, indicateResponsesExecuting) {
  Svc::Tester tester;
  tester.indicateResponsesExecuting();
}

TEST(Test, exercisePriorityQ) {
  Svc::Tester tester;
  tester.exercisePriorityQ();
}


TEST(Test, checkMisc) {
  Svc::Tester tester;
  tester.checkMisc();
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
