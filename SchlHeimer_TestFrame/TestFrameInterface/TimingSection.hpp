#ifndef TIMINGSECTION_HPP
#define TIMINGSECTION_HPP

extern "C"
{
#include "SHMEM_Layout.h"
}

#include "ShMemSection.hpp"

// Specific section "Timing".

class TimingSection : public ShMemSection
{
public:
   TimingSection() : ShMemSection(SHMEM_nSectTimingTest)
   {
      m_stTimingTest.u16MsNeeded = 0;
      m_stTimingTest.u8CycleCount100ms = 0;
   }

   virtual ~TimingSection()
   {
   }

   void vSetCycleCount(uint8 u8Cycles)
   {
      m_stTimingTest.u8CycleCount100ms = u8Cycles;
   }

   uint8 u8GetCycleCount(void)
   {
      return m_stTimingTest.u8CycleCount100ms;
   }

   uint16 u16GetTimeNeeded(void)
   {
      return m_stTimingTest.u16MsNeeded;
   }

   virtual void vSerialize(ByteBuffer *poBuffer) const
   {
      poBuffer->vAppendItem(&m_stTimingTest, sizeof(m_stTimingTest));
   }

   virtual bool boDeserialize(ByteBuffer *poBuffer)
   {
      return poBuffer->boGetNextItem(&m_stTimingTest, sizeof(m_stTimingTest));
   }

private:
   SHMEM_tstTimingTest m_stTimingTest;
};

#endif
