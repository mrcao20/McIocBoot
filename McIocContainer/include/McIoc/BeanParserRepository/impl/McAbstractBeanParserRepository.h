#pragma once

#include "../IMcBeanParserRepository.h"
#include "../IMcBeanDefinitionBuilder.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractBeanParserRepository);

class McAbstractBeanParserRepository : public QObject
        , public IMcBeanParserRepository
        , public IMcBeanDefinitionBuilder
{
    Q_OBJECT
public:
    McAbstractBeanParserRepository() noexcept;
    ~McAbstractBeanParserRepository() override;
    
    void parseBeanDefinition(const QVariant &var, IMcBeanDefinitionRegistry *registry) noexcept override;
    
    IMcBeanDefinitionPtr createNewBeanDefinition(const QString &beanName) noexcept override;
    
protected:
    virtual QString resolveParserName(const QVariant &var) const noexcept = 0;
    
private:
    MC_DECL_PRIVATE(McAbstractBeanParserRepository)
};

MC_DECL_POINTER(McAbstractBeanParserRepository)
