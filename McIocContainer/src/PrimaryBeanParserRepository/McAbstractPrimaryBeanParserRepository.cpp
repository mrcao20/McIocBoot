#include "McIoc/PrimaryBeanParserRepository/impl/McAbstractPrimaryBeanParserRepository.h"

#include <QDebug>

#include "McIoc/PrimaryBeanParserRepository/PrimaryBeanParser/IMcPrimaryBeanParser.h"

MC_DECL_PRIVATE_DATA(McAbstractPrimaryBeanParserRepository)
QHash<QString, IMcPrimaryBeanParserPtr> parsers;
IMcBeanDefinitionRegistry *registry{nullptr};
IMcBeanDefinitionPtr beanDefinition;
MC_DECL_PRIVATE_DATA_END

McAbstractPrimaryBeanParserRepository::McAbstractPrimaryBeanParserRepository(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractPrimaryBeanParserRepository)
}

McAbstractPrimaryBeanParserRepository::~McAbstractPrimaryBeanParserRepository() 
{
}

void McAbstractPrimaryBeanParserRepository::addParser(
        const QString &name
        , IMcPrimaryBeanParserConstPtrRef parser) noexcept
{
    d->parsers.insert(name, parser);
}

IMcBeanDefinitionRegistry *McAbstractPrimaryBeanParserRepository::registry() const noexcept
{
    return d->registry;
}

void McAbstractPrimaryBeanParserRepository::parseBean(
        const QVariant &data
        , IMcBeanDefinitionConstPtrRef beanDefinition
        , IMcBeanDefinitionRegistry *registry) noexcept
{
    d->registry = registry;
    d->beanDefinition = beanDefinition;
    
    doParseBean(data);
}

void McAbstractPrimaryBeanParserRepository::parsePrimaryBean(
        const QString &parserName, const QVariant &data) noexcept
{
    if(!d->parsers.contains(parserName)) {
        qCritical("not exists parser for named: %s\n", qPrintable(parserName));
        return;
    }
    auto parser = d->parsers.value(parserName);
    parser->parseBean(data, d->beanDefinition, d->registry);
}
