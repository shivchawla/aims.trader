#include <QtCore/QCoreApplication>
#include <iostream>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlError>
#include "codegenerator.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Total " << argc << " arguments passed to me" << endl;

    if (argc==1) {
        qDebug() << "Usage:" << endl;
        qDebug() << "classgen <tablename>" <<endl;
        qDebug() << "Classify table name with db name!! For eg. Schema.TableName" << endl;
        return 1;
    }

    QString tableName(argv[1]);
    //QString tableName("StratTrader.Order");
    qDebug() << "Tablename: " << tableName << endl;

    CodeGenerator gen(tableName);
    gen.generate();

    a.quit();
    return 0;
}
