#ifndef LOG_HPP
#define LOG_HPP

// Logging via stdout/stderr, depending on how important the message is.
// Logging can be filtered by specifying a verbosity level.

namespace util
{
   enum {
      LOG_ERROR,
      LOG_WARNING,
      LOG_INFO,
      LOG_DEBUG,
      LOG_DEFAULT_LEVEL = LOG_WARNING,
   };

   class Log
   {
   public:
      static void vInit(void);
      static void vIncreaseVerbosity(void);
      static void vDecreaseVerbosity(void);
      static void vPrint(int iLevel, const char *fmt, ...);

   private:
      static int m_iVerbosity;
   };

}

#endif
