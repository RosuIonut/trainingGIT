#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <lib-util.hpp>

extern "C"
{
#include "SHMEM_Layout.h"
}

#include "CommandInterpreter.hpp"

#define VERSION  "1.1"

void print_help(const char *s)
{
   printf("%s version %s\n", s, VERSION);
   printf("Built with SHMEM_nLayoutVersion = 0x%.2x\n", SHMEM_nLayoutVersion);
   printf("\n");
   printf("Usage: %s [options]\n", s);
   printf("Version: %s", VERSION);
   printf("\n");
   printf("Options:\n");
   printf("  -h         print help and exit\n");
   printf("  -i         interactive session\n");
   printf("  -q         decrease verbosity by one level\n");
   printf("  -s <file>  use 'file' as startup script\n");
   printf("  -v         increase verbosity by one level\n");
}

int main(int argc, char *argv[])
{
   int do_exit = 0;

   util::Log::vInit();

   CommandInterpreter oInterpreter;
   oInterpreter.vSetVersionString("TFInterface " VERSION);

   for (int i = 1; !do_exit && i < argc; i++)
   {
      if (!strcmp(argv[i], "-h"))
      {
         print_help(util::basename(argv[0]));
         do_exit = 1;
         continue;
      }

      if (!strcmp(argv[i], "-i"))
      {
         oInterpreter.vSetInteractive(true);
         continue;
      }

      if (!strcmp(argv[i], "-s"))
      {
         if (i + 1 < argc)
         {
            oInterpreter.vSetStartupScript(argv[++i]);
         }
         else
         {
            util::Log::vPrint(util::LOG_ERROR, "Missing argument (script file)\n");
            do_exit = 1;
         }
         continue;
      }

      if (!strcmp(argv[i], "-q"))
      {
         util::Log::vDecreaseVerbosity();
         continue;
      }

      if (!strcmp(argv[i], "-v"))
      {
         util::Log::vIncreaseVerbosity();
         continue;
      }

      util::Log::vPrint(util::LOG_ERROR, "Unknown argument: %s", argv[i]);
      do_exit = 1;
   }

   if (!do_exit)
   {
      util::Log::vPrint(util::LOG_DEBUG, "Printing of debug messages enabled.\n");
      oInterpreter.boRunSession();
   }

   return 0;
}
