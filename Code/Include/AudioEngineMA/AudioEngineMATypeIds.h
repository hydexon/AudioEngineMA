
#pragma once

namespace AudioEngineMA
{
    // System Component TypeIds
    inline constexpr const char* AudioEngineMASystemComponentTypeId = "{FE28D075-DB3D-4AC3-8969-A11AD4894E4C}";
    inline constexpr const char* AudioEngineMAEditorSystemComponentTypeId = "{08EE4F95-3237-4026-A5EE-CC6FE7D1DCFC}";

    // Module derived classes TypeIds
    inline constexpr const char* AudioEngineMAModuleInterfaceTypeId = "{81BCC5B2-A303-45A9-BD92-10C382B29C0D}";
    inline constexpr const char* AudioEngineMAModuleTypeId = "{7E7EFC81-304A-453B-947B-84B59B75874D}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* AudioEngineMAEditorModuleTypeId = AudioEngineMAModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* AudioEngineMARequestsTypeId = "{F0BDC15E-49FC-41E5-8B19-354DC7435469}";
} // namespace AudioEngineMA
