#pragma once

#include "McAbstractQuickBoot.h"

#include "McBoot/Controller/impl/McCppResponse.h"
#include "McBoot/Requestor/McCppRequestor.h"

//! 此宏所对应的对象将在Application析构时销毁，所以一旦Application开始析构，就再也不要调用此宏
#define $ (McQuickBootSimple::instance()->requestor())

MC_FORWARD_DECL_CLASS(IMcApplicationContext);

MC_FORWARD_DECL_PRIVATE_DATA(McQuickBootSimple);

class MCQUICKBOOT_EXPORT McQuickBootSimple : public McAbstractQuickBoot
{
    Q_OBJECT
    MC_DECL_INIT(McQuickBootSimple)
public:
    using QObject::connect;

    explicit McQuickBootSimple(QObject *parent = nullptr);
    ~McQuickBootSimple() override;

    static QMetaObject::Connection connect(const QString &sender,
                                           const QString &signal,
                                           const QString &receiver,
                                           const QString &slot,
                                           Qt::ConnectionType type = Qt::AutoConnection) noexcept;
    static QMetaObject::Connection connect(const QString &sender,
                                           const QString &signal,
                                           QObject *receiver,
                                           const QString &slot,
                                           Qt::ConnectionType type = Qt::AutoConnection) noexcept;

    static void init() noexcept;
    static QSharedPointer<McQuickBootSimple> instance() noexcept;

    McCppRequestor &requestor() const noexcept;
    //! 如果重新或新加载了某些组件，则调用此函数
    void refresh() noexcept;
    IMcApplicationContextPtr getApplicationContext() const noexcept override;

private:
    void doRefresh() noexcept;
    void initContainer() const noexcept;

private:
    MC_DECL_PRIVATE(McQuickBootSimple)
};

MC_DECL_POINTER(McQuickBootSimple);
