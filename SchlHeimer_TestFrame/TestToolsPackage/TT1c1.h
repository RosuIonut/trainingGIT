#ifndef TT_C1_H
#define TT_C1_H
/***************************************************************************
*=====================      Copyright by Continental AG      ===============
****************************************************************************
* Title        : TT1c1.h
*
* Description  : Interface to the TestTools package
*
* Environment  : VAG FPK, currently tested with PC based simulation only
*
*
* Responsible  : Felix Opatz, Schleissheimer GmbH
*
* Guidelines   : SMK 4.11
*
* Template name: OOLite Interface, Revision 1.0
*
* CASE-Tool    : Together
*
* Revision List: (Will be filled by PVCS)
* Archive: $Log$
* End of History
****************************************************************************/

/****************************************************************************
** Header Files (Only those that are needed in this file)
****************************************************************************/

/* System Header Files */

/* Foreign headerfiles */

/* Own headerfiles */
#ifdef __cplusplus
extern "C" {
#endif

#include "TT_Interface.h"

#ifdef __cplusplus
}
#endif

/***************************************************************************
* CLASS DEFINITION
***************************************************************************/
#ifdef TOGETHER /* open C++ class */

/**
 * Package TT Exportinterface
 * @interface
 * @stereotype Facade
 */
class TT1C1: public TT_Interface {
private:
};
#endif

#endif /* TT_C1_H */
