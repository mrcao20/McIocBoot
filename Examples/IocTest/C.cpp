#include "C.h"

#include <QThread>
#include <QDebug>

MC_INIT(R)
MC_REGISTER_BEAN_FACTORY(R);    //!< 由于类R没有使用MC_DEFINE_TYPELIST宏，则这里只需要传入R即可
MC_INIT_END

QString R::text() const noexcept
{
    return m_text;
}

void R::setText(const QString &val) noexcept
{
    m_text = val;
}

void R::slot_recv() noexcept
{
    qDebug() << "r slot recv";
}

#include <McIoc/ApplicationContext/McContainerGlobal.h>

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(C));   //!< 由于类C使用了MC_DEFINE_TYPELIST宏，则需要使用MC_TYPELIST宏
MC_REGISTER_CONTAINER_CONVERTER(QList<QString>);    //!< 容器需要额外注册，只需注册一次即可到处使用，此宏多次调用只生效一次
MC_REGISTER_LIST_CONVERTER(QVector<RPtr>);  //!< 和MC_REGISTER_CONTAINER_CONVERTER效果一样
MC_REGISTER_MAP_CONVERTER(StringMap);       //!< 重定义之后需要使用重定义之后的类型
MC_REGISTER_CONTAINER_CONVERTER(RHash);     //!< 和MC_REGISTER_MAP_CONVERTER效果一样
mcConnect("c", "this", "signal_send()", "r", "slot_recv()");
//!< 可以做更多事情，此代码块中的功能将在main函数之前被调用，以后可能会在QCoreApplication构造时调用
//!< 所以建议其他正常操作都放在QCoreApplication构造后
MC_DESTROY()
qDebug() << "destroy.............";
MC_STATIC_END

void C::a() noexcept
{
    qDebug() << "m_text:" << m_text
             << "m_r:" << m_r << m_r->text()
             << "m_texts:" << m_texts
             << "m_rs:" << m_rs
             << "m_mtexts:" << m_mtexts
             << "m_hrs:" << m_hrs
             << "obj thread:" << thread()
             << "cur thread:" << QThread::currentThread()
             << "align" << m_align;
    emit signal_send();
}

QString C::text() const noexcept
{
    return m_text;
}

void C::setText(const QString &val) noexcept
{
    m_text = val;
}

void C::start() noexcept
{
    qDebug() << "start";
}

void C::finished() noexcept
{
    qDebug() << "finished";
}

void C::threadFinished() noexcept
{
    qDebug() << "thread finished";
}
