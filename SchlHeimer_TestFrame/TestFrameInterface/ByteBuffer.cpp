#include "ByteBuffer.hpp"

#include <cassert>

void ByteBuffer::vAppendItem(const void *pvItem, size_t length)
{
   const uint8 *pu8Data = (uint8*) pvItem;

   while (length-- > 0)
      m_buffer.push_back(*pu8Data++);
}

void ByteBuffer::vAppendItem(const ByteBuffer &oBuffer)
{
   m_buffer.insert(m_buffer.end(), oBuffer.m_buffer.begin(), oBuffer.m_buffer.end());
}

unsigned ByteBuffer::uiReadItem(uint8 *puBuffer, size_t length)
{
   unsigned uiReadPos = m_uiCurrPos;

   // Enough data inside the buffer?
   if (length <= get_fill_level())
   {
      while (length-- > 0)
         *puBuffer++ = m_buffer[uiReadPos++];
   }

   return uiReadPos - m_uiCurrPos;
}

unsigned ByteBuffer::uiReadItem(ByteBuffer *poBuffer, size_t length)
{
   unsigned uiReadPos = m_uiCurrPos;

   // Enough data inside the buffer?
   if (length <= get_fill_level())
   {
      while (length-- > 0)
         poBuffer->m_buffer.push_back(m_buffer[uiReadPos++]);
   }

   return uiReadPos - m_uiCurrPos;
}


bool ByteBuffer::boGetNextItem(void *pvItem, size_t length)
{
   unsigned uiBytesRead = uiReadItem((uint8*) pvItem, length);
   m_uiCurrPos += uiBytesRead;
   return uiBytesRead > 0;
}

bool ByteBuffer::boGetNextItem(ByteBuffer *poBuffer, size_t length)
{
   unsigned uiBytesRead = uiReadItem(poBuffer, length);
   m_uiCurrPos += uiBytesRead;
   return uiBytesRead > 0;
}


bool ByteBuffer::boPeekNextItem(void *pvItem, size_t length)
{
   unsigned uiBytesRead = uiReadItem((uint8*) pvItem, length);
   return uiBytesRead > 0;
}

bool ByteBuffer::boPeekNextItem(ByteBuffer *poBuffer, size_t length)
{
   unsigned uiBytesRead = uiReadItem(poBuffer, length);
   return uiBytesRead > 0;
}
