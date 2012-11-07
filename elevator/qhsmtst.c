//Elevator state machine
//Three states. One super state to handle unknown signals
//One wait state which is when the elvator stands still at some floor
//One moce state which is when the elvator moves between floors
//The time in this system is event driven

#include "qpn_port.h"m
#include "qhsmtst.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

extern int timePassed;
extern int currentFloor ;
extern enum direction direct;
//extern int maxFloor;
extern int elevOrders [5];
extern int timeBetweenOrders = 200;
//10, 25,50, 200


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
}

/*..........................................................................*/
QState QHsmTst_initial(QHsmTst *me) {
    BSP_display("top-INIT;");
    srandom (time (0));
    //srandom (1);
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
    switch (Q_SIG(me)) {

    case Q_ENTRY_SIG: {
	if(elevOrders[currentFloor]){
	    arrivedAtFloor(currentFloor,waiting);
	    tick5();
	    tick5();
	    BSP_display("timePassed10;");
	    BSP_display("waiting-ENTRY;");
	    arrivedAtFloor(currentFloor,waiting);

	}
	return Q_HANDLED();
    }
    case Q_EXIT_SIG: {

	BSP_display("waiting-EXIT;");
	return Q_HANDLED();
    }

    case GO_DOWN: {
	BSP_display("received-go-down;");
	direct = DOWN;
	return Q_TRAN(&QHsmTst_MOVING);
    }
    case GO_UP: {

	BSP_display("received-go-up;");
	direct = UP;
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
	}
	case DOWN:{
	    currentFloor--;
	    BSP_display("floorDecreased");
	    break;
	}
	case NA:{

	    BSP_display("error in moving");
	    break;
	}
	}
	//arrivedAtFloor(currentFloor,waiting);

	return Q_TRAN(&QHsmTst_WAITING);
    }
    }
    return Q_SUPER(&QHsmTst_SUPER);
}
/*..........................................................................*/
void tick5(){
    timePassed += 5;
    pressButtonMaybe();

    if(elevOrders[currentFloor]){
	arrivedAtFloor(currentFloor,waiting);
    }

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
	if(	waiting[floor].currentlyWaiting[j] != 0){
	    waiting[floor].sumTimeWaited += (timePassed - waiting[floor].currentlyWaiting[j]);
	    waiting[floor].currentlyWaiting[j] = 0;
	}
    }
    waiting[floor].indexTracker = 0;
    elevOrders[floor] = false;
}

void orderedToFloor(int floor, storeWait  * waiting){
    elevOrders[floor] = true;
    waiting[floor].currentlyWaiting[waiting[floor].indexTracker] = timePassed;
    waiting[floor].indexTracker++;
    waiting[floor].buttonPressCount ++;
}

void printStatistics(storeWait  * waiting){
    int k ;
    printf("time between orders: %d\n",timeBetweenOrders);
    for (k = 0; k < 5; k++){
	arrivedAtFloor(k,waiting);
	printf("getting stats; average time to wait in floor %d is: %f\n",k,(((double)waiting[k].sumTimeWaited)/waiting[k].buttonPressCount));
    }
}

void printCurrWait(storeWait  * waiting){
    printf("time passed %d", timePassed);
    printf("currentwait %d", waiting[0].currentlyWaiting[0]);
    printf("sumtimewaited %d", waiting[0].sumTimeWaited);
}
