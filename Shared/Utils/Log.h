#ifndef LOG_H
#define LOG_H

#include <QDebug>
#include <QFile>

enum MessageType {INFO, DEBUG, CRITICAL, WARNING};

class Log
{
    QFile* _file;
    QDebug* _ptr;
    MessageType _messageType;

    public:
        inline Log(const MessageType messageType):_file(new QFile("log.txt")), _messageType(messageType)
        {
            _file->open(QIODevice::Append | QIODevice::Text);
            _ptr = new QDebug(_file);
            switch(messageType)
            {
                case INFO: (*_ptr) << "INFO"; break;
                case DEBUG: (*_ptr) <<  "DEBUG"; break;
                case CRITICAL: (*_ptr) <<  "CRITICAL"; break;
                case WARNING: (*_ptr) <<  "WARNING"; break;
            }
        }

        inline ~Log()
        {
           delete _ptr;
           delete _file;
        }

    public:
        /*inline void log(QChar t) { (*ptr) << t << endl; }
        inline void log(QBool t) { (*ptr) << t << endl; }
        inline void log(bool t) { (*ptr) << t << endl; }
        inline void log(char t) { (*ptr) << t << endl; }
        inline void log(signed short t) { (*ptr) << t << endl; }
        inline void log(unsigned short t) { (*ptr) << t << endl; }
        inline void log(signed int t) { (*ptr) << t << endl; }
        inline void log(unsigned int t) { (*ptr) << t << endl; }
        inline void log(signed long t) { (*ptr) << t << endl; }
        inline void log(unsigned long t) { (*ptr) << t << endl; }
        inline void log(qint64 t)
            { (*ptr) << t << endl; }
        inline void log(quint64 t)
            { (*ptr) << t << endl; }
        inline void log(float t) { (*ptr) << t << endl; }
        inline void log(double t) { (*ptr) << t << endl; }
        inline void log(const char* t) { (*ptr) << t << endl; }
        inline void log(const QString & t) { (*ptr) << t << endl; }
        inline void log(const QStringRef & t) { (*ptr) << t << endl; }
        inline void log(const QLatin1String &t) { (*ptr) << t << endl; }
        inline void log(const QByteArray & t) { (*ptr) << t << endl; }
        */


        inline Log &operator<<(QChar t) { (*_ptr) << t; return *this;  }
        inline Log &operator<<(QBool t) { (*_ptr) << t; return *this; }
        inline Log &operator<<(bool t) { (*_ptr) << t; return *this; }
        inline Log &operator<<(char t) { (*_ptr) << t; return *this; }
        inline Log &operator<<(signed short t) { (*_ptr) << t; return *this; }
        inline Log &operator<<(unsigned short t) { (*_ptr) << t; return *this;; }
        inline Log &operator<<(signed int t) { (*_ptr) << t; return *this; }
        inline Log &operator<<(unsigned int t) { (*_ptr) << t; return *this;}
        inline Log &operator<<(signed long t) { (*_ptr) << t; return *this; }
        inline Log &operator<<(unsigned long t) { (*_ptr) << t; return *this;}
        inline Log &operator<<(qint64 t)
            { (*_ptr) << t; return *this; }
        inline Log &operator<<(quint64 t)
            { (*_ptr) << t; return *this; }
        inline Log &operator<<(float t) { (*_ptr) << t; return *this;}
        inline Log &operator<<(double t) { (*_ptr) << t; return *this; }
        inline Log &operator<<(const char* t) { (*_ptr) << t; return *this; }
        inline Log &operator<<(const QString & t) { (*_ptr) << t; return *this;}
        inline Log &operator<<(const QStringRef & t) { (*_ptr) << t; return *this;}
        inline Log &operator<<(const QLatin1String &t) { (*_ptr) << t; return *this; }
        inline Log &operator<<(const QByteArray & t) { (*_ptr) << t; return *this; }
        inline Log &operator<<(const QDateTime& t) { (*_ptr) << t.toString(Qt::ISODate); return *this;}
        inline Log &operator<<(QTextStreamFunction f) {
            (*_ptr) << f;
            return *this;
        }

        inline Log &operator<<(QTextStreamManipulator m)
        { (*_ptr) << m; return *this; }

        inline const char* messageType(const MessageType type)
        {
            switch(type)
            {
                case INFO: return "INFO"; break;
                case DEBUG: return "DEBUG"; break;
                case CRITICAL: return "CRITICAL"; break;
                case WARNING: return "WARNING"; break;
            }
        }


};

inline Log log(const MessageType type = INFO)
{
    return Log(type);
}


#endif // LOG_H
