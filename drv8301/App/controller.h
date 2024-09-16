
/********************************Copyright (c)**********************************\
**                   (c) Copyright 2022, Huazi, China, MY.
**                           All Rights Reserved
**-------------------------------File Information-------------------------------
** \file: controller.h
** \author: Huazi
** \data: 2022-06-04
** \brief: 
**-------------------------------Version Information----------------------------
** \version: V0.1
** \describe: 
**-----------------------------------------------------------------------------*/
#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_

#include "focmath.h"
extern Foc_Controller Controller;
void main_entery(void);
void run_open(Foc_Controller * struct_ptr);
void run_close(Foc_Controller * struct_ptr);

#endif
