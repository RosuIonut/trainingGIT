#ifndef TT_SYSTEM_H
#define TT_SYSTEM_H

/*initalisation and timer functions for ST when running in the Legacy module*/
void TT_vLegacyInitReset(void);

/*initalisation and timer functions for ST when running in the HMI module (as CF-Component)*/
void TT_vHMIInitReset(void);

/*cyclic 100ms timer function used when ST-Component is running in Legacy AND HMI modules*/
void TT_vCyclic100ms(void);
void TT_vCyclic10ms(void);

#endif

