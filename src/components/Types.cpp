#include <QtCore/QDataStream>
#include <QtEndian>

#include "Types.h"

namespace {
const QString NULL_VALUE("(null)");

}

QString format(Types type, const QByteArray &data)
{
    switch (type) {
    case NoneType:
        return NULL_VALUE;

    case JSON:
    case String: {
        if (data.isEmpty()) {
            return NULL_VALUE;
        }
        return QString::fromUtf8(data);
    }

    case Base64:
        return data.toBase64();

    case Long: {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);

        switch (data.size()) {
        case 4: {
            qint32 val;
            stream >> val;
            return QString::number(val, 10);
        }

        case 8: {
            qint64 val;
            stream >> val;
            return QString::number(val, 10);
        }
        default:
            return QString("Invalid");
        }
    }

    case Float: {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);

        switch (data.size()) {
        case 4: {
            float val;
            stream >> val;
            return QString::number(val);
        }
        case 8: {
            double val;
            stream >> val;
            return QString::number(val);
        }
        default:
            return QString("Invalid");
        }
    }

    default:
        return QString("Unknown");
    }
}
