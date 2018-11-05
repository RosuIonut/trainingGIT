#ifndef TT__SEQUENCER_H
#define TT__SEQUENCER_H

#include "cdef.h"

void TT__vSequencerInit( void );
void TT__vSequencerOnDownload(   void *pvData, uint32 u32Length );
bool TT__boSequencerQueryUpload( void *pvData, uint32 u32Length );
void TT__vSequencerCyclic10ms( void );

#endif
