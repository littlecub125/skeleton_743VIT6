/*
 * info.h
 *
 *  Created on: Aug 16, 2026
 *      Author: Yubin Kim
 */


//-- Definition
//


//-- Functions
//


//-- Variables 
//

#ifndef INC_AP_THREAD_INFO_H_
#define INC_AP_THREAD_INFO_H_

#include "uart.h"

typedef void (*cli_func_t)(int argc, char *argv[]);

void infoCliInit(void);
void infoCliUpdate(void);
bool cliAdd(const char *name, cli_func_t func);

#endif /* INC_AP_THREAD_INFO_H_ */
