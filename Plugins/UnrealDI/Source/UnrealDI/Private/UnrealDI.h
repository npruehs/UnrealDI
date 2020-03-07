#pragma once

#include "IUnrealDI.h"

class FUnrealDI : public IUnrealDI
{
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
