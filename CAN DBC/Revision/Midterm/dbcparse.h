#ifndef DBCPARSE_H
#define DBCPARSE_H

#include <QString>
#include <QVector>

struct Signal {

    QString name;
    int startBit;
    int bitLength;
    QString endian;
    double scale;
    double offset;
    double min;
    double max;
    QString sgcm;
    QString unit;
};

struct Message {
    QString name;
    int id;
    int length;
    QString cm;
    QVector<Signal> signalList;
};

class DBCParse {
public:
    static QVector<Message> parseDBCToMessages(const QString &filename);
};

#endif // DBCPARSE_H
