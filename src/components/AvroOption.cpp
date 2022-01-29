#include "formats/avro/AvroConverter.h"
#include "spdlog/spdlog.h"

#include "AbstractConverter.h"

#include "AvroOption.h"

AvroOption::AvroOption(QObject *parent)
    : QObject(parent)
{}

void AvroOption::setFile(const QUrl &file)
{
    m_file = file;
    emit changed();
}

const QUrl &AvroOption::file() const
{
    return m_file;
}

std::tuple<std::unique_ptr<core::AbstractConverter>, ErrorWrap> AvroOption::converter()
{
    if (!m_file.isValid()) {
        return std::make_tuple(nullptr, ErrorWrap("converter", "path to schema not set"));
    }
    QStringList errors;
    auto converter = formats::avro::AvroConverter::fabric(m_file, errors);
    if (converter == nullptr) {
        spdlog::error("failed create protobuf converter");
        return std::make_tuple(nullptr, ErrorWrap("consumer", errors.join('\n')));
    }

    return std::make_tuple(std::move(converter), ErrorWrap{});
}
