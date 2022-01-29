#include "spdlog/spdlog.h"

#include "AbstractConverter.h"
#include "Consumer.h"
#include "ProtoOption.h"

#include "formats/protobuf/ProtobufConverter.h"

ProtoOption::ProtoOption(QObject *parent)
    : QObject(parent)
{}

void ProtoOption::setFile(const QUrl &file)
{
    m_file = file;
    emit changed();
}

const QUrl &ProtoOption::file() const
{
    return m_file;
}

void ProtoOption::setMessage(const QString &message)
{
    m_message = message;
    emit changed();
}

const QString &ProtoOption::message() const
{
    return m_message;
}

std::tuple<std::unique_ptr<core::AbstractConverter>, ErrorWrap> ProtoOption::converter()
{
    if (!m_file.isValid()) {
        return std::make_tuple(nullptr, ErrorWrap("converter", "path to proto not set"));
    }

    if (m_message.isEmpty()) {
        return std::make_tuple(nullptr, ErrorWrap("converter", "proto message not select"));
    }

    QStringList errors;
    auto converter = formats::protobuf::ProtobufConverter::fabric(m_file, m_message, errors);
    if (converter == nullptr) {
        spdlog::error("failed create protobuf converter");
        return std::make_tuple(nullptr, ErrorWrap("consumer", errors.join('\n')));
    }

    return std::make_tuple(std::move(converter), ErrorWrap{});
}
