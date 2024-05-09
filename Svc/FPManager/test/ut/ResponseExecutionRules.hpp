#ifndef _RESPONSE_EXECUTION_RULES_HPP
#define _RESPONSE_EXECUTION_RULES_HPP

#include "RulesHeaders.hpp"


namespace Svc {



  struct ExecuteRESP_1 : public STest::Rule<Svc::Tester> {

             ExecuteRESP_1();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };
  

  struct IndicateRESP_1DroppedDisbled : public STest::Rule<Svc::Tester> {

             IndicateRESP_1DroppedDisbled();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };

  struct IndicateRESP_1DroppedQueued : public STest::Rule<Svc::Tester> {

             IndicateRESP_1DroppedQueued();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };

  struct IndicateRESP_1DroppedExecuting : public STest::Rule<Svc::Tester> {

             IndicateRESP_1DroppedExecuting();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };
  


  struct ExecuteRESP_2 : public STest::Rule<Svc::Tester> {

             ExecuteRESP_2();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };
  

  struct IndicateRESP_2DroppedDisbled : public STest::Rule<Svc::Tester> {

             IndicateRESP_2DroppedDisbled();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };

  struct IndicateRESP_2DroppedQueued : public STest::Rule<Svc::Tester> {

             IndicateRESP_2DroppedQueued();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };

  struct IndicateRESP_2DroppedExecuting : public STest::Rule<Svc::Tester> {

             IndicateRESP_2DroppedExecuting();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };
  


  struct ExecuteSAFING : public STest::Rule<Svc::Tester> {

             ExecuteSAFING();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };
  

  struct IndicateSAFINGDroppedDisbled : public STest::Rule<Svc::Tester> {

             IndicateSAFINGDroppedDisbled();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };

  struct IndicateSAFINGDroppedQueued : public STest::Rule<Svc::Tester> {

             IndicateSAFINGDroppedQueued();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };

  struct IndicateSAFINGDroppedExecuting : public STest::Rule<Svc::Tester> {

             IndicateSAFINGDroppedExecuting();

             bool precondition(const Svc::Tester& tester);

             void action(Svc::Tester& tester);

  };
  
  
  
}
#endif
