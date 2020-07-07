#pragma once

#include "../McGlobal.h"

class IMcBeanDefinitionRegistry;

class IMcBeanParserRepository
{
public:
    virtual ~IMcBeanParserRepository() = default;
    
    virtual void parseBeanDefinition(const QVariant &var, IMcBeanDefinitionRegistry *registry) noexcept = 0;
};

MC_DECL_POINTER(IMcBeanParserRepository)
