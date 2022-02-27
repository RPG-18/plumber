#pragma once

#include <QtCore/QBuffer>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QObject>
#include <QtCore/QVector>

#include <QtGui/QFont>
#include <QtQuick/QQuickPaintedItem>

#include "Message.h"

class AbstractDataSource : public QObject
{
    Q_OBJECT
public:
    AbstractDataSource(QObject *parent = nullptr);
    virtual QIODevice *device();

signals:

    void deviceChanged();
};

class FileDataSource : public AbstractDataSource
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename)
public:
    FileDataSource(QObject *parent = nullptr);
    QIODevice *device() override;

    QString filename();
    void setFilename(const QString &file);

private:
    QFile m_file;
};

class MessageValueDataSource : public AbstractDataSource
{
    Q_OBJECT
    Q_PROPERTY(Message message READ message WRITE setMessage)
public:
    MessageValueDataSource(QObject *parent = nullptr);
    QIODevice *device() override;

    const Message &message();
    void setMessage(const Message &msg);

private:
    Message m_message;
    QBuffer *m_device;
};

class HexView : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(AbstractDataSource *dataSource READ dataSource WRITE setDataSource)
    Q_PROPERTY(int bytesPerLine READ bytesPerLine WRITE setBytesPerLine)

public:
    static constexpr int ADR_LENGTH = 10;
    static constexpr int MIN_BYTES_PER_LINE = 16;
    static constexpr int GAP_ADR_HEX = 10;
    static constexpr int GAP_HEX_ASCII = 16;
    static constexpr int MIN_HEXCHARS_IN_LINE = 47;

    explicit HexView(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;

    AbstractDataSource *dataSource();
    void setDataSource(AbstractDataSource *source);

    int bytesPerLine() const;
    void setBytesPerLine(int line);

private slots:

    void calcSize();

private:
    QFont m_font;
    AbstractDataSource *m_dataSource;
    int m_charWidth;
    int m_charHeight;
    int m_posAddr;
    int m_posHex;
    int m_posAscii;
    int m_bytesPerLine;
    QVector<QString> m_numbers;
};