#include "McBoot/McAbstractQuickBoot.h"

#include <QThread>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h"
#include "McBoot/Configuration/McConfigurationContainer.h"
#include "McBoot/Controller/impl/McControllerContainer.h"

namespace {

struct McAbstractQuickBootStaticData
{
    McAbstractQuickBootPtr boot;
};

} // namespace

MC_GLOBAL_STATIC(McAbstractQuickBootStaticData, mcAbstractQuickBootStaticData)

MC_DECL_PRIVATE_DATA(McAbstractQuickBoot)
McCppRequestorPtr requestor;
QThread *workThread{nullptr};
MC_DECL_PRIVATE_DATA_END

MC_INIT(McAbstractQuickBoot)
qRegisterMetaType<QThread *>();
MC_DESTROY()
if (!mcAbstractQuickBootStaticData.exists()) {
    return;
}
if (mcAbstractQuickBootStaticData->boot->d->workThread != nullptr) {
    mcAbstractQuickBootStaticData->boot->d->workThread->quit();
    mcAbstractQuickBootStaticData->boot->d->workThread->wait();
    delete mcAbstractQuickBootStaticData->boot->d->workThread;
    mcAbstractQuickBootStaticData->boot->d->workThread = nullptr;
}
mcAbstractQuickBootStaticData->boot.reset();
MC_INIT_END

McAbstractQuickBoot::McAbstractQuickBoot(QObject *parent) : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractQuickBoot);
}

McAbstractQuickBoot::~McAbstractQuickBoot() {}

QMetaObject::Connection McAbstractQuickBoot::connect(const QString &sender,
                                                     const QString &signal,
                                                     const QString &receiver,
                                                     const QString &slot,
                                                     Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                            sender,
                            signal,
                            receiver,
                            slot,
                            type);
}

QMetaObject::Connection McAbstractQuickBoot::connect(const QString &sender,
                                                     const QString &signal,
                                                     QObject *receiver,
                                                     const QString &slot,
                                                     Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                            sender,
                            signal,
                            receiver,
                            slot,
                            type);
}

bool McAbstractQuickBoot::disconnect(const QString &sender,
                                     const QString &signal,
                                     const QString &receiver,
                                     const QString &slot) noexcept
{
    return Mc::Ioc::disconnect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                               sender,
                               signal,
                               receiver,
                               slot);
}

bool McAbstractQuickBoot::disconnect(const QString &sender,
                                     const QString &signal,
                                     QObject *receiver,
                                     const QString &slot) noexcept
{
    return Mc::Ioc::disconnect(McAbstractQuickBoot::instance()->getApplicationContext().data(),
                               sender,
                               signal,
                               receiver,
                               slot);
}

QSharedPointer<McAbstractQuickBoot> McAbstractQuickBoot::instance() noexcept
{
    McAbstractQuickBootPtr &boot = mcAbstractQuickBootStaticData->boot;
    Q_ASSERT_X(!boot.isNull(), "McAbstractQuickBoot::instance()", "please call init before");
    return boot;
}

McCppRequestor &McAbstractQuickBoot::requestor() const noexcept
{
    return *d->requestor.data();
}

void McAbstractQuickBoot::setInstance(const QSharedPointer<McAbstractQuickBoot> &ins) noexcept
{
    mcAbstractQuickBootStaticData->boot = ins;
}

void McAbstractQuickBoot::doRefresh() noexcept
{
    auto context = getApplicationContext();
    auto reader = McConfigurationFileBeanDefinitionReaderPtr::create(context);
    reader->readBeanDefinition(context.data());
    auto configurationContainer = context->getBean<McConfigurationContainer>(
        "configurationContainer");
    configurationContainer->init(this);
    auto workThreadName = QT_STRINGIFY(MC_QUICKBOOT_WORK_THREAD_NAME);
    if (context->containsBean(workThreadName)) {
        d->workThread = context->getBeanPointer<QThread *>(workThreadName);
    }
    if (d->workThread != nullptr && !d->workThread->isRunning()) {
        d->workThread->start();
    }
    context->refresh(d->workThread); //!< 预加载bean
    initContainer();

    auto controllerContainer = context->getBean<McControllerContainer>("controllerContainer");
    auto cppRequestor = context->getBean<McCppRequestor>("cppRequestor");
    cppRequestor->setControllerContainer(controllerContainer);
    cppRequestor->setAppCtx(context.data());
    d->requestor = cppRequestor;
}
