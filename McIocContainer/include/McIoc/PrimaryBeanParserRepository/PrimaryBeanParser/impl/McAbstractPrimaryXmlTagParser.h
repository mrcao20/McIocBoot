#pragma once

#include "../IMcPrimaryBeanParser.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractPrimaryXmlTagParser);

MC_FORWARD_DECL_CLASS(IMcSecondaryBeanParser);

class McAbstractPrimaryXmlTagParser
        : public QObject
        , public IMcPrimaryBeanParser 
{
    Q_OBJECT
public:
    explicit McAbstractPrimaryXmlTagParser(QObject *parent = nullptr);
    ~McAbstractPrimaryXmlTagParser() override;
    
    void addParser(const QString &name, IMcSecondaryBeanParserConstPtrRef parser) noexcept;
    
    void parseBean(const QVariant &data
                   , IMcBeanDefinitionConstPtrRef beanDefinition
                   , IMcBeanDefinitionRegistry *registry) noexcept override final;
    
private:
    MC_DECL_PRIVATE(McAbstractPrimaryXmlTagParser)
};

MC_DECL_POINTER(McAbstractPrimaryXmlTagParser)
