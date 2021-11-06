#pragma once

#include <QtCore/QMetaEnum>

enum Types { JSON, String, Base64, Float, Double, Long, NoneType, Protobuf };
Q_DECLARE_METATYPE(Types)

QString format(Types type, const QByteArray &data);
QByteArray bytes(Types type, const QString &value);