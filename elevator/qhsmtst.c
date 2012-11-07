/*****************************************************************************
 * QHsmTst example
 * Last Updated for Version: 4.0.00
 * Date of the Last Update:  Apr 07, 2008
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
#include "qpn_port.h"
#include "qhsmtst.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

extern int timePassed;
extern int currentFloor ;
extern enum direction direct;
//extern int maxFloor;
extern int elevOrders [5];
extern int timeBetweenOrders = 30;

int c;

//struct storeWaitTimes waiting [5];


/* QHsmTst class -----------------------------------------------------------*/

typedef struct QHsmTstTag {
    QHsm super;                                        /* extend class QHsm */
                                             /* extended state variables... */



} QHsmTst;

void   QHsmTst_ctor(void);                               /* the ctor */
static QState QHsmTst_initial(QHsmTst *me);  /* initial pseudostate-handler */
static QState QHsmTst_WAITING    (QHsmTst *me);                  /* state-handler */
static QState QHsmTst_MOVING   (QHsmTst *me);                  /* state-handler */
static QState QHsmTst_SUPER (QHsmTst *me);

/* global objects ----------------------------------------------------------*/

QHsmTst HSM_QHsmTst;                /* the sole instance of the QHsmTst HSM */



/*..........................................................................*/
void QHsmTst_ctor(void) {
	QHsm_ctor(&HSM_QHsmTst.super, (QStateHandler)&QHsmTst_initial);
/*
	HSM_QHsmTst.timePassed = 0;
	HSM_QHsmTst.currentFloor = 0;
	HSM_QHsmTst.maxFloor = 4;
	HSM_QHsmTst.direct = NA;


*/
}




/*..........................................................................*/
QState QHsmTst_initial(QHsmTst *me) {
	BSP_display("top-INIT;");
	//srandom (time (0));
	srandom (1);
	//initWaiting();
	return Q_TRAN(&QHsmTst_WAITING);
}



QState QHsmTst_SUPER(QHsmTst *me) {
	switch (Q_SIG(me)) {

	case Q_ENTRY_SIG: {
		BSP_display("super-ENTRY;");
		return Q_HANDLED();
	}
	case Q_EXIT_SIG: {
		BSP_display("super-EXIT;");
		return Q_HANDLED();
	}
	case Q_INIT_SIG: {
		BSP_display("super-INIT;");
		return Q_TRAN(&QHsmTst_WAITING);
	}


	}
	BSP_display("Super return to top");
	return Q_SUPER(&QHsm_top);
}

/*..........................................................................*/

QState QHsmTst_WAITING(QHsmTst *me) {
	//BSP_display(me->timePassed);
	switch (Q_SIG(me)) {

	case Q_ENTRY_SIG: {
		tick5();
		tick5();
		BSP_display("timePassed10;");
		BSP_display("waiting-ENTRY;");
		return Q_HANDLED();
	}
	case Q_EXIT_SIG: {

		BSP_display("waiting-EXIT;");
		return Q_HANDLED();
	}

	case GO_DOWN: {
		BSP_display("received-go-down;");
		direct = DOWN;

		//BSP_display(me->timePassed);
		return Q_TRAN(&QHsmTst_MOVING);
	}
	case GO_UP: {

		BSP_display("received-go-up;");
		direct = UP;
		//BSP_display(me->timePassed);
		return Q_TRAN(&QHsmTst_MOVING);
	}
	}
	return Q_SUPER(&QHsmTst_SUPER);
}
/*..........................................................................*/

QState QHsmTst_MOVING(QHsmTst *me) {
	switch (Q_SIG(me)) {

	case Q_ENTRY_SIG: {
		BSP_display("moving-ENTRY;");
		tick5();
		return Q_HANDLED();
	}
	case Q_EXIT_SIG: {
		BSP_display("moving-EXIT;");
		return Q_HANDLED();
	}

	case FLOOR_REACHED: {
		BSP_display("floor-reached;");
		switch (direct){
		case UP: {
			currentFloor++;
			BSP_display("floorIncrease");
			break;
			//BSP_display(me->currentFloor);
		}
		case DOWN:{
			currentFloor--;
			BSP_display("floorDecreased");
			break;
			//BSP_display(me->currentFloor);
		}
		case NA:{

			BSP_display("error in moving");
			break;
		}

		}

		arrivedAtFloor(currentFloor,waiting);

		return Q_TRAN(&QHsmTst_WAITING);
	}
	}
	return Q_SUPER(&QHsmTst_SUPER);
}
/*..........................................................................*/
void tick5(){
	timePassed += 5;
	pressButtonMaybe();

}

void pressButtonMaybe(){
	int rando;
	static int nextTimeToOrder = 10;
	if (timePassed == nextTimeToOrder){
		nextTimeToOrder += timeBetweenOrders;
		rando = rand() % 5;
		orderedToFloor(rando,waiting);
		//printf("order assigned: %d ",rando);
	}
}


void arrivedAtFloor(int floor,storeWait  * waiting){
	int j;
	for (j = 0; j < waiting[floor].indexTracker; j++){
		waiting[floor].sumTimeWaited += (timePassed - waiting[floor].currentlyWaiting[j]);

		//printf("time passed arrival %d", timePassed);
		//printf("currentwait %d", waiting[floor].currentlyWaiting[j]);
		//printf("sumtimewaited %d", waiting[floor].sumTimeWaited);

		waiting[floor].currentlyWaiting[j] = 0;
	}
	waiting[floor].indexTracker = 0;
	elevOrders[floor] = false;
}

void orderedToFloor(int floor, storeWait  * waiting){
	elevOrders[floor] = true;
	waiting[floor].currentlyWaiting[waiting[floor].indexTracker] = timePassed;
	waiting[floor].indexTracker++;

	//printf("time passed %d", timePassed);
	//printf("currentwait %d", waiting[floor].currentlyWaiting[waiting[floor].indexTracker]);
	//printf("sumtimewaited %d", waiting[floor].sumTimeWaited);

	waiting[floor].buttonPressCount ++;
}
//
void printStatistics(storeWait  * waiting){
	int k ;
	for (k = 0; k < 5; k++){
		//arrivedAtFloor(k,&waiting);
		printf("getting stats; average time to wait in floor %d is: %f\n",k,(((double)waiting[k].sumTimeWaited)/waiting[k].buttonPressCount));
	}
}

void printCurrWait(storeWait  * waiting){
	printf("time passed %d", timePassed);
	printf("currentwait %d", waiting[0].currentlyWaiting[0]);
	printf("sumtimewaited %d", waiting[0].sumTimeWaited);
}
/*
void initWaiting(){
	storeWait waiting0;
	storeWait waiting1;
	storeWait waiting2;
	storeWait waiting3;
	storeWait waiting4;
	waiting0 = initInstanceWaiting(waiting0);
	waiting1 = initInstanceWaiting(waiting1);
	waiting2 = initInstanceWaiting(waiting2);
	waiting3 = initInstanceWaiting(waiting3);
	waiting4 = initInstanceWaiting(waiting4);

	waiting[0] = waiting0;
	waiting[1] = waiting1;
	waiting[2] = waiting2;
	waiting[3] = waiting3;
	waiting[4] = waiting4;

}
*/
/*
storeWait initInstanceWaiting(storeWait * waiting){
	int k;
	wait.buttonPressCount = 0;
	for(k = 0; k<50; k++){
		wait.currentlyWaiting[k] = 0;
	}
	wait.indexTracker = 0;
	wait.sumTimeWaited = 0;
	return wait;
}
*/
