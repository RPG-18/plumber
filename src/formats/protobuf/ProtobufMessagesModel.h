#pragma once
#include <QtCore/QAbstractListModel>
#include <QtCore/QUrl>

namespace formats {
namespace protobuf {

/**!
 * ProtobufMessagesModel show messages in proto file
 */
class ProtobufMessagesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QUrl file READ file WRITE setFile NOTIFY fileChanged)
public:
    enum Roles { Message = Qt::UserRole + 1 };

    explicit ProtobufMessagesModel(QObject *parent = nullptr);

    void setFile(const QUrl &file);
    QUrl file() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:

    void fileChanged();
    void loaded();
    void parseErrors(QString errs);

private:
    void loadMessages();

private:
    QUrl m_file;
    QStringList m_messages;
};

} // namespace protobuf
} // namespace formats