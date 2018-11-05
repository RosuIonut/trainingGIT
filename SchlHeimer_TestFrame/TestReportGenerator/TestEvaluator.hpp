#ifndef TESTEVALUATOR_HPP
#define TESTEVALUATOR_HPP

#include <string>

#include "ReportGenerator.hpp"

class TestEvaluator
{
public:
   TestEvaluator() : m_boForceEvaluation(false)
   {
   }

   ~TestEvaluator()
   {
   }

   void vSetForceEvaluation(void)
   {
      m_boForceEvaluation = true;
   }

   void vSetInputPath(const std::string &sPath)
   {
      m_sInputPath = sPath;
   }

   void vSetOutputPath(const std::string &sPath)
   {
      m_sOutputPath = sPath;
   }

   bool boHasInputPath(void)
   {
      return !m_sInputPath.empty();
   }

   void vSetActorString(const std::string &sActor)
   {
      m_sActorString = sActor;
   }

   bool boEvaluate(ReportGenerator *poRepGen);

private:
   bool boLoadTestSpec(std::string sFile, mrts::document *poDoc);
   void vInsertResult(mrts::test_case &stCase, ReportGenerator *poRepGen);

   std::string m_sInputPath;
   std::string m_sOutputPath;
   std::string m_sActorString;
   bool m_boForceEvaluation;

};

#endif
