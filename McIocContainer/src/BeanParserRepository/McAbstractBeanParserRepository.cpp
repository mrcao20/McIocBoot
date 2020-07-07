#include "McIoc/BeanParserRepository/impl/McAbstractBeanParserRepository.h"

#include <QDebug>

#include "McIoc/BeanFactory/IMcBeanDefinitionRegistry.h"
#include "McIoc/BeanParserRepository/BeanParser/IMcBeanParser.h"
#include "McIoc/BeanDefinition/impl/McRootBeanDefinition.h"

MC_DECL_PRIVATE_DATA(McAbstractBeanParserRepository)
QHash<QString, IMcBeanParserPtr> beanParsers;
IMcBeanDefinitionRegistry *registry{nullptr};
MC_DECL_PRIVATE_DATA_END

McAbstractBeanParserRepository::McAbstractBeanParserRepository() noexcept
{
    MC_NEW_PRIVATE_DATA(McAbstractBeanParserRepository);
}

McAbstractBeanParserRepository::~McAbstractBeanParserRepository()
{
}

void McAbstractBeanParserRepository::parseBeanDefinition(const QVariant &var, IMcBeanDefinitionRegistry *registry) noexcept
{
    d->registry = registry;
}

IMcBeanDefinitionPtr McAbstractBeanParserRepository::createNewBeanDefinition(const QString &beanName) noexcept
{
    McRootBeanDefinitionPtr beanDefinition = McRootBeanDefinitionPtr::create();
    d->registry->registerBeanDefinition(beanName, beanDefinition);
    return beanDefinition;
}
