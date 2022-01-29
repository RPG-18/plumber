#include <QtCore/QDataStream>
#include <QtEndian>

#include "Types.h"
#include "spdlog/spdlog.h"

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
            qint32 val = 0;
            stream >> val;
            return QString::number(val, 10);
        }

        case 8: {
            qint64 val = 0;
            stream >> val;
            return QString::number(val, 10);
        }
        default:
            return {"Invalid"};
        }
    }

    case Float: {
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::BigEndian);

        switch (data.size()) {
        case 4: {
            float val = 0;
            stream >> val;
            return QString::number(val);
        }
        case 8: {
            double val = 0;
            stream >> val;
            return QString::number(val);
        }
        default:
            return {"Invalid"};
        }
    };

    case Protobuf:
    case Avro: {
        return data;
    };

    default:
        return {"Unknown"};
    }
}

QByteArray bytes(Types type, const QString &value)
{
    switch (type) {
    case Types::JSON:
    case Types::String:
        return value.toUtf8();
    case Types::Base64:
        return QByteArray::fromBase64(value.toLatin1());
    case Types::Float: {
        bool ok = false;
        auto val = value.toFloat(&ok);
        if (ok) {
            QByteArray data;
            QDataStream stream(&data, QIODeviceBase::WriteOnly);
            stream.setByteOrder(QDataStream::BigEndian);
            stream << val;
            return data;
        }
        spdlog::error("failed convert string {} to float", value.toStdString());
    } break;
    case Types::Double: {
        bool ok = false;
        auto val = value.toDouble(&ok);
        if (ok) {
            QByteArray data;
            QDataStream stream(&data, QIODeviceBase::WriteOnly);
            stream.setByteOrder(QDataStream::BigEndian);
            stream << val;
            return data;
        }
        spdlog::error("failed convert string {} to float", value.toStdString());
    } break;
    case Types::Long: {
        bool ok = false;
        auto val = value.toLongLong(&ok);
        if (ok) {
            QByteArray data;
            QDataStream stream(&data, QIODeviceBase::WriteOnly);
            stream.setByteOrder(QDataStream::BigEndian);
            stream << val;
            return data;
        }
        spdlog::error("failed convert string {} to long", value.toStdString());
    } break;
    case Types::NoneType:
        return {};

    case Types::Protobuf:
    case Types::Avro: {
        // we convert protobuf to json before apply filters
        return value.toUtf8();
    }
    }
    return {};
}