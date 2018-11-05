#ifndef SHMEM_LAYOUT_H
#define SHMEM_LAYOUT_H

#ifdef WIN32
/* Set struct alignment to 1 */
#pragma pack(push, 1)
#endif

#include "cdef.h"

/* Global layout of a shared memory object
 *
 * +------------------------
 * | Shared Memory Header  |
 * +-----------------------+
 * | Section Descriptor  1 |
 * +-----------------------+
 * | Section Descriptor  2 |
 * +-----------------------+
 * | Section Descriptor  N |
 * +-----------------------+
 * | Section Data 1        |
 * |                       |
 * |                       |
 * +-----------------------+
 * | Section Data 2        |
 * +-----------------------+
 * | Section Data N        |
 * |                       |
 * +-----------------------+
 */

/* Maximum size of the shared object, needed for memory mapping */
#define SHMEM_nMaxSharedObjectSize     0x100000  /* 1 MB */
/* Shared Memory Header
 *
 * Very first data item in a shared memory object.
 * Indicates version of memory layout that was used to create the object
 * and announces how many sections are included.
 * For convenience the header also contains the information how big the
 * complete object is, although it could be calculated from the individual
 * sections' sizes.
 */
#define SHMEM_nLayoutVersion           0x01

typedef struct
{
   uint8 u8Version;                    /* version of memory layout used */
   uint8 u8SectCount;                  /* number of sections */
   uint32 u32ObjSize;                  /* complete object's size */
} SHMEM_tstShMemHeader;


/* Section Descriptor
 *
 * Pointer like structure that declares a section (type) and tells how
 * this sections is found (offset and length).
 */
#define SHMEM_nSectTimingTest          0x01
#define SHMEM_nSectSequencer           0x02
#define SHMEM_nSectTraceLog            0x03
#define SHMEM_nSectObjectPatches       0x04

typedef struct
{
   uint8 u8Type;                       /* Section type */
   uint32 u32Offset;                   /* Abs. offset in object in bytes */
   uint32 u32Length;                   /* Length of section in bytes */
} SHMEM_tstSectDescr;


/* Section for timing test
 *
 * The timing test checks how fast the target executes CAM cyclic tasks in
 * relation to the system time. This gives a clue about the general "lag"
 * of the simulation.
 */
typedef struct
{
   uint8 u8CycleCount100ms;            /* Number of 100ms cycles to test */
   uint16 u16MsNeeded;                 /* Number of milliseconds needed */
} SHMEM_tstTimingTest;


/* Section for sequencer
 *
 * The sequencer contains a set of commands which are executed in the order
 * of their occurrence. Each command has an action code (e.g. "key press")
 * and a fixed width argument (currently uint16). The fixed width argument
 * was chosen for simplicity when parsing through the sequence.
 * The design idea is that for each action code a handler is written that
 * knows implicitly how the argument must be understood (e.g. the sleep
 * handler know that it finds the number of milliseconds to sleep there).
 */
#define SHMEM_nSeqCodeNull             0x00
#define SHMEM_nSeqCodeSleep            0x01
#define SHMEM_nSeqCodeKeyPress         0x02
#define SHMEM_nSeqCodeKeyRelease       0x03
#define SHMEM_nSeqCodeWarnRequest      0x04
#define SHMEM_nSeqCodeWarnRelease      0x05
#define SHMEM_nSeqCodeExecFunction     0x06
#define SHMEM_nSeqCodeHmiWarn          0x07
#define SHMEM_nSeqCodeHmiWarnOff       0x08
#define SHMEM_nSeqCodeStartSync        0x0A

typedef struct
{
   uint16 u16CmdCount;                 /* Number of commands in sequence */
} SHMEM_tstSeqHeader;

typedef struct
{
   uint8  u8Processed;    /* Number of clients which processed this command - 20170118 wti */
   uint8  u8ActionCode;   /* Action code */
   uint16 u16Argument;    /* Argument (fixed width) */
   uint16 u16Argument2;   /* 2nd argument added - 20170306 wti */
} SHMEM_tstSeqCommand;

/* Key codes used as argument for nSeqCodeKeyPress and nSeqCodeKeyRelease */
/* 20170120 wti: adapted for FPK-Entry */
#define SHMEM_nSeqKey_MFL_MENU         01 //old: 0x06
#define SHMEM_nSeqKey_MFL_NEXT         02 //old: 0x01
#define SHMEM_nSeqKey_MFL_PREV         03 //old: 0x02
#define SHMEM_nSeqKey_MFL_UP           04 //old: 0x03
#define SHMEM_nSeqKey_MFL_DOWN         05 //old: 0x04
#define SHMEM_nSeqKey_MFL_OK           07 //old: 0x05
#define SHMEM_nSeqKey_MFL_ESC          08 //Cancel / no old
#define SHMEM_nSeqKey_MFL_FAS          12 //old: 0x07

/* Warning codes used as argument for
 * SHMEM_nSeqCodeWarnRequest and SHMEM_nSeqCodeWarnRelease
 */
// #define SHMEM_nSeqWarn_AirbagSystemf   0x01

/* Function IDs used as argument for SHMEM_nSeqCodeExecFunction */
/* Moved to TT_ExecFunctions.hpp */

/* Section for trace
 *
 * The trace section contains a static configuration specified by the host
 * and free space which will be filled by the client with actual trace data.
 *
 * The configuration specifies which trace item is active. The target software
 * needs to check this configuration for every trace action, thus it must be
 * fast. This is implemented by using a fixed index into a data field.
 * Each trace item can have up to 4 arguments. Unused arguments default to 0.
 * To distinguish "real" zeros from default zeros, the number of expected
 * arguments is also specified in the configuration.
 *
 * There is an optional filter section to reduce the trace traffic. See
 * block comment below for details.
 *
 * Thus the overall layout of the trace section is:
 *
 * +--------------------------------+
 * | Trace header                   |
 * | - Item configuration           |
 * | - Size of filter section       |
 * | - Maximum size of trace buffer |
 * | - Used space of trace buffer   |
 * +--------------------------------+
 * | Filter section                 |
 * | (optional)                     |
 * +--------------------------------+
 * | Trace buffer                   |
 * | (filled by target)             |
 * +--------------------------------+
 */
#include "SHMEM_TraceItems.h"

/* Item types */
#define SHMEM_nTraceItemTypeEvent      0x01
#define SHMEM_nTraceItemTypeData       0x02
#define SHMEM_nTraceItemTypeBlock      0x03

/* Tracing modes */
#define SHMEM_nTraceEventDisabled      0x00
#define SHMEM_nTraceEventEnabled       0x01
#define SHMEM_nTraceDataNotifyNever    0x00
#define SHMEM_nTraceDataNotifyAlways   0x01
#define SHMEM_nTraceDataNotifyOnChange 0x02
#define SHMEM_nTraceBlockDisabled      0x00
#define SHMEM_nTraceBlockEnabled       0x01

/* Special filter index for "no filter" */
#define SHMEM_nTraceNoFilter           0xFFFF

/* Static configuration of a trace item */
typedef struct
{
   uint8 u8Type;                       /* Type (event or data) */
   uint8 u8Mode;                       /* Tracing mode (type dependent) */
   uint8 u8ArgCount;                   /* Number of arguments (0 to 4) */
   uint16 u16FilterIndex;              /* Filter start index, or 0xFFFF */
} SHMEM_tstTraceItemCfg;

/* Flag to signal that buffer was exhausted */
#define SHMEM_nTraceFlagBufferFull     0x01

/* Header of the trace item section */
typedef struct
{
   /* Configuration array for items (ID used as index) */
   SHMEM_tstTraceItemCfg astItemCfg[SHMEM_nenTraceItemCount]; 
   uint16 u16FilterEntries;            /* Length of filter section */
   uint32 u32BytesMax;                 /* Maximum number of bytes for trace */
   uint32 u32BytesUsed;                /* Number of bytes used for trace */
   uint8 u8Flags;                      /* Flags to signal result quality */
} SHMEM_tstTraceHeader;


/* Trace filtering
 *
 * Filters are defined in their disjunctive normal form, e.g.
 *    arg1 = 10 AND arg2 = 5
 * OR arg1 = 10 AND arg2 = 6
 * OR arg1 = 12
 *
 * Entry:    smallest piece of expression, e.g. "arg1 = 10"
 * Product:  one or more entries, combined by AND
 * Sum:      one or more products, combined by OR
 *
 * A filter thus is a sum of products of entries. The filter section includes
 * 0 to 65534 entries which are organized as a linear array, forming 0 or more
 * (65534 at most) filters. This is possible by having a flag for "end of
 * product" and "end of sum" for each entry.
 *
 * Example:          (P = end of product,  S = end of sum)
 *
 * 0        1        2        3        4        5
 * +---------+---------+---------+---------+---------+---------+
 * | exp1 .. | exp2 P. | exp3 PS | exp4 PS | exp5 .. | exp6 PS |
 * +---------+---------+---------+---------+---------+---------+
 *
 * We have 3 filters:
 *
 * A: exp1 AND exp2 OR exp3
 * B: exp4
 * C: exp5 AND exp6
 *
 * The trace items can now define their filters by specifying a filter index
 * different to 0xFFFF. These filter indices must point to the start of a
 * filter, e.g. 0, 3 oder 4 in the example above.
 */
#define SHMEM_nTraceFlagEndProduct     0x01
#define SHMEM_nTraceFlagEndSum         0x02
#define SHMEM_nTraceFlagNot            0x04
#define SHMEM_nTraceFlagAlwaysTrue     0x08

typedef struct
{
   uint8 u8Flags;                      /* Flags for end of product/sum */
   uint8 u8ArgIndex;                   /* Index of argument to match */
   uint32 u32ArgValue;                 /* Value this argument must match */
} SHMEM_tstTraceFilterEntry;


/* Layout of trace items
 *
 * A trace item can have variable length. For the case of arguments, this is
 * 0 to 4 arguments of type uint32. For blocks it is the block length.
 * Knowing the length of each item makes reading of the items easier (or, in
 * case of blocks, at all possible), as a known fixed-size header can be read
 * in advance and later on all information for reading the complete item is
 * known.
 */
typedef struct
{
   uint16 u16Id;                       /* ID of trace item */
   uint16 u16ArgSize;                  /* Size of payload in bytes */
   __int64  i64Timestamp;              /* Trace timestamp*/
} SHMEM_tstTraceEntryHeader;

/* Layout of a trace item record with uint32 arguments:
 *
 * 0      1        3      7     11     15     19
 * +------+--------+......:......:......:......:
 * |  ID  | Length | Arg1 : Arg2 : Arg3 : Arg4 :
 * +------+--------+......:......:......:......:
 *
 * The maximum number of arguments has been defined to 4. This is needed to
 * create the C++ interface code with a fixed number of (default) arguments,
 * which makes calling the function much more user-friendly.
 */
#define SHMEM_nTraceMaxArgCount        4

/*struct for storing return values for traces*/
typedef struct
{
   SHMEM_tstTraceEntryHeader stHeader;
   uint32 au32Args[SHMEM_nTraceMaxArgCount];
} SHMEM_tstTraceEntryArgs;

/* Layout of a trace item record of block type:
 *
 * 0      1        3          1027
 * +------+--------+............:
 * |  ID  | Length | block data :
 * +------+--------+............:
 *
 * The maximum length of block data has been defined to 1024. To have a known
 * maximum size makes memory handling easier.
 */
#define SHMEM_nTraceMaxBlockSize       1024

typedef struct
{
   SHMEM_tstTraceEntryHeader stHeader;
   uint8 au8Data[SHMEM_nTraceMaxBlockSize];
} SHMEM_tstTraceEntryBlock;


/* Section for Object Patches
 *
 * The Object Patch mechanism allows the test frame system to patch objects
 * (which could be as simple as an int, or as complex as an animation control)
 * at runtime.
 */
typedef struct
{
   uint8 u8PatchCount;
} SHMEM_tstObjPatchHeader;

/* The Object Type specifies the data type of the object as well as its
 * purpose (e.g. "StoryboardId" instead of "uint16").
 */
typedef enum
{
   SHMEM_nenObjType_Scene,
   SHMEM_nenObjType_EasingScene,
   SHMEM_nenObjType_EasingSceneSelf,
   SHMEM_nenObjType_AnimControl,
   SHMEM_nenObjType_ACFade,
   SHMEM_nenObjType_ACMove,
   SHMEM_nenObjType_ACScale,
   SHMEM_nenObjType_ANTFRuntimeCfg,
   SHMEM_nenObjTypeCount
} SHMEM_tenObjTypes;

/* The Property ID specifies a property within a object. The numeric values
 * need to be unique only in combination with the object type, but some
 * redundancy is not bad.
 */
typedef enum
{
   SHMEM_nenObjProp_Nothing,
   SHMEM_nenObjProp_NumOfFrames,
   SHMEM_nenObjProp_EasingTypeWpa,
   SHMEM_nenObjProp_StartAlpha,
   SHMEM_nenObjProp_EndAlpha,
   SHMEM_nenObjProp_StartPos,
   SHMEM_nenObjProp_EndPos,
   SHMEM_nenObjProp_StartFactorX,
   SHMEM_nenObjProp_StartFactorY,
   SHMEM_nenObjProp_EndFactorX,
   SHMEM_nenObjProp_EndFactorY,
   SHMEM_nenObjProp_IsActive1,
   SHMEM_nenObjProp_DataType1,
   SHMEM_nenObjProp_InstanceID1,
   SHMEM_nenObjProp_Duration1,
   SHMEM_nenObjProp_EasingType1,
   SHMEM_nenObjProp_EasingVariant1,
   SHMEM_nenObjProp_IsActive2,
   SHMEM_nenObjProp_DataType2,
   SHMEM_nenObjProp_InstanceID2,
   SHMEM_nenObjProp_Duration2,
   SHMEM_nenObjProp_EasingType2,
   SHMEM_nenObjProp_EasingVariant2,
   SHMEM_nenObjProp_IsActive3,
   SHMEM_nenObjProp_DataType3,
   SHMEM_nenObjProp_InstanceID3,
   SHMEM_nenObjProp_Duration3,
   SHMEM_nenObjProp_EasingType3,
   SHMEM_nenObjProp_EasingVariant3,
} SHMEM_tenObjPropIds;

/* A Patch Specification is an instruction of how an object can be identified
 * and how it should be modified.
 */
typedef struct
{
   uint8 u8ObjType;                    /* Type of object to patch */
   uint8 u8FilterProperty;             /* Property to use as filter */
   uint32 u32FilterValue;              /* Value that property must have */
   uint8 u8PatchProperty;              /* Property to patch */
   uint32 u32PatchValue;               /* Value to insert in property */
   uint8 u8ApplyCounter;               /* Counter for applications of patch */
} SHMEM_tstObjPatchSpec;


#ifdef WIN32
/* Restore previous struct alignment */
#pragma pack(pop)
#endif

#endif
