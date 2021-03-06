#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>
#include <QDebug>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McOrm/DaoLoader/impl/McDaoLoader.h>

#include "TestDao.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(R"(E:\Code\QtCreator\McQuickBoot\Examples\OrmTest\d_orm_test.db)");
    qDebug() << db.open();
    
    McAnnotationApplicationContextPtr appCtx = McAnnotationApplicationContextPtr::create();
    appCtx->getBean<McDaoLoader>("daoLoader")->init(appCtx);
    
    auto testDao = appCtx->getBean<TestDao>("testDao");
    QList<TestVoPtr> lr;
    TestVoPtr r = TestVoPtr::create();
    r->id = 1;
    lr.append(r);
    auto l = testDao->selectById(1);
    qDebug() << l->id << l->userName << l->password << l->article->id;
    
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
    return app.exec();
}
