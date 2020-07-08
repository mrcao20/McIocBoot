#include "McIoc/PrimaryBeanParserRepository/PrimaryBeanParser/impl/McAbstractPrimaryXmlTagParser.h"

#include <QDebug>

MC_DECL_PRIVATE_DATA(McAbstractPrimaryXmlTagParser)
QHash<QString, IMcSecondaryBeanParserPtr> parsers;
IMcBeanDefinitionRegistry *registry{nullptr};
IMcBeanDefinitionPtr beanDefinition;
MC_DECL_PRIVATE_DATA_END

McAbstractPrimaryXmlTagParser::McAbstractPrimaryXmlTagParser(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractPrimaryXmlTagParser)
}

McAbstractPrimaryXmlTagParser::~McAbstractPrimaryXmlTagParser() 
{
}

void McAbstractPrimaryXmlTagParser::addParser(
        const QString &name, IMcSecondaryBeanParserConstPtrRef parser) noexcept
{
    d->parsers.insert(name, parser);
}

void McAbstractPrimaryXmlTagParser::parseBean(
        const QVariant &data
        , IMcBeanDefinitionConstPtrRef beanDefinition
        , IMcBeanDefinitionRegistry *registry) noexcept
{
    d->registry = registry;
    d->beanDefinition = beanDefinition;
    
    
}
