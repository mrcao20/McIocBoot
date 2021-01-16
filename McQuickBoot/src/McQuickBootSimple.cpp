#include "McBoot/McQuickBootSimple.h"

#include <QDebug>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

#include "McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h"
#include "McBoot/Configuration/McConfigurationContainer.h"
#include "McBoot/Controller/impl/McControllerContainer.h"
#include "McBoot/Model/McModelContainer.h"

namespace {

struct McQuickBootSimpleStaticData
{
    McQuickBootSimplePtr boot;
};

}

MC_GLOBAL_STATIC(McQuickBootSimpleStaticData, mcQuickBootSimpleStaticData)

MC_DECL_PRIVATE_DATA(McQuickBootSimple)
McAnnotationApplicationContextPtr context;

MC_DECL_PRIVATE_DATA_END

MC_INIT(McQuickBootSimple)
MC_INIT_END

McQuickBootSimple::McQuickBootSimple(QObject *parent) : McAbstractQuickBoot(parent)
{
    MC_NEW_PRIVATE_DATA(McQuickBootSimple);
}

McQuickBootSimple::~McQuickBootSimple() {}

void McQuickBootSimple::init() noexcept
{
    if(!mcQuickBootSimpleStaticData->boot.isNull()) {
        qWarning("the boot is already init.");
        return;
    }
    mcQuickBootSimpleStaticData->boot = McQuickBootSimplePtr::create();
    McAbstractQuickBoot::setInstance(mcQuickBootSimpleStaticData->boot);
    McQuickBootSimplePtr &boot = mcQuickBootSimpleStaticData->boot;
    if(boot->d->context.isNull()) {
        boot->d->context = McAnnotationApplicationContextPtr::create();
        boot->doRefresh();
    }
    auto appCtx = boot->d->context;

    auto controllerContainer = appCtx->getBean<McControllerContainer>("controllerContainer");
    auto requestor = appCtx->getBean<McCppRequestor>("cppRequestor");
    requestor->setControllerContainer(controllerContainer);
    boot->setRequestor(requestor);
}

QSharedPointer<McQuickBootSimple> McQuickBootSimple::instance() noexcept
{
    McQuickBootSimplePtr &boot = mcQuickBootSimpleStaticData->boot;
    Q_ASSERT_X(!boot.isNull(), "McQuickBootSimplePtr::instance()", "please call init before");
    return boot;
}

void McQuickBootSimple::refresh() noexcept
{
    d->context->generateReader();
    doRefresh();
}

IMcApplicationContextPtr McQuickBootSimple::getApplicationContext() const noexcept 
{
    return d->context;
}

void McQuickBootSimple::doRefresh() noexcept
{
    auto reader = McConfigurationFileBeanDefinitionReaderPtr::create(d->context);
    reader->readBeanDefinition(d->context.data());
    auto configurationContainer = d->context->getBean<McConfigurationContainer>(
        "configurationContainer");
    configurationContainer->init(this);
    d->context->refresh(); //!< 预加载bean
    initContainer();
}

void McQuickBootSimple::initContainer() const noexcept
{
    auto controllerContainer = d->context->getBean<McControllerContainer>("controllerContainer");
    controllerContainer->init(this);

    auto modelContainer = d->context->getBean<McModelContainer>("modelContainer");
    modelContainer->init(this);
}
