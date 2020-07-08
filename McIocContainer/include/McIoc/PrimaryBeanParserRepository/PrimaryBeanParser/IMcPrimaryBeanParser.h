#pragma once

#include "../IMcBeanParser.h"

MC_FORWARD_DECL_CLASS(IMcBeanDefinition);

class IMcBeanDefinitionRegistry;

class IMcPrimaryBeanParser : public IMcBeanParser {
public:
    virtual ~IMcPrimaryBeanParser() = default;
    
    virtual void parseBean(const QVariant &data
                           , IMcBeanDefinitionConstPtrRef beanDefinition
                           , IMcBeanDefinitionRegistry *registry) noexcept = 0;
};

MC_DECL_POINTER(IMcPrimaryBeanParser)
