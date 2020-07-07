#pragma once

#include "../McGlobal.h"

MC_FORWARD_DECL_CLASS(IMcBeanDefinition);

class IMcBeanDefinitionCreator
{
public:
    virtual ~IMcBeanDefinitionCreator() = default;
    
    virtual IMcBeanDefinitionPtr createNewBeanDefinition(const QString &beanName) noexcept = 0;
};

MC_DECL_POINTER(IMcBeanDefinitionCreator)
