#include "dbcparse.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>


QVector<Message> DBCParse::parseDBCToMessages(const QString &filename) {
    QVector<Message> messages;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open DBC file";
        return messages;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.startsWith("BO_")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 3) {
                Message msg;
                msg.id = parts[1].toInt();
                msg.name = parts[2].split(":").first();
                msg.length = parts[3].toInt();
                msg.cm = parts.mid(4).join(' ');
                messages.append(msg);
            }
        } else if (line.startsWith("SG_")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 7) {
                Signal signal;
                signal.name = parts[1];
                signal.startBit = parts[3].split("|").first().toInt();
                signal.bitLength = parts[3].split("@").first().split("|").last().toInt();
                signal.endian = (parts[3].split("@").last().contains("1")?"Little":"Big");
                signal.scale = parts[4].split(",").first().split("(").last().toDouble();
                signal.offset = parts[4].split(",").last().split(")").first().toDouble();
                signal.min = parts[5].split("|").first().split("[").last().toDouble();
                signal.max = parts[5].split("|").last().split("]").first().toDouble();
                signal.unit = parts[6].mid(1,parts[6].size()-2);
                signal.sgcm = parts[7];
                messages.last().signalList.append(signal);
            }
        }
    }

    file.close();
    return messages;
}
