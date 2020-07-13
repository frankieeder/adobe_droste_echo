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

/*
	Echo.cpp
*/

#include <cmath>
#include <algorithm>
#include "Echo.h"


static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output ) 
{
	PF_Err 	err = PF_Err_NONE;

	try {
		AEFX_SuiteScoper<DuckSuite1> dsP = AEFX_SuiteScoper<DuckSuite1>(in_data,
																		kDuckSuite1,
																		kDuckSuiteVersion1,
																		out_data,
																		"No Duck Suite! That's OK, it just means the Sweetie SDK sample wasn't loaded. \
                                                                        Build the Sweetie SDK sample in the AEGP folder and put it in the plug-in folder.");

		dsP->Quack(2);

	} catch (A_Err &thrown_err) {
		// No need to return the thrown error in this case, since this isn't a critical error.
		// It just means the Sweetie SDK sample wasn't loaded.
		thrown_err = 0;
	}

	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	out_data->out_flags = 	PF_OutFlag_WIDE_TIME_INPUT |
							PF_OutFlag_I_DO_DIALOG;

	out_data->out_flags2 =	PF_OutFlag2_SUPPORTS_THREADED_RENDERING;

	return err;
}


static PF_Err 
ParamsSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;	

	AEFX_CLR_STRUCT(def);
	
	PF_ADD_SLIDER(	CHECK_FRAME_NAME, 
					CHECK_FRAME_MIN, 
					CHECK_FRAME_MAX, 
					CHECK_FRAME_MIN, 
					CHECK_FRAME_MAX, 
					CHECK_FRAME_DFLT,
					CHECK_FRAME_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_LAYER(	CHECK_LAYER_NAME,
					PF_LayerDefault_MYSELF,
					CHECK_LAYER_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDER(
                        CHECK_DEST_DECAY_NAME,
                        CHECK_DEST_DECAY_MIN,
                        CHECK_DEST_DECAY_MAX,
                        CHECK_DEST_DECAY_MIN,
                        CHECK_DEST_DECAY_MAX,
                        0,
                        CHECK_DEST_DECAY_DFLT,
                        PF_Precision_TENTHS,
                        PF_ValueDisplayFlag_PERCENT,
                        0,
                        CHECK_DEST_X_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDER(
                        CHECK_DEST_X_NAME,
                        CHECK_DEST_X_MIN,
                        CHECK_DEST_X_MAX,
                        CHECK_DEST_X_MIN,
                        CHECK_DEST_X_MAX,
                        0,
                        CHECK_DEST_X_DFLT,
                        PF_Precision_TENTHS,
                        PF_ValueDisplayFlag_PERCENT,
                        0,
                        CHECK_DEST_X_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDER(
                        CHECK_DEST_Y_NAME,
                        CHECK_DEST_Y_MIN,
                        CHECK_DEST_Y_MAX,
                        CHECK_DEST_Y_MIN,
                        CHECK_DEST_Y_MAX,
                        0,
                        CHECK_DEST_Y_DFLT,
                        PF_Precision_TENTHS,
                        PF_ValueDisplayFlag_PERCENT,
                        0,
                        CHECK_DEST_Y_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDER(
                        CHECK_DEST_X_POWER_NAME,
                        CHECK_DEST_X_POWER_MIN,
                        CHECK_DEST_X_POWER_MAX,
                        CHECK_DEST_X_POWER_MIN,
                        CHECK_DEST_X_POWER_MAX,
                        0,
                        CHECK_DEST_X_POWER_DFLT,
                        PF_Precision_HUNDREDTHS,
                        PF_ValueDisplayFlag_NONE,
                        0,
                        CHECK_DEST_X_POWER_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDER(
                        CHECK_DEST_Y_POWER_NAME,
                        CHECK_DEST_Y_POWER_MIN,
                        CHECK_DEST_Y_POWER_MAX,
                        CHECK_DEST_Y_POWER_MIN,
                        CHECK_DEST_Y_POWER_MAX,
                        0,
                        CHECK_DEST_Y_POWER_DFLT,
                        PF_Precision_HUNDREDTHS,
                        PF_ValueDisplayFlag_NONE,
                        0,
                        CHECK_DEST_Y_POWER_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDER(
                        CHECK_SOURCE_DECAY_NAME,
                        CHECK_SOURCE_DECAY_MIN,
                        CHECK_SOURCE_DECAY_MAX,
                        CHECK_SOURCE_DECAY_MIN,
                        CHECK_SOURCE_DECAY_MAX,
                        0,
                        CHECK_SOURCE_DECAY_DFLT,
                        PF_Precision_TENTHS,
                        PF_ValueDisplayFlag_PERCENT,
                        0,
                        CHECK_SOURCE_X_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDER(
                        CHECK_SOURCE_X_NAME,
                        CHECK_SOURCE_X_MIN,
                        CHECK_SOURCE_X_MAX,
                        CHECK_SOURCE_X_MIN,
                        CHECK_SOURCE_X_MAX,
                        0,
                        CHECK_SOURCE_X_DFLT,
                        PF_Precision_TENTHS,
                        PF_ValueDisplayFlag_PERCENT,
                        0,
                        CHECK_SOURCE_X_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDER(
                        CHECK_SOURCE_Y_NAME,
                        CHECK_SOURCE_Y_MIN,
                        CHECK_SOURCE_Y_MAX,
                        CHECK_SOURCE_Y_MIN,
                        CHECK_SOURCE_Y_MAX,
                        0,
                        CHECK_SOURCE_Y_DFLT,
                        PF_Precision_TENTHS,
                        PF_ValueDisplayFlag_PERCENT,
                        0,
                        CHECK_SOURCE_Y_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDER(
                        CHECK_SOURCE_X_POWER_NAME,
                        CHECK_SOURCE_X_POWER_MIN,
                        CHECK_SOURCE_X_POWER_MAX,
                        CHECK_SOURCE_X_POWER_MIN,
                        CHECK_SOURCE_X_POWER_MAX,
                        0,
                        CHECK_SOURCE_X_POWER_DFLT,
                        PF_Precision_HUNDREDTHS,
                        PF_ValueDisplayFlag_NONE,
                        0,
                        CHECK_SOURCE_X_POWER_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDER(
                        CHECK_SOURCE_Y_POWER_NAME,
                        CHECK_SOURCE_Y_POWER_MIN,
                        CHECK_SOURCE_Y_POWER_MAX,
                        CHECK_SOURCE_Y_POWER_MIN,
                        CHECK_SOURCE_Y_POWER_MAX,
                        0,
                        CHECK_SOURCE_Y_POWER_DFLT,
                        PF_Precision_HUNDREDTHS,
                        PF_ValueDisplayFlag_NONE,
                        0,
                        CHECK_SOURCE_Y_POWER_DISK_ID);
    
    PF_ADD_SLIDER(    CHECK_ITERS_NAME,
                    CHECK_ITERS_MIN,
                    CHECK_ITERS_MAX,
                    CHECK_ITERS_MIN,
                    CHECK_ITERS_MAX,
                    CHECK_ITERS_DFLT,
                    CHECK_ITERS_DISK_ID);


	out_data->num_params = CHECK_NUM_PARAMS;
	
	// Premiere Pro/Elements does not support this suite
	if (in_data->appl_id != 'PrMr')
	{
		AEFX_SuiteScoper<PF_EffectUISuite1> effect_ui_suiteP = AEFX_SuiteScoper<PF_EffectUISuite1>(
																	in_data,
																	kPFEffectUISuite,
																	kPFEffectUISuiteVersion1,
																	out_data);

		ERR(effect_ui_suiteP->PF_SetOptionsButtonName(in_data->effect_ref, "Whatever I want!"));
	}
	return err;
}


static PF_Rect ScaledRect(float s, int w, int h, float x, float y, float xp, float yp) {
    int w_diff = (w * (1-s))/2;
    int h_diff = (h * (1-s))/2;
    int w_offset = w * pow(1-s, xp) * x;//, xp);// + x;
    int h_offset = h * pow(1-s, yp) * y;//, yp);// + y;
    
    PF_Rect crop_rect = {
        w_diff + w_offset,
        h_diff + h_offset,
        w - w_diff + w_offset,
        h - h_diff + h_offset
    };
    return crop_rect;
}

 
static PF_Err 
Render(	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err 			= PF_Err_NONE,
						err2 			= PF_Err_NONE;
	int32_t				num_channelsL 	= 0;
	PF_Rect             bounds          = {0,0,output->width,output->height};
	PF_ParamDef			checkout; 
	PF_ChannelDesc		desc;
	PF_ChannelRef		ref;
	PF_ChannelChunk		chunk;
	PF_Boolean			found_depthPB;
	
	AEFX_CLR_STRUCT(checkout);
	
	// Premiere Pro/Elements does not support this suite
	if (in_data->appl_id != 'PrMr')
	{
		AEFX_SuiteScoper<PF_ChannelSuite1> csP = AEFX_SuiteScoper<PF_ChannelSuite1>(	in_data,
																						kPFChannelSuite1,
																						kPFChannelSuiteVersion1,
																						out_data);
								
		ERR(csP->PF_GetLayerChannelCount(	in_data->effect_ref, 
											0, 
											&num_channelsL));
												 

		if(num_channelsL) {
			ERR(csP->PF_GetLayerChannelTypedRefAndDesc(	in_data->effect_ref,
														0,	
														PF_ChannelType_DEPTH,		
														&found_depthPB,		
														&ref,	
														&desc)); 

			ERR(csP->PF_CheckoutLayerChannel(	in_data->effect_ref,
												&ref,
												in_data->current_time,
												in_data->time_step,			
												in_data->time_scale,			
												desc.data_type,			
												&chunk));
												
			// do interesting 3d stuff here;
			
			ERR(csP->PF_CheckinLayerChannel(in_data->effect_ref,
											&ref,
											&chunk));
		}
	}
    
    ERR(PF_CHECKOUT_PARAM(    in_data,
        CHECK_LAYER,
        (in_data->current_time + params[CHECK_FRAME]->u.sd.value * in_data->time_step),
        in_data->time_step,
        in_data->time_scale,
        &checkout));
    
    float dd = params[CHECK_DEST_DECAY]->u.fs_d.value / 100.0;
    //float(params[CHECK_DEST_DECAY]->u.fd.value) / float(params[CHECK_DEST_DECAY]->u.fd.valid_max);
    float dx = params[CHECK_DEST_X]->u.fs_d.value / 200.0;
    float dy = params[CHECK_DEST_Y]->u.fs_d.value / 200.0;
    float dxp = params[CHECK_DEST_X_POWER]->u.fs_d.value;
    float dyp = params[CHECK_DEST_Y_POWER]->u.fs_d.value;
    
    float sd = params[CHECK_SOURCE_DECAY]->u.fs_d.value / 100.0;
    float sx = params[CHECK_SOURCE_X]->u.fs_d.value / 200.0;
    float sy = params[CHECK_SOURCE_Y]->u.fs_d.value / 200.0;
    float sxp = params[CHECK_SOURCE_X_POWER]->u.fs_d.value;
    float syp = params[CHECK_SOURCE_Y_POWER]->u.fs_d.value;
    
    int num_iters = params[CHECK_ITERS]->u.sd.value;
    float dd_c; // crop scale
    float sd_c; // source scale
    int w_diff, h_diff;
    // Fill Background First
    PF_FILL(NULL, &bounds, output);
    for (int i = 0; i < num_iters; i++) {
        dd_c = pow(dd, i);
        sd_c = pow(sd, i);
        if (dd_c < 0.001) {
            break;
        }
        PF_Rect cr = ScaledRect(dd_c, output->width, output->height, dx, dy, dxp, dyp);
        PF_Rect sr = ScaledRect(sd_c*dd_c, output->width, output->height, sx, sy, sxp, syp);
        
        if (!err) {
            //PF_COPY(SRC, DST, SRC_RECT, DST_RECT)
            if (checkout.u.ld.data)  {
                ERR(PF_COPY(&checkout.u.ld,
                            output,
                            &sr,
                            &cr));
            }  else  {
                // no layer? Zero-alpha black.
                ERR(PF_FILL(NULL, &bounds, output));
            }
        
            /*if (!err)  {
                halfsies.top        = halfsies.bottom = 0;                //reset rect, copy.
                halfsies.bottom     = (short)output->height;

                ERR(PF_COPY(&params[CHECK_INPUT]->u.ld,
                            output,
                            NULL,
                            &halfsies));
            }*/
        } else {
            break;
        }
    }
    
    ERR2(PF_CHECKIN_PARAM(in_data, &checkout));        // ALWAYS check in,
    // even if invalid param.
	return err;
}


static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_SPRINTF(	out_data->return_msg, 
				"%s, v%d.%d\r%s",
				NAME, 
				MAJOR_VERSION, 
				MINOR_VERSION, 
				DESCRIPTION);

	return PF_Err_NONE;
}

static PF_Err 
PopDialog (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err err = PF_Err_NONE;

	PF_SPRINTF(	out_data->return_msg, 
				"This would be a fine place for\ra platform-specific options dialog.");
	out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;
	
 	return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"Checkout", // Name
		"ADBE Checkout", // Match Name
		"Sample Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}


PF_Err
EffectMain (	
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err = PF_Err_NONE;

	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:
				err = About(in_data,out_data,params,output);
				break;		
			case PF_Cmd_GLOBAL_SETUP:
				err = GlobalSetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ParamsSetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_RENDER:
				err = Render(in_data,out_data,params,output);
				break;
			case PF_Cmd_DO_DIALOG:
				err = PopDialog(in_data,out_data,params,output);
				break;		
			default:
				break;
		}
	} catch(PF_Err &thrown_err) {
		// Never EVER throw exceptions into AE.
		err = thrown_err;
	}
	return err;
}

