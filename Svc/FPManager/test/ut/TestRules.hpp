#ifndef _TEST_RULES_HPP
#define _TEST_RULES_HPP

#include "RulesHeaders.hpp"

namespace Svc {

   struct CheckPriorityQ : public STest::Rule<Svc::Tester> {

            CheckPriorityQ();

            bool precondition(const Svc::Tester& tester);

            void action(Svc::Tester& tester);

     };

   struct PackedTlm : public STest::Rule<Svc::Tester> {

            PackedTlm();

            bool precondition(const Svc::Tester& tester);

            void action(Svc::Tester& tester);

     };

   struct QueOverflow : public STest::Rule<Svc::Tester> {

             QueOverflow();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

      };

   struct SendPing : public STest::Rule<Svc::Tester> {
     SendPing();

     bool precondition(const Svc::Tester& tester);

     void action(Svc::Tester& tester);
   };

   struct BadArgs : public STest::Rule<Tester> {

     BadArgs();

     bool precondition(const Svc::Tester& tester);

     void action(Svc::Tester& tester);

  };

  struct BadResp : public STest::Rule<Tester> {

     BadResp();

     bool precondition(const Svc::Tester& tester);

     void action(Svc::Tester& tester);

  };

  struct ExecuteQueued : public STest::Rule<Svc::Tester> {

     ExecuteQueued();

     bool precondition(const Svc::Tester& tester);

     void action(Svc::Tester& tester);

 };


}
#endif
