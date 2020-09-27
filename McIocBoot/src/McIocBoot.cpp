#include "McBoot/McIocBoot.h"

#include <QQmlContext>
#include <QJSValue>
#include <QGlobalStatic>
#include <QQuickView>
#include <QDebug>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McIoc/BeanDefinition/IMcBeanDefinition.h>

#include "McBoot/Controller/impl/McControllerContainer.h"
#include "McBoot/Model/McModelContainer.h"
#include "McBoot/Socket/impl/McQmlSocketContainer.h"
#include "McBoot/Requestor/McQmlRequestor.h"

MC_DECL_PRIVATE_DATA(McIocBoot)
McAnnotationApplicationContextPtr context;
MC_DECL_PRIVATE_DATA_END

Q_GLOBAL_STATIC_WITH_ARGS(QQmlEngine *, mcEngine, (nullptr))

McIocBoot::McIocBoot(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McIocBoot)
}

McIocBoot::~McIocBoot() 
{
}

void McIocBoot::init(QQmlApplicationEngine *engine) noexcept 
{
    *mcEngine = engine;
    
    McIocBootPtr boot = McIocBootPtr::create();
    boot->initBoot();
    
    McControllerContainerPtr controllerContainer = McControllerContainerPtr::create();
    controllerContainer->init(boot);
    
    McModelContainerPtr modelContainer = McModelContainerPtr::create();
    modelContainer->init(boot);
    
    McQmlSocketContainerPtr socketContainer = McQmlSocketContainerPtr::create();
    socketContainer->init(boot);
    
    McQmlRequestor *requestor = new McQmlRequestor(engine); //!< 不需要设置父对象
    requestor->setControllerContainer(controllerContainer);
    requestor->setSocketContainer(socketContainer);
    
    //! engine的newQObject函数会将其参数所有权转移到其返回的QJSValue中
    QJSValue jsObj = engine->newQObject(requestor);
    engine->globalObject().setProperty("$", jsObj);
    QString data = R"(
       $.__proto__.get = function(uri) {
           return $.invoke(uri);
       }
       
       $.__proto__.post = function(uri, body) {
           return $.invoke(uri, body);
       }
       
       $.__proto__.qs = function(uri, data) {
           if(data === undefined) {
               return $.addConnect(uri);
           }else{
               return $.addConnect(uri, data);
           }
       }
       String.prototype.format = function(args) {
           if(arguments.length <= 0) {
               return this;
           }

           var result = this;
           if(arguments.length == 1 && typeof(args) == 'object') {
               for(var key in args) {
                   if(args[key] !== undefined) {
                       var reg = new RegExp('({' + key + '})', 'g');
                       result = result.replace(reg, args[key]);
                   }
               }
           }else{
               for(var i = 0; i < arguments.length; ++i) {
                   if(arguments[i] !== undefined) {
                       reg = new RegExp('({)' + i + '(})', 'g');
                       result = result.replace(reg, arguments[i]);
                   }
               }
           }
           return result;
       }
    )";
    engine->evaluate(data);
}

QQmlEngine *McIocBoot::engine() noexcept
{
    return *mcEngine;
}

QQuickView *McIocBoot::createQuickView(const QString &source, QWindow *parent) noexcept
{
    if(McIocBoot::engine() == nullptr) {
        qCritical("engine is null. you must be call function init or run before");
        return nullptr;
    }
    QQuickView *view = new QQuickView(engine(), parent);
    view->setSource(QUrl(Mc::toAbsolutePath(source)));
    return  view;
}

void McIocBoot::initBoot() noexcept 
{
    if (d->context) {
		qInfo() << "The container has been initialized";
		return;
	}
    d->context = McAnnotationApplicationContextPtr::create();
    d->context->refresh();  //!< 预加载bean
}

QSharedPointer<IMcApplicationContext> McIocBoot::getApplicationContext() const noexcept 
{
    return d->context;
}

QList<QString> McIocBoot::getAllComponent() noexcept
{
    auto context = getApplicationContext();
	if (!context) {
		qCritical() << "Please call initContainer to initialize container first";
		return QList<QString>();
	}
	QList<QString> components;
    QHash<QString, IMcBeanDefinitionPtr> beanDefinitions = context->getBeanDefinitions();
	for (auto itr = beanDefinitions.cbegin(); itr != beanDefinitions.cend(); ++itr) {
		auto beanDefinition = itr.value();
		if (!isComponent(beanDefinition->getBeanMetaObject()))
			continue;
		components.append(itr.key());
	}
	return components;
}

QList<QString> McIocBoot::getComponents(const QString &componentType) noexcept 
{
    auto context = getApplicationContext();
	if (!context) {
		qCritical() << "Please call initContainer to initialize container first";
		return QList<QString>();
	}
	QList<QString> components;
    QHash<QString, IMcBeanDefinitionPtr> beanDefinitions = context->getBeanDefinitions();
	for (auto itr = beanDefinitions.cbegin(); itr != beanDefinitions.cend(); ++itr) {
		auto beanDefinition = itr.value();
		if (!isComponentType(beanDefinition->getBeanMetaObject(), componentType))
			continue;
		components.append(itr.key());
	}
	return components;
}

bool McIocBoot::isComponent(const QMetaObject *metaObj) noexcept
{
    if(!metaObj) {
        return false;
    }
    int classInfoCount = metaObj->classInfoCount();
	for (int i = 0; i < classInfoCount; ++i) {
		auto classInfo = metaObj->classInfo(i);
		if (qstrcmp(classInfo.name(), MC_COMPONENT) != 0)
			continue;
		return true;
	}
	return false;
}

bool McIocBoot::isComponentType(const QMetaObject *metaObj, const QString &type) noexcept 
{
    if(!metaObj) {
        return false;
    }
	int classInfoCount = metaObj->classInfoCount();
	for (int i = 0; i < classInfoCount; ++i) {
		auto classInfo = metaObj->classInfo(i);
		if (qstrcmp(classInfo.name(), MC_COMPONENT) != 0)
			continue;
		return classInfo.value() == type;
	}
	return false;
}
