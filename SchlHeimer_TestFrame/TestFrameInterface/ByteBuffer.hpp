#ifndef BYTEBUFFER_HPP
#define BYTEBUFFER_HPP

#include <vector>
#include "cdef.h"

// The ByteBuffer is sort of a container that stores data internally as a
// vector of uint8. There are two main use cases:
//
// 1a. Append items as binary data
// 1b. Append items as ByteBuffer
//
// 2a. Get items as binary data
// 2b. Get items as ByteBuffer
//
// Reading data does not remove the data from the buffer, but increases a
// position counter. It can be rewound using the vRewind method.

class ByteBuffer
{
public:

   ByteBuffer::ByteBuffer() : m_uiCurrPos(0)
   {
   }

   virtual ByteBuffer::~ByteBuffer()
   {
   }

   // Append (copy) data to internal buffer
   void vAppendItem(const void *pvItem, size_t length);
   void vAppendItem(const ByteBuffer &oBuffer);

   // Gets (copy) data from internal buffer and
   // increases the current read position
   bool boGetNextItem(void *pvItem, size_t length);
   bool boGetNextItem(ByteBuffer *poBuffer, size_t length);

   // Peeks at data from internal buffer only
   // (does not increate the current read position)
   bool boPeekNextItem(void *pvItem, size_t length);
   bool boPeekNextItem(ByteBuffer *poBuffer, size_t length);

   // Forwards read position (discards data)
   void vForward(size_t length)
   {
      m_uiCurrPos += (unsigned) length;
      if (m_uiCurrPos > m_buffer.size())
         m_uiCurrPos = (unsigned) m_buffer.size();
   }

   // Resets read position to read items again
   void vRewind(void)
   {
      m_uiCurrPos = 0;
   }

   // Get total size of data in buffer
   uint32 u32GetTotalSize(void) const
   {
      return (uint32) m_buffer.size();
   }

   // Clear byte buffer to be used again
   void vClear(void)
   {
      m_buffer.clear();
      m_uiCurrPos = 0;
   }

   // Check if read position is at end of buffer
   bool boIsEmpty(void)
   {
      return m_buffer.size() == m_uiCurrPos;
   }


private:
   size_t get_fill_level(void) const
   {
      return m_buffer.size() - m_uiCurrPos;
   }

   // Low level functions used for Get and Peek
   unsigned uiReadItem(uint8 *puBuffer, size_t length);
   unsigned uiReadItem(ByteBuffer *poBuffer, size_t length);

   unsigned m_uiCurrPos;
   std::vector<uint8> m_buffer;
};

#endif
