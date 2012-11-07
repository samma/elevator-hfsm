/*****************************************************************************
* QHsmTst example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 05, 2008
* Update for linux execution Oct 27,2009 FB
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#ifndef qpn_port_h
#define qpn_port_h

#define Q_ROM

#define Q_PARAM_SIZE            1

#define Q_SIGNAL_SIZE		2

#define QF_TIMEEVT_CTR_SIZE     2

#define Q_NFSM
/*#define QF_FSM_ACTIVE*/

#define QF_MAX_ACTIVE           4


#define QF_INT_LOCK()           cli()
#define QF_INT_UNLOCK()         sti()

#include <stdint.h>
#include "qepn.h"              /* QEP-nano platform-independent header file */

#endif                                                        /* qpn_port_h */
