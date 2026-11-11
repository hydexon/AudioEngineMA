#include "Utils.h"

namespace AudioEngineMA
{
    ma_attenuation_model AttenuationMode::ToMAModel(Type type)
    {
        switch(type) {
        case AttenuationMode::None: return ma_attenuation_model_none;
        case AttenuationMode::Linear: return ma_attenuation_model_linear;
        case AttenuationMode::Inverse: return ma_attenuation_model_inverse;
        case AttenuationMode::Exponential: return ma_attenuation_model_exponential;
        default: return ma_attenuation_model_none;
        }
    }
    AttenuationMode::Type AttenuationMode::FromMA(ma_attenuation_model matype)
    {
        switch(matype)
        {
        case ma_attenuation_model_none:
            return AttenuationMode::None;
        case ma_attenuation_model_linear:
            return AttenuationMode::Linear;
        case ma_attenuation_model_inverse:
            return AttenuationMode::Inverse;
        case ma_attenuation_model_exponential:
            return AttenuationMode::Exponential;
        default:
            return AttenuationMode::Count;
        }
    }

    ma_pan_mode PanMode::ToMA(Type type)
    {
        if(type == PanMode::Pan)
            return ma_pan_mode_pan;

        return ma_pan_mode_balance;
    }

    PanMode::Type PanMode::FromMA(ma_pan_mode matype)
    {
        switch(matype)
        {
        case ma_pan_mode_balance:
            return PanMode::Balance;
        case ma_pan_mode_pan:
            return PanMode::Pan;
        default:
            return PanMode::Count;
        }
    }

    ma_positioning PositioningMode::ToMA(Type type)
    {
        if(type == PositioningMode::Relative)
            return ma_positioning_relative;

        return ma_positioning_absolute;
    }

    PositioningMode::Type PositioningMode::FromMA(ma_positioning matype)
    {
        switch(matype)
        {
        case ma_positioning_absolute:
            return PositioningMode::Absolute;
        case ma_positioning_relative:
            return PositioningMode::Relative;
        default:
            return PositioningMode::Count;
        }
    }


}