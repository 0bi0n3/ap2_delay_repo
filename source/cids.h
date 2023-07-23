//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

enum AudioParams : Steinberg::Vst::ParamID
{
    kParamGainId = 102,
    kParamDelayTime1Id = 103,
    kParamDelayTime2Id = 104,
    kParamDelayTime3Id = 105,
    kParamDelayTime4Id = 106,
    kParamTapGain1Id = 107,
    kParamTapGain2Id = 108,
    kParamTapGain3Id = 109,
    kParamTapGain4Id = 110,
    kParamFeedbackGain1Id = 111,
    kParamFeedbackGain2Id = 112,
    kParamFeedbackGain3Id = 113,
    kParamFeedbackGain4Id = 114,
};


namespace delay_sfx {
//------------------------------------------------------------------------
static const Steinberg::FUID kdelay_oneProcessorUID (0xE4964DE3, 0x799A5817, 0x8174458A, 0xEEF2A6F9);
static const Steinberg::FUID kdelay_oneControllerUID (0x2A655AAC, 0x767450DB, 0xBCC54D8D, 0x3D4AE478);

#define delay_oneVST3Category "Fx"

//------------------------------------------------------------------------
} // namespace delay_sfx
