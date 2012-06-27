#include "codegenerator.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextStream>
#include <QFile>
#include <QDebug>

CodeGenerator::CodeGenerator(QString tableName)
{
    _tableName = tableName;
    int pos = _tableName.indexOf(".")+1;
    _entityName = _tableName.mid(pos);
    _className = _entityName + "Data";
    _dbClassName = _entityName + "Db";
}

CodeGenerator :: ~CodeGenerator() {

}

void CodeGenerator :: generateDataHeader() {
    int pos = _tableName.indexOf(".")+1;
    _className = _tableName.mid(pos).append("Data");
    _dbClassName = _tableName.mid(pos).append("Db");

    _text.append(QString("#ifndef %1_H\n").arg(_className.toUpper()));
    _text.append(QString("#define %1_H\n").arg(_className.toUpper()));
    _text.append("{includes}\n");
    _text.append(QString("class %1 : public QObject\n").arg(_className));
    _text.append("{\n");
    _text.append("\tQ_OBJECT\n");
    _text.append("public:\n");
    _text.append("\t//member variables\n");

    foreach(Metadata* metadatap, _metadataList) {
        _text.append(QString("\t%1 %2%3;\n").arg(getQtType(metadatap->columnType),
                                                         metadatap->columnName.left(1).toLower(),
                                                         metadatap->columnName.mid(1)));
    }

    _text.append("\n");
    _text.append("\t//ctor\n");
    _text.append(QString("\t%1(void);\n").arg(_className));
    _text.append(QString("\t%1(%1* copy);\n").arg(_className));
    _text.append(QString("\t~%1(void);\n\n").arg(_className));

    _text.append("\t//methods\n\n");
    _text.append("};\n\n");
    _text.append(QString("#endif //%1_H\n").arg(_className.toUpper()));

    QString headerList;
    foreach(QString headerName, _includes) {
        headerList.append(QString("#include<%1>\n").arg(headerName));
    }

    //headerList.append("\n");
    _text.replace("{includes}", headerList);

    //now write this to file. Default action is to overwrite file
    dumpText(_className.toLower().append(".h"), _text);
}

bool CodeGenerator :: fetchMetadata() {
    QSqlDatabase db = QSqlDatabase :: database();
    if (!db.isValid()) {
        //connect to one
        db = QSqlDatabase:: addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setPort(3306);
        db.setDatabaseName("StratTrader");
        db.setUserName("root");
        db.setPassword("proline21");
        qDebug() << "Database initialized" << endl;
    }
    else {
        qDebug() << "Unable to init database" << endl;
        return false;
    }

    if (!db.open()) {
        qDebug() << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText() << endl;
        return false;
    }
    else qDebug() << "Database connected!!" << endl;

    //get list of columns from this table
    QSqlQuery query;
    query.exec("show columns from " + _tableName);
    qDebug() << "Got " << query.size() << " rows" << endl;

    if (query.size() == 0) {
        qDebug() << "No columns found in table " << _tableName << "\n";
        return false;
    }

    qDebug() << "Loading metadata" << endl;
    _primaryKeyMetadata = NULL;
    while (query.next()) {
        Metadata* metadatap = new Metadata;
        metadatap->columnName = query.value(0).toString();
        metadatap->columnType = query.value(1).toString();
        metadatap->isNull = query.value(2).toString() == "YES";
        metadatap->propertyName = metadatap->columnName.left(1).toLower() + metadatap->columnName.mid(1);

        _metadataList.append(metadatap);

        if (query.value(3).toString() == "PRI")
            _primaryKeyMetadata = metadatap;

        qDebug() << metadatap->columnName << " " << metadatap->columnType << " " << metadatap->isNull << endl;
    }
    query.finish();
    db.close();

    if (_primaryKeyMetadata == NULL && _metadataCount > 0)
        _primaryKeyMetadata = _metadataList.at(0);

    //QString dbName = db.connectionName();
    //QSqlDatabase::removeDatabase(dbName);
    _metadataCount = _metadataList.count();
    qDebug() << "Metadata loaded" << endl;

    return true;
}

void CodeGenerator :: generate() {
    if (!fetchMetadata()) return;
    generateDataHeader();
    generateDataClass();
    generateDbHeader();
    generateDbClass();
}

void CodeGenerator :: generateDataClass() {
    _text = "";
    _text.append(QString("#include \"%1.h\"\n\n").arg(_className.toLower()));
    _text.append(QString("%1 :: %1(void)\n").arg(_className));
    _text.append("{\n}\n\n");
    _text.append(QString("%1 :: %1(%1* copy)\n").arg(_className));
    _text.append("{\n");

    foreach(Metadata* metadatap, _metadataList) {
        QString varName = QString("%1").arg(metadatap->propertyName);
        _text.append(QString("\t%1 = copy->%1;\n").arg(varName));
    }

    _text.append("}\n");
    _text.append(QString("%1 :: ~%1(void)\n").arg(_className));
    _text.append("{\n}\n\n");

    dumpText(_className.toLower().append(".cpp"), _text);

}

void CodeGenerator :: generateDbHeader() {
    _text="";
    _text.append("#include <QObject>\n");
    _text.append("#include <QtCore/QDebug>\n");
    _text.append("#include <QtSql/QSqlDatabase>\n");
    _text.append("#include <QtSql/QSqlError>\n");
    _text.append("#include <QtSql/QSqlQuery>\n");
    _text.append("#include \"DbBase.h\"\n");
    _text.append(QString("#include \"%1.h\"\n\n").arg(_className.toLower()));

    _text.append(QString("class %1 : public DbBase\n").arg(_dbClassName));
    _text.append("{\n");
    _text.append("private:\n");
    _text.append(QString("\tenum %1 {\n").arg(_entityName));

    for(int i=0;i<_metadataCount-1;i++) {
        _text.append(QString("\t\t%1, \n").arg(_metadataList.at(i)->columnName));
    }
    _text.append(QString("\t\t%1, \n").arg(_metadataList.at(_metadataCount-1)->columnName));
    _text.append("\t};\n\n");

    _text.append("public:\n");
    _text.append(QString("\t%1(void);\n").arg(_dbClassName));
    _text.append(QString("\t~%1(void);\n").arg(_dbClassName));
    _text.append("\n\t//Methods\n\n");

    _text.append(QString("\t%1* get%2ById(%3 id);\n").arg(_className, _entityName, getQtType(_primaryKeyMetadata->columnType)));
    _text.append(QString("\tQList<%1*> get%2s();\n").arg(_className, _entityName));

    /*void insertInstrument(const InstrumentData* data);
    void updateInstrument(const InstrumentData* data);
    void deleteInstrument(QUuid id);*/

    _text.append(QString("\tunsigned int insert%1(const %2* data);\n").arg(_entityName, _className));
    _text.append(QString("\tunsigned int update%1(const %2* data);\n").arg(_entityName, _className));
    _text.append(QString("\tunsigned int delete%1(%2 id);\n").arg(_entityName, getQtType(_primaryKeyMetadata->columnType)));

    _text.append("\n");
    _text.append("};\n");

    dumpText(_dbClassName.toLower().append(".h"), _text);
}

void CodeGenerator :: generateDbClass() {
    _text = "";
    //header first
    //_text.append("#include <QObject>\n");
    //_text.append("#include <QtCore/QDebug>\n");
    //_text.append("#include <QtSql/QSqlDatabase>\n");
    _text.append("#include <QtSql/QSqlError>\n");
    //_text.append("#include <QtSql/QSqlQuery>\n");
    _text.append("#include \"stdafx.h\"\n");
    _text.append(QString("#include \"%1.h\"\n\n").arg(_dbClassName.toLower()));

    _text.append(QString("%1 :: %1(void)\n").arg(_dbClassName));
    _text.append("{\n}\n");
    _text.append(QString("%1 :: ~%1(void)\n").arg(_dbClassName));
    _text.append("{\n}\n");

    _text.append(QString("%1* %2 :: get%3ById(%4 id) {\n").arg(_className,
                                                               _dbClassName,
                                                               _entityName,
                                                               getQtType(_primaryKeyMetadata->columnType)));

    _text.append("\tqDebug() << \"Received \" << id << endl;\n");
    _text.append("\tif (!db.open()) {\n"
                 "\t\tqDebug() << \"Unable to connect to database!!\" << endl;\n"
                 "\t\tqDebug() << db.lastError().driverText();\n"
                 "\t\treturn NULL;\n"
                 "\t}\n\n");
    _text.append("\tQSqlQuery query;\n");
    _text.append("\tquery.prepare(\"select ");
    for(int i=0; i<_metadataCount-1; i++) {
        _text.append(QString("%1, ").arg(_metadataList.at(i)->columnName));
    }
    _text.append(QString("%1 from %2 where %3 = %4").arg(_metadataList.at(_metadataCount-1)->columnName,
                                                         _entityName,
                                                         _primaryKeyMetadata->columnName,
                                                         getRhsClause(getQtType(_primaryKeyMetadata->columnType), _primaryKeyMetadata->columnName)
                     ));
    _text.append("\");\n");
    _text.append(QString("\tquery.bindValue(\":%1\", QVariant(id));\n").arg(_primaryKeyMetadata->columnName));
    _text.append("\tquery.exec();\n"
                "\tqDebug() << \"Got \" << query.size() << \" rows\" << endl;\n");
    _text.append("\tif (!query.next()) {\n"
                 "\t\tquery.finish();\n"
                 "\t\tdb.close();\n"
                 "\t\treturn NULL;\n"
                 "\t}\n");
    _text.append(QString("\t%1 *item = new %1();\n").arg(_className));

    foreach(Metadata* mp, _metadataList) {
        writeColumnExtraction(mp->columnName, mp->columnType, mp->propertyName);
    }
    _text.append(QString("\tqDebug() << item->%1 << endl;\n").arg(_primaryKeyMetadata->propertyName));
    _text.append("\tquery.finish();\n"
                 "\tdb.close();\n"
                 "\treturn item;\n"
                 "}\n");
    _text.append("\n");

    //generate insert methods here
    _text.append(QString("unsigned int %1 :: insert%2(const %3* data) {\n").arg(_dbClassName, _entityName, _className));
    _text.append(QString("\treturn insert%1(").arg(_entityName));
    for(int i=1; i<_metadataCount-1; i++) {
        _text.append(QString("data.%1, ").arg(_metadataList.at(i)->propertyName));
    }
    _text.append(QString("data.%1);\n").arg(_metadataList.at(_metadataCount-1)->propertyName));
    _text.append(" }\n\n");

    // second overloaded insert method
    _text.append(QString("unsigned int %1 :: insert%2(").arg(_dbClassName, _entityName));
    for(int i=1; i<_metadataCount-1; i++) {
        _text.append(QString("%1 %2, ").arg(getQtType(_metadataList.at(i)->columnType),
                                              _metadataList.at(i)->propertyName));
    }
    _text.append(QString("%1 %2) {\n ").arg(getQtType(_metadataList.at(_metadataCount-1)->columnType),
                                          _metadataList.at(_metadataCount-1)->propertyName));

    _text.append("\tif (!db.open()) {\n"
                 "\t\tqDebug() << \"Unable to connect to database!!\" << endl;\n"
                 "\t\tqDebug() << db.lastError().driverText();\n"
                 "\t\treturn 0;\n"
                 "\t}\n\n");
    _text.append("\t//prepare statement\n");
    _text.append("\tQSqlQuery query;\n");
    _text.append(QString("query.prepare(\"insert into %1(").arg(_entityName));
    QString colList, paramList;
    for(int i=0; i<_metadataCount-1;i++) {
        colList.append(QString("%1, ").arg(_metadataList.at(i)->columnName));
        paramList.append(QString("%1, ").arg(getRhsClause(_metadataList.at(i)->columnType, _metadataList.at(i)->columnName)));
    }
    colList.append(QString("%1) Values(").arg(_primaryKeyMetadata->columnName));
    paramList.append(QString("%1 )\"\n").arg(getRhsClause(_primaryKeyMetadata->columnType, _primaryKeyMetadata->columnName)));

    _text.append(colList);
    _text.append(paramList);
    _text.append("\t);\n\n");

    foreach(Metadata* mp, _metadataList) {
        if (mp == _primaryKeyMetadata)
            _text.append(QString("\tquery.bindValue(\":%1\", QVariant(QUuid :: createUuid()));\n").arg(mp->columnName));
        else if (getQtType(mp->columnType) == "QUuid")
            _text.append(QString("\tquery.bindValue(\":%1\", QVariant(%2));\n").arg(mp->columnName, mp->propertyName));
        else
            _text.append(QString("\tquery.bindValue(\":%1\", %2);\n").arg(mp->columnName, mp->propertyName));
    }

    _text.append("\t//execute\n"
                 "\tbool result = query.exec();\n"
                 "\tif (!result) {\n"
                 "\t\tqDebug() << \"Couldn't insert row. \" << query.lastError().text() << endl;\n"
                 "\treturn 0;\n"
                 "\t}\n"
                 "\tqDebug() << \"Inserted a row\" << endl;\n"

                 "\treturn 1;\n");
    _text.append("}\n\n");

    //generate update methods
    _text.append(QString("unsigned int %1 :: update%2(%3* data, %4 id) {\n")
                    .arg(_dbClassName, _entityName, _className, getQtType(_primaryKeyMetadata->columnType)));

    _text.append("\tqDebug() << \"Received \" << id << endl;\n\n"
                 "\tif (!db.open()) {\n"
                     "\t\tqDebug() << \"Unable to connect to database!!\" << endl;\n"
                     "\t\tqDebug() << db.lastError().driverText();\n"
                     "\t\treturn 0;\n"
                 "\t}\n\n"
                 "\tQSqlQuery query;\n");

    _text.append(QString("\tquery.prepare(\"Update %1 Set ").arg(_entityName));

    QString colBindings;
    for(int i=0; i<_metadataCount-1;i++) {
        if (_metadataList.at(i) != _primaryKeyMetadata) {
            _text.append(QString("%1 = :%1, ").arg(_metadataList.at(i)->columnName));
            colBindings.append(QString("\tquery.bindValue(\":%1\", data->%2);\n").arg(_metadataList.at(i)->columnName,
                                                                                    _metadataList.at(i)->propertyName));
        }
    }
    colBindings.append(QString("\tquery.bindValue(\":%1\", id);\n").arg(_primaryKeyMetadata->columnName));
    _text.append(QString("%1 = :%1 Where %2 = :%2 \");\n").arg(_metadataList.at(_metadataCount-1)->columnName,
                                                               _primaryKeyMetadata->columnName));
    _text.append(colBindings);
    _text.append("\tbool result = query.exec();\n"
                 "\tif (!result)\n"
                     "\t\tqDebug() << \"Could not update table for id \" << id << endl;\n"
                 "\tquery.finish();\n"
                 "\tdb.close();\n"
                 "\treturn query.size();\n");
    _text.append("}\n\n");

    //generate delete methods
    _text.append(QString("unsigned int %1 :: delete%2(%3 id) {\n")
                    .arg(_dbClassName, _entityName, getQtType(_primaryKeyMetadata->columnType)));

    _text.append("\tqDebug() << \"Received \" << id << endl;\n\n"
                 "\tif (!db.open()) {\n"
                     "\t\tqDebug() << \"Unable to connect to database!!\" << endl;\n"
                     "\t\tqDebug() << db.lastError().driverText();\n"
                     "\t\treturn 0;\n"
                 "\t}\n\n"
                 "\tQSqlQuery query;\n");

    _text.append(QString("\tquery.prepare(\"Delete from %1 where %2 = %3\");\n")
                            .arg(_entityName, _primaryKeyMetadata->columnName, getRhsClause(_primaryKeyMetadata->columnType, _primaryKeyMetadata->columnName)));

    _text.append(QString("\tquery.bindValue(\":%1\", id);\n").arg(_primaryKeyMetadata->columnName));
    _text.append("\tbool result = query.exec();\n"
                 "\tif (!result)\n"
                     "\t\tqDebug() << \"Could not delete row with id \" << id << endl;\n"
                 "\tquery.finish();\n"
                 "\tdb.close();\n"
                 "\treturn query.size();\n");
    _text.append("}\n\n");

    dumpText(_dbClassName.toLower().append(".cpp"), _text);
}

void CodeGenerator :: writeColumnExtraction(QString& colName, QString& colType, QString& propertyName) {
    QString qtType = getQtType(colType);
    if (qtType == "QUuid") {
        _text.append(QString("\titem->%1 = QUuid::fromRfc4122(query.value(%2).toByteArray());\n").arg(propertyName, colName));
    }
    else if (qtType == "float") {
        _text.append(QString("\titem->%1 = query.value(%2).toFloat();\n").arg(propertyName, colName));
    }
    else if (qtType == "QDateTime") {
        _text.append(QString("\titem->%1 = query.value(%2).toDateTime();\n").arg(propertyName, colName));
    }
    else if(qtType == "QString") {
        _text.append(QString("\titem->%1 = query.value(%2).toString();\n").arg(propertyName, colName));
    }
    else if(qtType == "QChar") {
        _text.append(QString("\titem->%1 = query.value(%2).toChar();\n").arg(propertyName, colName));
    }
    else if(qtType == "unsigned int") {
        _text.append(QString("\titem->%1 = query.value(%2).toUInt();\n").arg(propertyName, colName));
    }
    else if(qtType == "long") {
        _text.append(QString("\titem->%1 = query.value(%2).toLong();\n").arg(propertyName, colName));
    }
}

void CodeGenerator :: addHeader(QString& headerName) {
    if (!_includes.contains(headerName) && headerName.startsWith('Q'))
        _includes.append(headerName);
}

QString CodeGenerator :: getQtType(QString& columnType) {
    QString qtType;
    if (columnType == "binary(16)") {
        qtType = "QUuid";
    }
    else if (columnType.startsWith("varchar")) {
        if (columnType.contains("(1)"))
            qtType = "QChar";
        else
            qtType = "QString";
    }
    else if (columnType == "datetime") {
        qtType = "QDateTime";
    }
    else if (columnType == "decimal(7,2)") {
        qtType = "float";
    }
    else if (columnType.startsWith("int")) {
        /* if (columnType.contains("unsigned"))
            qtType = "unsigned int";
//        if (columnType.contains("(11)"))
        else
            qtType = "long"; */
        qtType = "unsigned int";
    }

    addHeader(qtType);
    return qtType;
}

void CodeGenerator :: dumpText(QString& filename, QString& text) {
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream dump(&file);
    dump << text;

    file.close();
}

QString CodeGenerator :: getRhsClause(QString qtColType, QString parameterName) {
    if (qtColType == "QUuid") {
        //Unhex(Replace(Replace(Replace(:COLUMN_NAME, '{', ''), '}', ''), '-', ''))
        return QString("Unhex(Replace(Replace(Replace(:%1, '{', ''), '}', ''), '-', ''))").arg(parameterName);
    }
    else {
        return ":" + parameterName;
    }
}
