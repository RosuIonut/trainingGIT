#include <cstdio>
#include <cstdarg>



/*#include "EVHD_C1.h"
#include "IL_C1.h"
#include "DPOOL_C1.h"
#include "TT__ExecWrapper.h"*/

/*#include "dpool__gendefinitions.h"
#include "dpool__gen2ct.h"
#include "evhd_events.h"
#include "evhd_main.h"


#define TT__Set_DPOOL_MFL_VAL(field, val)                                                                                                 \
{                                                                                                                                         \
   DPOOL__xCUHIC_MFLCANDataData temp = {EventCode1 = 0, EventCode2 = 0, TastenCode1 = 0, TastenCode2 = 0, MflMarke = 0, MflVariante = 0}; \
   DPOOL_enGetPoolData_DPOOL_nDataCUHIC_MFLCANData(&temp);                                                                                \
   temp.field = val;                                                                                                                      \
   DPOOL_enSetPoolData(DPOOL_nDataCUHIC_MFLCANData, &val);                                                                                \
}

#define  TT__Set_DPOOL_MFL_EventCode1 (val)  TT__Set_DPOOL_MFL_VAL(EventCode1  , val)
#define  TT__Set_DPOOL_MFL_EventCode2 (val)  TT__Set_DPOOL_MFL_VAL(EventCode2  , val)
#define  TT__Set_DPOOL_MFL_TastenCode1(val)  TT__Set_DPOOL_MFL_VAL(TastenCode1 , val)
#define  TT__Set_DPOOL_MFL_TastenCode2(val)  TT__Set_DPOOL_MFL_VAL(TastenCode2 , val)
#define  TT__Set_DPOOL_MFL_MflMarke   (val)  TT__Set_DPOOL_MFL_VAL(MflMarke    , val)
#define  TT__Set_DPOOL_MFL_MflVariante(val)  TT__Set_DPOOL_MFL_VAL(MflVariante , val)

#define TT__Set_DPOOL_FAS_VAL(field, val)                                                                   \
{                                                                                                           \
   DPOOL__xCUHIC_FASCANDataData temp = {FASTaster = 0, FASTasterFehler = 0, TastenCode1 = 0, reserved = 0}; \
   DPOOL_enGetPoolData_DPOOL_nDataCUHIC_FASCANData(&temp);                                                  \
   temp.field = val;                                                                                        \
   DPOOL_enSetPoolData(DPOOL_nDataCUHIC_FASCANData, &val);                                                  \
}

#define  TT__Set_DPOOL_FAS_FASTaster       (val)  TT__Set_DPOOL_FAS_VAL(FASTaster       , val)
#define  TT__Set_DPOOL_FAS_FASTasterFehler (val)  TT__Set_DPOOL_FAS_VAL(FASTasterFehler , val)*/


#define myMacro(field  , val)         \
{                                   \
    MyBollox temp;                  \
    temp.##field = val;             \
}

#define MyMacro_BolloxField(val)    myMacro(BolloxField    , val)

struct MyBollox
{
    int BolloxField;
} sMyBollox;



#include "Log.hpp"

namespace util
{

   int Log::m_iVerbosity;

   void Log::vInit(void)
   {
      Log::m_iVerbosity = LOG_DEFAULT_LEVEL;
   }

   void Log::vIncreaseVerbosity(void)
   {
      Log::m_iVerbosity++;
   }

   void Log::vDecreaseVerbosity(void)
   {
      Log::m_iVerbosity--;
   }

   void Log::vPrint(int iLevel, const char *fmt, ...)
   {
      va_list args;
      va_start(args, fmt);

      if (iLevel <= m_iVerbosity)
      {
         FILE *stream = (iLevel <= LOG_WARNING) ? stderr : stdout;
         vfprintf(stream, fmt, args);
         fputc('\n', stream);
      }
      else
      {
         // Not important enough
      }

      va_end(args);
   }

}