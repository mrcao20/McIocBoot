#include "McBoot/Requestor/McCppRequestor.h"

#include <QVariant>

#include "McBoot/Controller/IMcControllerContainer.h"
#include "McBoot/Controller/impl/McCppResponse.h"

MC_AUTO_INIT(McCppRequestor)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McCppRequestor)
MC_DECL_PRIVATE_DATA_END

McCppRequestor::McCppRequestor(QObject *parent) : McAbstractRequestor(parent)
{
    MC_NEW_PRIVATE_DATA(McCppRequestor);
}

McCppRequestor::~McCppRequestor()
{
}

QMetaObject::Connection McCppRequestor::connect(const QString &sender,
                                                const QString &signal,
                                                const QString &receiver,
                                                const QString &slot,
                                                Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(appCtx(), sender, signal, receiver, slot, type);
}

QMetaObject::Connection McCppRequestor::connect(const QString &sender,
                                                const QString &signal,
                                                QObject *receiver,
                                                const QString &slot,
                                                Qt::ConnectionType type) noexcept
{
    return Mc::Ioc::connect(appCtx(), sender, signal, receiver, slot, type);
}

bool McCppRequestor::disconnect(const QString &sender,
                                const QString &signal,
                                const QString &receiver,
                                const QString &slot) noexcept
{
    return Mc::Ioc::disconnect(appCtx(), sender, signal, receiver, slot);
}

bool McCppRequestor::disconnect(const QString &sender,
                                const QString &signal,
                                QObject *receiver,
                                const QString &slot) noexcept
{
    return Mc::Ioc::disconnect(appCtx(), sender, signal, receiver, slot);
}

McCppResponse &McCppRequestor::invoke(const QString &uri) noexcept
{
    auto response = new McCppResponse();
    run(response, uri, QVariant());
    return *response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

McCppResponse &McCppRequestor::invoke(const QString &uri, const QJsonObject &data) noexcept
{
    auto response = new McCppResponse();
    run(response, uri, data);
    return *response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

McCppResponse &McCppRequestor::invoke(const QString &uri, const QVariant &data) noexcept
{
    return invoke(uri, QVariantList() << data);
}

McCppResponse &McCppRequestor::invoke(const QString &uri, const QVariantList &data) noexcept
{
    auto response = new McCppResponse();
    run(response, uri, data);
    return *response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

QVariant McCppRequestor::syncInvoke(const QString &uri) noexcept
{
    return controllerContainer()->invoke(uri, QVariant());
}

QVariant McCppRequestor::syncInvoke(const QString &uri, const QJsonObject &data) noexcept
{
    return controllerContainer()->invoke(uri, data);
}

QVariant McCppRequestor::syncInvoke(const QString &uri, const QVariant &data) noexcept
{
    return syncInvoke(uri, QVariantList() << data);
}

QVariant McCppRequestor::syncInvoke(const QString &uri, const QVariantList &data) noexcept
{
    return controllerContainer()->invoke(uri, data);
}