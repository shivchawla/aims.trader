#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include<QString>
#include<QStringList>
#include<QList>
#include "Metadata.cpp"

class CodeGenerator
{
    QString _tableName;
    QString _className;
    QString _dbClassName;
    QString _entityName;
    QString _text;
    QList<QString> _includes;
    QList<LPMetadata> _metadataList;
    LPMetadata _primaryKeyMetadata;
    int _metadataCount;

    bool fetchMetadata();
    QString getQtType(QString& columnType);
    void addHeader(QString& headerName);
    void dumpText(QString& filename, QString& text);
    QString getRhsClause(QString colType, QString parameterName);
    void writeColumnExtraction(QString& colName, QString& colType, QString& propertyName);

public:
    CodeGenerator(QString tableName);
    ~CodeGenerator();
    void generateDataHeader();
    void generateDataClass();
    void generateDbHeader();
    void generateDbClass();
    void generate();
};

#endif // CODEGENERATOR_H
