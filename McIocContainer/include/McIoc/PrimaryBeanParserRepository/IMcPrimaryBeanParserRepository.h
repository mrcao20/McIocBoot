#pragma once

#include "../McGlobal.h"

MC_FORWARD_DECL_CLASS(IMcBeanDefinition);

class IMcBeanDefinitionRegistry;

class IMcPrimaryBeanParserRepository {
public:
    virtual ~IMcPrimaryBeanParserRepository() = default;
    
    virtual void parseBean(const QVariant &data
                           , IMcBeanDefinitionConstPtrRef beanDefinition
                           , IMcBeanDefinitionRegistry *registry) noexcept = 0;
};

MC_DECL_POINTER(IMcPrimaryBeanParserRepository)
