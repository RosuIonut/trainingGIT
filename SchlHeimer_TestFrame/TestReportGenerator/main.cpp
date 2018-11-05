#include <string>

#include <lib-util.hpp>

#include "ReportGenerator.hpp"
#include "TestEvaluator.hpp"

#define DEF_OUTPUT_PATH  "output"
#define VERSION          "1.1"

void print_help(const char *s)
{
   printf("%s version %s\n", s, VERSION);
   printf("\n");
   printf("Usage: %s [options] <input file>\n", s);
   printf("\n");
   printf("Options:\n");
   printf("  -f         force test case evaluation\n");
   printf("  -h         print help and exit\n");
   printf("  -o <path>  output directory\n");
   printf("  -q         decrease verbosity by one level\n");
   printf("  -s         create a single HTML file\n");
   printf("  -v         increase verbosity by one level\n");
}

int main(int argc, char *argv[])
{
   bool failed = false;
   bool do_exit = false;
   TestEvaluator oTestEval;
   ReportGenerator oRepGen;

   util::Log::vInit();

   oTestEval.vSetOutputPath(DEF_OUTPUT_PATH);
   oTestEval.vSetActorString(util::get_login_name() + " (TRGenerator " VERSION ")");
   oRepGen.vSetOutputPath(DEF_OUTPUT_PATH);

   for (int i = 1; !failed && i < argc; i++)
   {
      if (!strcmp(argv[i], "-f"))
      {
         oTestEval.vSetForceEvaluation();
         continue;
      }

      if (!strcmp(argv[i], "-h"))
      {
         print_help(util::basename(argv[0]));
         do_exit = true;
         continue;
      }

      if (!strcmp(argv[i], "-o"))
      {
         if (i + 1 < argc)
         {
            i++;
            oTestEval.vSetOutputPath(argv[i]);
            oRepGen.vSetOutputPath(argv[i]);
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "Missing argument (output path)\n");
            failed = true;
         }
         continue;
      }

      if (!strcmp(argv[i], "-q"))
      {
         util::Log::vDecreaseVerbosity();
         continue;
      }

      if (!strcmp(argv[i], "-s"))
      {
         oRepGen.vSetSingleFile();
         continue;
      }

      if (!strcmp(argv[i], "-v"))
      {
         util::Log::vIncreaseVerbosity();
         continue;
      }

      if (!oTestEval.boHasInputPath())
      {
         oTestEval.vSetInputPath(argv[i]);
         continue;
      }

      util::Log::vPrint(util::LOG_ERROR, "Unknown argument: %s", argv[i]);
      failed = true;
   }

   if (!failed && !do_exit)
   {
      if (!oTestEval.boHasInputPath())
      {
         util::Log::vPrint(util::LOG_ERROR, "No input file specified!");
         failed = true;
      }
   }

   if (!failed && !do_exit)
   {
      util::Log::vPrint(util::LOG_DEBUG, "Printing of debug messages enabled.\n");

      if (oTestEval.boEvaluate(&oRepGen))
      {
         if (oRepGen.boWriteReport())
         {
            util::Log::vPrint(util::LOG_INFO, "HTML report successfully written.");
         }
         else
         {
            // Writing report failed
            failed = true;
         }
      }
      else
      {
         // Evaluation process failed
         failed = true;
      }
   }

   return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
