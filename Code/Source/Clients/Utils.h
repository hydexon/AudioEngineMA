#pragma once

#include "ATLData.h"
#include <miniaudio.h>

namespace AudioEngineMA
{
    namespace AttenuationMode
    {
        ma_attenuation_model ToMAModel(Type type);
        Type FromMA(ma_attenuation_model matype);
    }

    namespace PanMode
    {
        ma_pan_mode ToMA(Type type);
        Type FromMA(ma_pan_mode matype);
    }

    namespace PositioningMode
    {
        ma_positioning ToMA(Type type);
        Type FromMA(ma_positioning matype);
    }
}
