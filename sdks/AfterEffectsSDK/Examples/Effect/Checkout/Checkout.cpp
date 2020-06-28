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
    Checkout.cpp
    
    Revision History
        
    Version        Change                                        Engineer    Date
    =======        ======                                        ========    ======
    1.0         created                                        bbb
    1.1            folded ADM sample code into project            bbb
    1.2            removed ADM sample code                        bbb            12/22/2005
    2.0            Premiere Pro compatibility                    zal            12/28/2006
    2.1            -> .cpp                                        zal            11/15/2011
    2.2            Allow negative frame offsets                zal            8/12/2013
    2.3            AEFX_AcquireSuite -> AEFX_SuiteScoper        zal            1/5/2015
                 and some exception handling
    2.5            Added new entry point                        zal            9/15/2017
*/

#include <cmath>
#include "Checkout.h"


static PF_Err
GlobalSetup (
    PF_InData        *in_data,
    PF_OutData        *out_data,
    PF_ParamDef        *params[],
    PF_LayerDef        *output )
{
    PF_Err     err = PF_Err_NONE;

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

    out_data->my_version = PF_VERSION(    MAJOR_VERSION,
                                        MINOR_VERSION,
                                        BUG_VERSION,
                                        STAGE_VERSION,
                                        BUILD_VERSION);

    out_data->out_flags =     PF_OutFlag_WIDE_TIME_INPUT |
                            PF_OutFlag_I_DO_DIALOG;

    out_data->out_flags2 =    PF_OutFlag2_SUPPORTS_THREADED_RENDERING;

    return err;
}


static PF_Err
ParamsSetup (
    PF_InData        *in_data,
    PF_OutData        *out_data,
    PF_ParamDef        *params[],
    PF_LayerDef        *output )
{
    PF_Err            err = PF_Err_NONE;
    PF_ParamDef        def;

    AEFX_CLR_STRUCT(def);
    
    PF_ADD_SLIDER(    CHECK_FRAME_NAME,
                    CHECK_FRAME_MIN,
                    CHECK_FRAME_MAX,
                    CHECK_FRAME_MIN,
                    CHECK_FRAME_MAX,
                    CHECK_FRAME_DFLT,
                    CHECK_FRAME_DISK_ID);

    AEFX_CLR_STRUCT(def);

    PF_ADD_LAYER(    CHECK_LAYER_NAME,
                    PF_LayerDefault_MYSELF,
                    CHECK_LAYER_DISK_ID);
    
    // Decay
    /*PF_ADD_FLOAT_SLIDERX(    CHECK_DECAY_NAME,
                    0,
                    1,
                    0,
                    1,
                    CHECK_DECAY_DFLT,
                    PF_Precision_HUNDREDTHS,
                    0,
                    0,
                    CHECK_DECAY_DISK_ID);*/
    PF_ADD_SLIDER(  CHECK_DECAY_NAME,
                    0,
                    100,
                    0,
                    100,
                    75,
                    CHECK_DECAY_DISK_ID);

    AEFX_CLR_STRUCT(def);
    
    // Iters
    PF_ADD_SLIDER(    CHECK_ITERS_NAME,
                    0,
                    100,
                    0,
                    100,
                    10,
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

 
static PF_Err
Render(
    PF_InData        *in_data,
    PF_OutData        *out_data,
    PF_ParamDef        *params[],
    PF_LayerDef        *output )
{
    PF_Err                err             = PF_Err_NONE,
                        err2             = PF_Err_NONE;
    int32_t                num_channelsL     = 0;
    PF_Rect                halfsies        = {0,0,0,0};
    PF_ParamDef            checkout;
    PF_ChannelDesc        desc;
    PF_ChannelRef        ref;
    PF_ChannelChunk        chunk;
    PF_Boolean            found_depthPB;
    
    AEFX_CLR_STRUCT(checkout);
    
    // Premiere Pro/Elements does not support this suite
    if (in_data->appl_id != 'PrMr')
    {
        AEFX_SuiteScoper<PF_ChannelSuite1> csP = AEFX_SuiteScoper<PF_ChannelSuite1>(    in_data,
                                                                                        kPFChannelSuite1,
                                                                                        kPFChannelSuiteVersion1,
                                                                                        out_data);
                                
        ERR(csP->PF_GetLayerChannelCount(    in_data->effect_ref,
                                            0,
                                            &num_channelsL));
                                                 

        if(num_channelsL) {
            ERR(csP->PF_GetLayerChannelTypedRefAndDesc(    in_data->effect_ref,
                                                        0,
                                                        PF_ChannelType_DEPTH,
                                                        &found_depthPB,
                                                        &ref,
                                                        &desc));

            ERR(csP->PF_CheckoutLayerChannel(    in_data->effect_ref,
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

    // set the checked-out rect to be the top half of the layer
    //halfsies.top    = halfsies.left    = 0;
    //halfsies.right    = (short)output->width;
    //halfsies.bottom    = (short)(output->height / 2);
    ERR(PF_CHECKOUT_PARAM(    in_data,
        CHECK_LAYER,
        (in_data->current_time + params[CHECK_FRAME]->u.sd.value * in_data->time_step),
        in_data->time_step,
        in_data->time_scale,
        &checkout));
    float r = .9;//params[CHECK_DECAY]->u.sd.value / params[CHECK_DECAY]->u.sd.valid_max;
    int num_iters = params[CHECK_ITERS]->u.sd.value;
    float s;
    int w_diff, h_diff;
    for (int i = 0; i < num_iters; i++) {
        s = pow(r, i);
        w_diff = (output->width * (1-s))/2;
        h_diff = (output->height * (1-s))/2;
        if (w_diff <= 1 || h_diff <= 1) {
            break;
        }
        halfsies.top = h_diff;
        halfsies.left = w_diff;
        halfsies.right = (output->width - w_diff);
        halfsies.bottom = (output->height - h_diff);
        
        if (!err) {
            if (checkout.u.ld.data)  {
                ERR(PF_COPY(&checkout.u.ld,
                            output,
                            NULL,
                            &halfsies));
            }  else  {
                // no layer? Zero-alpha black.
                ERR(PF_FILL(NULL, &halfsies, output));
            }
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
    PF_InData        *in_data,
    PF_OutData        *out_data,
    PF_ParamDef        *params[],
    PF_LayerDef        *output )
{
    PF_SPRINTF(    out_data->return_msg,
                "%s, v%d.%d\r%s",
                NAME,
                MAJOR_VERSION,
                MINOR_VERSION,
                DESCRIPTION);

    return PF_Err_NONE;
}

static PF_Err
PopDialog (
    PF_InData        *in_data,
    PF_OutData        *out_data,
    PF_ParamDef        *params[],
    PF_LayerDef        *output )
{
    PF_Err err = PF_Err_NONE;

    PF_SPRINTF(    out_data->return_msg,
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
    PF_Cmd            cmd,
    PF_InData        *in_data,
    PF_OutData        *out_data,
    PF_ParamDef        *params[],
    PF_LayerDef        *output )
{
    PF_Err        err = PF_Err_NONE;

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

