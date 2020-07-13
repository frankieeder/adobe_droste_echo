/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007-2015 Adobe Systems Incorporated                  */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/**
	Echo.h
**/

#pragma once

#ifndef ECHO_H
#define ECHO_H

#include "AEConfig.h"
#include "entry.h"

#include "AE_Effect.h"
#include "AE_EffectCB.h"		
#include "AE_Macros.h"
#include "AE_ChannelSuites.h"
#include "AE_EffectSuites.h"

#include "Param_Utils.h"
#include "AEFX_SuiteHelper.h"		// PICA Suite Stuff
#include "DuckSuite.h"


#define	MAJOR_VERSION	2
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	0


#define	NAME				"Visual Echo"
#define DESCRIPTION			"Echo Image.\r Copyright 2020\r\Frankie Eder."
#define CHECK_FRAME_NAME	"Frame offset"
#define CHECK_LAYER_NAME	"Layer to checkout"
#define CHECK_DEST_DECAY_NAME    "Destination Decay Rate"
#define CHECK_DEST_X_NAME    "Destination X Offset"
#define CHECK_DEST_Y_NAME    "Destination Y Offset"
#define CHECK_DEST_X_POWER_NAME    "Destination X Offset Power"
#define CHECK_DEST_Y_POWER_NAME    "Destination Y Offset Power"
#define CHECK_SOURCE_DECAY_NAME    "Source Decay Rate"
#define CHECK_SOURCE_X_NAME    "Source X Offset"
#define CHECK_SOURCE_Y_NAME    "Source Y Offset"
#define CHECK_SOURCE_X_POWER_NAME    "Source X Offset Power"
#define CHECK_SOURCE_Y_POWER_NAME    "Source Y Offset Power"
#define CHECK_ITERS_NAME    "Number of Iterations"
#define CHECK_X_NAME        "X Offset"
#define CHECK_Y_NAME        "Y Offset"

enum {
	CHECK_INPUT = 0,
	CHECK_FRAME,
	CHECK_LAYER,
    CHECK_DEST_DECAY,
    CHECK_DEST_X,
    CHECK_DEST_Y,
    CHECK_DEST_X_POWER,
    CHECK_DEST_Y_POWER,
    CHECK_SOURCE_DECAY,
    CHECK_SOURCE_X,
    CHECK_SOURCE_Y,
    CHECK_SOURCE_X_POWER,
    CHECK_SOURCE_Y_POWER,
    CHECK_ITERS,
	CHECK_NUM_PARAMS
};

enum {
	CHECK_FRAME_DISK_ID = 1,
	CHECK_LAYER_DISK_ID,
    CHECK_DEST_DECAY_DISK_ID,
    CHECK_DEST_X_DISK_ID,
    CHECK_DEST_Y_DISK_ID,
    CHECK_DEST_X_POWER_DISK_ID,
    CHECK_DEST_Y_POWER_DISK_ID,
    CHECK_SOURCE_DECAY_DISK_ID,
    CHECK_SOURCE_X_DISK_ID,
    CHECK_SOURCE_Y_DISK_ID,
    CHECK_SOURCE_X_POWER_DISK_ID,
    CHECK_SOURCE_Y_POWER_DISK_ID,
    CHECK_ITERS_DISK_ID
};

#define	CHECK_FRAME_MIN		-100
#define	CHECK_FRAME_MAX		100
#define	CHECK_FRAME_DFLT	0

#define    CHECK_DEST_DECAY_MIN        0
#define    CHECK_DEST_DECAY_MAX        99.99
#define    CHECK_DEST_DECAY_DFLT       90

#define    CHECK_DEST_X_MIN        -1000
#define    CHECK_DEST_X_MAX        1000
#define    CHECK_DEST_X_DFLT       0

#define    CHECK_DEST_Y_MIN        -1000
#define    CHECK_DEST_Y_MAX        1000
#define    CHECK_DEST_Y_DFLT       0

#define    CHECK_DEST_X_POWER_MIN        -100
#define    CHECK_DEST_X_POWER_MAX        100
#define    CHECK_DEST_X_POWER_DFLT       1

#define    CHECK_DEST_Y_POWER_MIN        -100
#define    CHECK_DEST_Y_POWER_MAX        100
#define    CHECK_DEST_Y_POWER_DFLT       1

#define    CHECK_SOURCE_DECAY_MIN        0
#define    CHECK_SOURCE_DECAY_MAX        99.99
#define    CHECK_SOURCE_DECAY_DFLT       90

#define    CHECK_SOURCE_X_MIN        -1000
#define    CHECK_SOURCE_X_MAX        1000
#define    CHECK_SOURCE_X_DFLT       0

#define    CHECK_SOURCE_Y_MIN        -1000
#define    CHECK_SOURCE_Y_MAX        1000
#define    CHECK_SOURCE_Y_DFLT       0

#define    CHECK_SOURCE_X_POWER_MIN        -100
#define    CHECK_SOURCE_X_POWER_MAX        100
#define    CHECK_SOURCE_X_POWER_DFLT       1

#define    CHECK_SOURCE_Y_POWER_MIN        -100
#define    CHECK_SOURCE_Y_POWER_MAX        100
#define    CHECK_SOURCE_Y_POWER_DFLT       1

#define    CHECK_ITERS_MIN        1
#define    CHECK_ITERS_MAX        1000
#define    CHECK_ITERS_DFLT       90

extern "C" {

	DllExport
	PF_Err 
	EffectMain (
		PF_Cmd			cmd,
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output );

}

#endif // ECHO_H
