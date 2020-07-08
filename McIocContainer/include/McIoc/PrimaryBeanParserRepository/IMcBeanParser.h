#pragma once

#include "../McGlobal.h"

class IMcBeanParser {
public:
    virtual ~IMcBeanParser() = default;
    
    virtual QString parserName() const noexcept = 0;
};

MC_DECL_POINTER(IMcBeanParser)
