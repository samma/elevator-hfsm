/*****************************************************************************
* Product: QEP-nano public interface
* Last Updated for Version: 4.0.02
* Date of the Last Update:  Nov 11, 2008
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
#ifndef qepn_h
#define qepn_h

/**
* \file
* \ingroup qepn qfn qkn
* \brief Public QEP-nano interface.
*
* This header file must be included in all modules that use QP-nano.
* Typically, this header file is included indirectly through the
* header file qpn_port.h.
*/

/****************************************************************************/
#ifndef Q_ROM             /* if NOT defined, provide the default definition */

    /** \brief Macro to specify compiler-specific directive for placing a
    * constant object in ROM.
    *
    * Many compilers for 8-bit Harvard-architecture MCUs provide non-stanard
    * extensions to support placement of objects in different memories.
    * In order to conserve the precious RAM, QP-nano uses the Q_ROM macro for
    * all constant objects that can be allocated in ROM.
    *
    * To override the following empty definition, you need to define the
    * Q_ROM macro in the qpn_port.h header file. Some examples of valid
    * Q_ROM macro definitions are: __code (IAR 8051 compiler), code (Keil
    * 8051 compiler), PROGMEM (gcc for AVR), __flash (IAR for AVR).
    */
    #define Q_ROM
#endif
#ifndef Q_ROM_VAR         /* if NOT defined, provide the default definition */

    /** \brief Macro to specify compiler-specific directive for accessing a
    * constant object in ROM.
    *
    * Many compilers for 8-bit MCUs provide different size pointers for
    * accessing objects in various memories. Constant objects allocated
    * in ROM (see #Q_ROM macro) often mandate the use of specific-size
    * pointers (e.g., far pointers) to get access to ROM objects. The
    * macro Q_ROM_VAR specifies the kind of the pointer to be used to access
    * the ROM objects.
    *
    * To override the following empty definition, you need to define the
    * Q_ROM_VAR macro in the qpn_port.h header file. An example of valid
    * Q_ROM_VAR macro definition is: __far (Freescale HC(S)08 compiler).
    */
    #define Q_ROM_VAR
#endif
#ifndef Q_REENTRANT       /* if NOT defined, provide the default definition */

    /** \brief Macro to specify compiler-specific directive for generating
    * reentrant function.
    *
    * Some compilers for 8-bit MCUs provide, most notably the Keil C51
    * compiler for 8051, don't generate ANSI-C compliant reentrant functions
    * by default, due to the limited CPU architecture. These compilers allow
    * dedicating specific functions to be reentrant with a special extended
    * keyword (such as "reentrant" for Keil C51). The macro Q_REENTRANT is
    * defined to nothing by default, to work with ANSI-C compliant compilers,
    * but can be defined to "reentrant" to work with Keil C51 and perhaps
    * other embedded compilers.
    */
    #define Q_REENTRANT
#endif

/****************************************************************************/
/** helper macro to calculate static dimension of a 1-dim array \a array_ */
#define Q_DIM(array_) (sizeof(array_) / sizeof(array_[0]))

/****************************************************************************/
/** \brief get the current QP version number string
*
* \return version of the QP as a constant 6-character string of the form
* x.y.zz, where x is a 1-digit major version number, y is a 1-digit minor
* version number, and zz is a 2-digit release number.
*/
char const Q_ROM * Q_ROM_VAR QP_getVersion(void);

/** \brief Scalar type describing the signal of an event.
*/
typedef uint8_t QSignal;

/****************************************************************************/
#ifndef Q_PARAM_SIZE
    /** \brief macro to define the size of event parameter.
    * Valid values 0, 1, 2, or 4; default 0
    */
    #define Q_PARAM_SIZE 0
#endif
#if (Q_PARAM_SIZE == 0)
#elif (Q_PARAM_SIZE == 1)

    /** \brief type of the event parameter.
    *
    * This typedef is configurable via the preprocessor switch #Q_PARAM_SIZE.
    * The other possible values of this type are as follows: \n
    * none when (Q_PARAM_SIZE == 0); \n
    * uint8_t when (Q_PARAM_SIZE == 1); \n
    * uint16_t when (Q_PARAM_SIZE == 2); and \n
    * uint32_t when (Q_PARAM_SIZE == 4).
    */
    typedef uint8_t QParam;
#elif (Q_PARAM_SIZE == 2)
    typedef uint16_t QParam;
#elif (Q_PARAM_SIZE == 4)
    typedef uint32_t QParam;
#else
    #error "Q_PARAM_SIZE defined incorrectly, expected 0, 1, 2, or 4"
#endif

/** \brief Event structure.
*
* QEvent represents events, optionally with a single scalar parameter.
* \sa Q_PARAM_SIZE
* \sa ::QParam
*/
typedef struct QEventTag {
    QSignal sig;                                   /**< signal of the event */
#if (Q_PARAM_SIZE != 0)
    QParam par;                          /**< scalar parameter of the event */
#endif
} QEvent;

/****************************************************************************/
/** \brief QP reserved signals */
enum QReservedSignals {
    Q_ENTRY_SIG = 1,                   /**< signal for coding entry actions */
    Q_EXIT_SIG,                         /**< signal for coding exit actions */
    Q_INIT_SIG,           /**< signal for coding nested initial transitions */
    Q_TIMEOUT_SIG,                          /**< signal used by time events */
    Q_USER_SIG      /**< first signal that can be used in user applications */
};

/****************************************************************************/

/** \brief Type returned from  a state-handler function. */
typedef uint8_t QState;

                       /** \brief the signature of a state handler function */
typedef QState (*QStateHandler)(void *me);

/** \brief Finite State Machine.
*
* QFsm represents a traditional non-hierarchical Finite State Machine (FSM)
* without state hierarchy, but with entry/exit actions.
*
* \note QFsm is not intended to be instantiated directly, but rather serves
* as the base structure for derivation of state machines in the application
* code.
*
* The following example illustrates how to derive a state machine structure
* from QFsm. Please note that the QFsm member super_ is defined as the FIRST
* member of the derived struct.
* \include qepn_qfsm.c
*
* \sa \ref derivation
*/
typedef struct QFsmTag {
    QStateHandler state;                /**< current active state (private) */
    QEvent evt;       /**< currently processed event in the FSM (protected) */
} QFsm;

/** \brief macro to access the signal of the current event of a state machine
*
* \sa ::QFsm ::QHsm
*/
#define Q_SIG(me_)  (((QFsm *)(me_))->evt.sig)

#if (Q_PARAM_SIZE != 0)
/** \brief macro to access the parameter of the current event of
* a state machine
*
* \sa ::QFsm ::QHsm Q_PARAM_SIZE
*/
#define Q_PAR(me_)  (((QFsm *)(me_))->evt.par)
#endif

#ifndef Q_NFSM

/** \brief State machine constructor.
*
* \param me_ pointer the state machine structure derived from ::QHsm.
* \param initial_ is the pointer to the initial state of the state machine.
* \note Must be called only ONCE before taking the initial transition
* with QFsm_init() and dispatching any events via QFsm_dispatch().
*/
#define QFsm_ctor(me_, initial_) ((me_)->state = (initial_))

/** \brief Initializes a FSM
*
* Takes the top-most initial transition in a FSM.
* \param me is the pointer the state machine structure derived from ::FHsm.
*
* \note Must be called only ONCE after QFsm_ctor() and before any calls
* to QFsm_dispatch().
*/
void QFsm_init(QFsm *me);

#ifndef QK_PREEMPTIVE
    /** \brief Dispatches an event to a FSM
    *
    * Processes one event at a time in Run-to-Completion fashion. The argument
    * \a me is the pointer the state machine structure derived from ::QFsm.
    *
    * \note Must be called after QFsm_init().
    */
    void QFsm_dispatch(QFsm *me);
#else
    void QFsm_dispatch(QFsm *me) Q_REENTRANT;
#endif

#endif                                                            /* Q_NFSM */


/** \brief Value returned by a state-handler function when it handles
* the event.
*/
#define Q_RET_HANDLED       ((QState)0)

/** \brief Value returned by a state-handler function when it handles
* the event.
*
* You call that macro after the return statement (return Q_HANDLED();)
* Q_HANDLED() can be used both in the FSMs and HSMs.
*
* \include qepn_qfsm.c
*/
#define Q_HANDLED()         (Q_RET_HANDLED)

/** \brief Value returned by a non-hierarchical state-handler function when
* it ignores (does not handle) the event.
*/
#define Q_RET_IGNORED       ((QState)1)

/** \brief The macro returned from a non-hierarchical state-handler function
* when it ignores (does not handle) the event.
*
* You call that macro after the return statement (return Q_IGNORED();)
*
* \include qepn_qfsm.c
*/
#define Q_IGNORED()         (Q_RET_IGNORED)

/** \brief Value returned by a state-handler function when it takes a
* regular state transition.
*/
#define Q_RET_TRAN          ((QState)2)

/** \brief Designates a target for an initial or regular transition.
* Q_TRAN() can be used both in the FSMs and HSMs.
*
* \include qepn_qtran.c
*/
/*lint -e960 */     /* ignore MISRA Rule 42 (comma operator) for this macro */
#define Q_TRAN(target_)  \
    (((QFsm *)me)->state = (QStateHandler)(target_), Q_RET_TRAN)

/** \brief Value returned by a state-handler function when it cannot
* handle the event.
*/
#define Q_RET_SUPER         ((QState)3)

/** \brief Designates the superstate of a given state in an HSM.
*
* \include qepn_qhsm.c
*/
/*lint -e960 */     /* ignore MISRA Rule 42 (comma operator) for this macro */
#define Q_SUPER(super_)  \
    (((QFsm *)me)->state = (QStateHandler)(super_),  Q_RET_SUPER)


/****************************************************************************/
#ifndef Q_NHSM

/** \brief a Hierarchical State Machine.
*
* QHsm represents a Hierarchical Finite State Machine (HSM). QHsm
* extends the capabilities of a basic FSM with state hierarchy.
*
* \note QHsm is not intended to be instantiated directly, but rather serves
* as the base structure for derivation of state machines in the application
* code.
*
* The following example illustrates how to derive a state machine structure
* from QHsm. Please note that the QHsm member super_ is defined as the FIRST
* member of the derived struct.
* \include qepn_qhsm.c
*
* \sa \ref derivation
*/
typedef struct QFsmTag QHsm;

/* public methods */
/** \brief State machine constructor.
*
* \param me_ pointer the state machine structure derived from ::QHsm.
* \param initial_ is the pointer to the initial state of the state machine.
* \note Must be called only ONCE before taking the initial transition
* with QHsm_init() and dispatching any events via QHsm_dispatch().
*/
#define QHsm_ctor(me_, initial_) ((me_)->state  = (initial_))

/** \brief Initializes a HSM.
*
* Takes the top-most initial transition in a HSM.
* \param me is the pointer the state machine structure derived from ::QHsm.
*
* \note Must be called only ONCE after QHsm_ctor() and before any calls
* to QHsm_dispatch().
*/
void QHsm_init(QHsm *me);

#ifndef QK_PREEMPTIVE
    /** \brief Dispatches an event to a HSM
    *
    * Processes one event at a time in Run-to-Completion fashion.
    * \a me is the pointer the state machine structure derived from ::QHsm.
    *
    * \note Must be called repetitively for each event after QHsm_init().
    */
    void QHsm_dispatch(QHsm *me);
#else
    void QHsm_dispatch(QHsm *me) Q_REENTRANT;
#endif

/* protected methods... */

/** \brief The top-state.
*
* QHsm_top() is the ultimate root of state hierarchy in all HSMs derived
* from ::QHsm. This state handler always returns (QState)0, which means
* that it "handles" all events.
*
* \sa Example of the QCalc_on() state handler for Q_INIT().
*/
QState QHsm_top(QHsm *me);

#endif                                                            /* Q_NHSM */

#include "qassert.h"            /* include the QP-nano assertions (for DbC) */

#endif                                                            /* qepn_h */
