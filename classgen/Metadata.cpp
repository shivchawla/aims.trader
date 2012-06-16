#include <QString>

typedef struct _Metadata
{
    QString columnName;
    QString columnType;
    bool isNull;
    QString propertyName; //helper property. For eg. Column = InstrumentId => PropertyName = instrumentId
} Metadata, *LPMetadata;
