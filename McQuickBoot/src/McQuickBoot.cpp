#include "McBoot/McQuickBoot.h"

#include <QQmlContext>
#include <QJSValue>
#include <QGlobalStatic>
#include <QQuickView>
#include <QDebug>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

#include "McBoot/BeanDefinitionReader/impl/McConfigurationFileBeanDefinitionReader.h"
#include "McBoot/Configuration/McConfigurationContainer.h"
#include "McBoot/Controller/impl/McControllerContainer.h"
#include "McBoot/Model/McModelContainer.h"
#include "McBoot/Requestor/McQmlRequestor.h"
#include "McBoot/Socket/impl/McQmlSocketContainer.h"

namespace {

struct McQuickBootStaticData
{
    McQuickBootPtr boot;
    function<void(QCoreApplication *)> preInitFunc;
    function<void(QCoreApplication *, QQmlApplicationEngine *)> afterInitFunc;
};

}

MC_GLOBAL_STATIC(McQuickBootStaticData, mcQuickBootStaticData)

MC_DECL_PRIVATE_DATA(McQuickBoot)
McAnnotationApplicationContextPtr context;
QQmlEngine *engine{nullptr};
McCppRequestorPtr requestor;
MC_DECL_PRIVATE_DATA_END

MC_INIT(McQuickBoot)
MC_DESTROY()
if(!mcQuickBootStaticData.exists()) {
    return;
}
auto requestor = mcQuickBootStaticData->boot->d->requestor.data();
mcQuickBootStaticData->boot->d->requestor.clear();
delete requestor;
mcQuickBootStaticData->boot.reset();
MC_INIT_END

McQuickBoot::McQuickBoot(QObject *parent) : McAbstractQuickBoot(parent)
{
    MC_NEW_PRIVATE_DATA(McQuickBoot);
}

McQuickBoot::~McQuickBoot()
{
}

void McQuickBoot::init(QCoreApplication *app, QQmlApplicationEngine *engine) noexcept
{
    if(mcQuickBootStaticData->preInitFunc) {
        mcQuickBootStaticData->preInitFunc(app);
    }
    
    if(!mcQuickBootStaticData->boot.isNull()) {
        qWarning("the boot is already init.");
        return;
    }
    mcQuickBootStaticData->boot = McQuickBootPtr::create();
    McQuickBootPtr &boot = mcQuickBootStaticData->boot;
    boot->initBoot(engine);
    auto appCtx = boot->d->context;

    boot->initContainer();

    auto controllerContainer = appCtx->getBean<McControllerContainer>("controllerContainer");
    auto modelContainer = appCtx->getBean<McModelContainer>("modelContainer");
    auto socketContainer = appCtx->getBean<McQmlSocketContainer>("socketContainer");

    boot->d->requestor = appCtx->getBean<McCppRequestor>("cppRequestor");
    boot->d->requestor->setControllerContainer(controllerContainer);

    auto requestor = appCtx->getBean<McQmlRequestor>("qmlRequestor");
    requestor->setControllerContainer(controllerContainer);
    requestor->setSocketContainer(socketContainer);

    //! engine的newQObject函数会将其参数所有权转移到其返回的QJSValue中
    QJSValue jsObj = engine->newQObject(requestor.data());
    engine->globalObject().setProperty("$", jsObj);
    engine->importModule(":/requestor.js.inl");
    
    if(mcQuickBootStaticData->afterInitFunc) {
        mcQuickBootStaticData->afterInitFunc(app, engine);
    }
}

QQmlEngine *McQuickBoot::engine() noexcept
{
    return mcQuickBootStaticData->boot->d->engine;
}

QQuickView *McQuickBoot::createQuickView(const QString &source, QWindow *parent) noexcept
{
    if(McQuickBoot::engine() == nullptr) {
        qCritical("engine is null. you must be call function init or run before\n");
        return nullptr;
    }
    QQuickView *view = new QQuickView(engine(), parent);
    view->setSource(QUrl(Mc::toAbsolutePath(source)));
    return  view;
}

QSharedPointer<McQuickBoot> McQuickBoot::instance() noexcept
{
    McQuickBootPtr &boot = mcQuickBootStaticData->boot;
    Q_ASSERT_X(!boot.isNull(), "McQuickBoot::instance()", "please call init before");
    return boot;
}

void McQuickBoot::setPreInitFunc(const function<void(QCoreApplication *)> &func) noexcept
{
    mcQuickBootStaticData->preInitFunc = func;
}

void McQuickBoot::setAfterInitFunc(const function<void(QCoreApplication *, QQmlApplicationEngine *)> &func) noexcept
{
    mcQuickBootStaticData->afterInitFunc = func;
}

void McQuickBoot::initBoot(QQmlEngine *engine) noexcept
{
    if (d->context) {
        qInfo() << "The container has been initialized";
        return;
    }
    d->engine = engine;
    d->context = McAnnotationApplicationContextPtr::create();
    auto reader = McConfigurationFileBeanDefinitionReaderPtr::create(d->context);
    reader->readBeanDefinition(d->context.data());
    d->context->refresh();  //!< 预加载bean
}

McCppRequestor &McQuickBoot::requestor() const noexcept
{
    return *d->requestor.data();
}

void McQuickBoot::refresh() const noexcept
{
    d->context->generateReader();
    auto reader = McConfigurationFileBeanDefinitionReaderPtr::create(d->context);
    reader->readBeanDefinition(d->context.data());
    d->context->refresh();
    initContainer();
}

IMcApplicationContextPtr McQuickBoot::getApplicationContext() const noexcept 
{
    return d->context;
}

void McQuickBoot::initContainer() const noexcept
{
    auto controllerContainer = d->context->getBean<McControllerContainer>("controllerContainer");
    controllerContainer->init(this);

    auto modelContainer = d->context->getBean<McModelContainer>("modelContainer");
    modelContainer->init(this);

    auto socketContainer = d->context->getBean<McQmlSocketContainer>("socketContainer");
    socketContainer->init(this);

    auto configurationContainer = d->context->getBean<McConfigurationContainer>(
        "configurationContainer");
    configurationContainer->init(this);
}
