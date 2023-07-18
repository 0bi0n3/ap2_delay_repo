//------------------------------------------------------------------------
// Copyright(c) 2023 Oberon Day-West.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace delay_sfx {
//------------------------------------------------------------------------
static const Steinberg::FUID kdelay_oneProcessorUID (0xE4964DE3, 0x799A5817, 0x8174458A, 0xEEF2A6F9);
static const Steinberg::FUID kdelay_oneControllerUID (0x2A655AAC, 0x767450DB, 0xBCC54D8D, 0x3D4AE478);

#define delay_oneVST3Category "Fx"

//------------------------------------------------------------------------
} // namespace delay_sfx
