#ifndef SHMEMBLOCK_HPP
#define SHMEMBLOCK_HPP

extern "C"
{
#include "cdef.h"
}

#include <vector>       //for std::vector
#include <functional>   //for std::reference_wrapper used in the parameter list of the constructor

#include "ShMemSection.hpp"

// The ShMemBlock is the internal data structure that represents a complete
// shared memory block. It is constructed by adding sections and serializing
// it afterwards into a ByteBuffer. In the opposite direction it can be
// created from a ByteBuffer and individual sections can be extracted.

class ShMemBlock
{
public:
   // Default constructor
   ShMemBlock();
   // Constructor which initialises the sections m_sections with the parameter sections
   ShMemBlock(std::vector<std::reference_wrapper<const ShMemSection>>& sections);
   // Destructor
   virtual ~ShMemBlock();

   // Add a section by copying data into internal buffers
   void vAddSection(const ShMemSection &oSection);
   // Get section data based on u8Type (copies data from internal buffer)
   bool boGetSection(ShMemSection *poSection);
   // Create a shared memory block from stored sections
   void vSerialize(ByteBuffer *poBuffer);
   // Create sections from a shared memory block
   bool boDeserialize(ByteBuffer *poBuffer);

private:
   typedef struct {
      uint8 u8Type;
      ByteBuffer oData;
   } tstSection;
   
   //sections for tranmission to the client (system under test) via shared memory
   std::vector<tstSection> m_sections;
};

#endif
