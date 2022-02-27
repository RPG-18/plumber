#include <QtCore/QDebug>

#include <QtGui/QFontDatabase>
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>

#include "HexView.h"

///
AbstractDataSource::AbstractDataSource(QObject *parent)
    : QObject(parent)
{}

QIODevice *AbstractDataSource::device()
{
    return nullptr;
}

FileDataSource::FileDataSource(QObject *parent)
    : AbstractDataSource(parent)
{}

QIODevice *FileDataSource::device()
{
    return &m_file;
}

QString FileDataSource::filename()
{
    return m_file.fileName();
}

void FileDataSource::setFilename(const QString &file)
{
    if (m_file.isOpen()) {
        m_file.close();
    }
    m_file.setFileName(file);
    if (m_file.open(QIODeviceBase::ReadOnly)) {
        emit deviceChanged();
    }
}

MessageValueDataSource::MessageValueDataSource(QObject *parent)
    : AbstractDataSource(parent)
    , m_device(new QBuffer(this))
{}
QIODevice *MessageValueDataSource::device()
{
    return m_device;
}

const Message &MessageValueDataSource::message()
{
    return m_message;
}
void MessageValueDataSource::setMessage(const Message &msg)
{
    m_message = msg;
    m_device->setData(msg.value);
    m_device->open(QIODeviceBase::ReadOnly);
    emit deviceChanged();
}

HexView::HexView(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_dataSource(nullptr)
    , m_charWidth(0)
    , m_charHeight(0)
    , m_posAddr(8)
    , m_posAscii(0)
    , m_bytesPerLine(MIN_BYTES_PER_LINE)
{
    QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    if (f.styleHint() != QFont::TypeWriter) {
        f.setFamily("Monospace"); // Force Monospaced font
        f.setStyleHint(QFont::TypeWriter);
    }
    f.setPointSize(12);
    m_font.swap(f);
    QFontMetrics metrics(m_font);

    m_charWidth = metrics.horizontalAdvance(QLatin1Char('9'));
    m_charHeight = metrics.height();

    m_posHex = m_posAddr + ADR_LENGTH * m_charWidth + GAP_ADR_HEX;
    m_posAscii = m_posHex + m_bytesPerLine * m_charWidth * 3 + GAP_HEX_ASCII;
    m_numbers.reserve(256);
    for (int i = 0; i < 256; i++) {
        auto pp = QString::number(i, 16);
        if (pp.size() < 2) {
            pp.push_front('0');
        }
        m_numbers.push_back(pp.toUpper());
    }
    setAntialiasing(true);
    setRenderTarget(QQuickPaintedItem::Image);
    setSmooth(true);
}

void HexView::paint(QPainter *painter)
{
    if (m_dataSource == nullptr) {
        return;
    }

    if (m_dataSource->device() == nullptr) {
        return;
    }

    painter->setFont(m_font);
    auto device = m_dataSource->device();
    device->seek(0);

    int yPos = m_charHeight;
    int line = 0;
    while (!device->atEnd()) {
        auto data = device->read(m_bytesPerLine);
        if (data.isEmpty()) {
            return;
        }

        QString address = QString("%1").arg(line * m_bytesPerLine, 10, 16, QChar('0'));
        painter->drawText(m_posAddr, yPos, address);

        int xPos = m_posHex + m_charWidth;
        for (int i = 0; i < data.size(); i++) {
            painter->drawText(xPos, yPos, m_numbers[uint8_t(data[i])]);
            xPos += m_charWidth * 3;
        }

        for (int i = 0; i < data.size(); i++) {
            char ch = data[i];
            if ((ch < 0x20) || (ch > 0x7e)) {
                ch = '.';
            }
            data[i] = ch;
        }
        painter->drawText(m_posAscii, yPos, QString::fromLatin1(data));

        yPos += m_charHeight;
        line++;
    }
}

AbstractDataSource *HexView::dataSource()
{
    return m_dataSource;
}

void HexView::setDataSource(AbstractDataSource *source)
{
    m_dataSource = source;
    QObject::connect(m_dataSource, &AbstractDataSource::deviceChanged, this, &HexView::calcSize);

    if (m_dataSource->device() != nullptr) {
        calcSize();
    }
}

int HexView::bytesPerLine() const
{
    return m_bytesPerLine;
}

void HexView::setBytesPerLine(int line)
{
    m_bytesPerLine = line;
    calcSize();
}

void HexView::calcSize()
{
    if (m_dataSource->device() == nullptr) {
        return;
    }

    m_posAscii = m_posHex + m_bytesPerLine * m_charWidth * 3 + GAP_HEX_ASCII;

    setImplicitWidth(m_posAscii + (m_bytesPerLine * m_charWidth));
    const int lines = m_dataSource->device()->size() / m_bytesPerLine;
    setImplicitHeight(lines * m_charHeight);
    setWidth(m_posAscii + (m_bytesPerLine * m_charWidth));
    setHeight(lines * m_charHeight);
}
