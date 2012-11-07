/*****************************************************************************
* QHsmTst example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 05, 2008
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
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>




/* local objects -----------------------------------------------------------*/
static FILE *l_outFile = (FILE *)0;
static void dispatch(QSignal sig);


extern int currentFloor ;
extern enum direction direct;
extern int maxFloor;
extern int elevOrders [5];
extern int timePassed;



int ordersExistInDir(int dir){
  int i;
  if (dir == UP){
    for( i = currentFloor+1; i <= maxFloor; ++i){
      if(elevOrders[i]){
	return true;
      }
    }
  }

  else if (dir == DOWN){
    for( i = currentFloor-1; i >= 0; --i){
      if(elevOrders[i]){
	return true;
      }
    }
  }

  else if (dir == NA){
    for( i = 0; i <= maxFloor; ++i){
      if(elevOrders[i]){
	return true;
      }
    }
  }
  return false;
}


/*..........................................................................*/
int main(int argc, char *argv[]) {



	int j;
	for (j = 0; j < 5 ; ++j){
		elevOrders[j] = false;
	}

    QHsmTst_ctor();                                  /* instantiate the HSM */

    if (argc > 1) {                                  /* file name provided? */
        l_outFile = fopen(argv[1], "w");
    }

    if (l_outFile == (FILE *)0) {                   /* interactive version? */
        l_outFile = stdout;            /* use the stdout as the output file */

        printf("QHsmTst example, built on %s at %s,\n"
               "QP-nano: %s.\nPress ESC to quit...\n",
               __DATE__, __TIME__, QP_getVersion());

        QHsm_init((QHsm *)&HSM_QHsmTst);    /* take the initial transitioin */

        for (;;) {                                            /* event loop */
            int c;

            printf("\n>");
            c = getchar();
            printf(": ");
            /*
            if ('a' <= c && c <= 'i') {                        // in range?
                Q_SIG((QHsm *)&HSM_QHsmTst) = (QSignal)(c - 'a' + A_SIG);
            }
	else if ('A' <= c && c <= 'I') {                   // in range?
                Q_SIG((QHsm *)&HSM_QHsmTst) = (QSignal)(c - 'A' + A_SIG);
            }
            else if (c == '\33') {                          // the ESC key?
                Q_SIG((QHsm *)&HSM_QHsmTst) = TERMINATE_SIG;
            }
            else {
                Q_SIG((QHsm *)&HSM_QHsmTst) = IGNORE_SIG;
            }
            */

            //QHsm_dispatch((QHsm *)&HSM_QHsmTst);      /* dispatch the event */
        }
    }
    else {                                                 /* batch version */
        printf("QHsmTst example, built on %s at %s, QP-nano %s\n"
               "output saved to %s\n",
               __DATE__, __TIME__, QP_getVersion(),
               argv[1]);

        fprintf(l_outFile, "QHsmTst example, QP-nano %s\n",
        		QP_getVersion());

        QHsm_init((QHsm *)&HSM_QHsmTst);    /* take the initial transitioin */




        extern storeWait waiting [5];
        int iterations;
        int timerCheckPoint = 0;

        direct = NA;
        maxFloor = 4;
        printStatus();
        for ( ; timePassed < 30000000; ){
        	timerCheckPoint = timePassed;

        	switch (direct){
        	case UP: {
        		if (ordersExistInDir(UP)){
        			dispatch(GO_UP);
        			dispatch(FLOOR_REACHED);
        		}
        		else {
        			direct = NA;
        		}
        		break;
        	}
        	case DOWN: {
        		if (ordersExistInDir(DOWN)){
        			dispatch(GO_DOWN);
        			dispatch(FLOOR_REACHED);
        		}
        		else {
        			direct = NA;
        		}
        		break;
        	}
        	case NA: {
        		if (ordersExistInDir(UP)){
        			dispatch(GO_UP);
        			dispatch(FLOOR_REACHED);
        		}
        		else if (ordersExistInDir(DOWN)){
        			dispatch(GO_DOWN);
        			dispatch(FLOOR_REACHED);
        		}
        		break;
        	}
        	}
        	//printCurrWait(waiting);
/*
        	if (iterations == 1 ){
        		//printf("time now: %d",timePassed);

        		orderedToFloor(4,waiting);
        	}

        	if (iterations == 3 ){
        		//printf("time now: %d",timePassed);
        		orderedToFloor(0,waiting);
        	}
*/
        	if (timerCheckPoint == timePassed){
        		tick5();
        	}
        	//printStatus(waiting);
        	//printf("iteration no: %d",iterations);

			assert(currentFloor <= 4 && currentFloor >= 0);




        }

        printStatistics(waiting);
        fclose(l_outFile);
    }

    return 0;
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    exit(-1);
}
/*..........................................................................*/
void BSP_display(char const *msg) {
    fprintf(l_outFile, msg);
}
/*..........................................................................*/
void BSP_exit(void) {
    printf("Bye, Bye!");
    exit(0);
}
/*..........................................................................*/
static void dispatch(QSignal sig) {
  //fprintf(l_outFile, "\n%c:", 'A' + sig - A_SIG);
    Q_SIG((QHsm *)&HSM_QHsmTst) = sig;
    QHsm_dispatch((QHsm *)&HSM_QHsmTst);              /* dispatch the event */
}

void printStatus(void){
	printf("\n");

	printf(" currentfloor: %d",currentFloor);
	printf(" direction: %d",direct);
	printf(" timePased: %d",timePassed);
	printf(" orders: ");
	int k;
	for (k = 0; k<5; k++){
		printf(" %d,",elevOrders[k]);

	}
	printf("\n");


}
