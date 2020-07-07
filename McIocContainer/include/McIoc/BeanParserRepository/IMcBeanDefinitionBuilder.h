#pragma once

#include "IMcBeanDefinitionCreator.h"

class IMcBeanDefinitionBuilder : public IMcBeanDefinitionCreator
{
public:
    virtual ~IMcBeanDefinitionBuilder() = default;
};

MC_DECL_POINTER(IMcBeanDefinitionBuilder)
