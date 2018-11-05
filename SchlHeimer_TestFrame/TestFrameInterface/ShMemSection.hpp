#ifndef SHMEMSECTION_HPP
#define SHMEMSECTION_HPP

#include "ByteBuffer.hpp"

// A ShMemSection represents a section in the shared memory block and is an
// interface that must be overloaded by specific section classes.
//
// The specific classes have data items that must be serialized and
// deserialized when dealing with shared memory blocks. The knowledge about
// this is section-specific and therefore must be implemented individually
// by each specific section class.

class ShMemSection
{
public:
   ShMemSection(uint8 u8Type) :
      m_u8Type(u8Type)
   {
   }

   virtual ~ShMemSection()
   {
   }

   uint8 u8GetType(void) const
   {
      return m_u8Type;
   }

   // Creates a section by serializing internal data items
   virtual void vSerialize(ByteBuffer *poBuffer) const = 0;
   // Fills internal data items by reading from the ByteBuffer
   virtual bool boDeserialize(ByteBuffer *poBuffer) = 0;

private:
   uint8 m_u8Type;
};

#endif
