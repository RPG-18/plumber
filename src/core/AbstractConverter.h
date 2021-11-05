#pragma once

#include <QtCore/QByteArray>

namespace core {

/**!
 * AbstractConverter convert binary data to json
 */
class AbstractConverter
{
public:
    virtual ~AbstractConverter() = default;

    /**!
     * to json
     * @param binary data
     * @return json
     */
    virtual QByteArray toJSON(QByteArray &&binary);

    /**!
     * from json
     * @param json message
     * @return binary data
     */
    virtual QByteArray fromJSON(QByteArray &&json);
};
} // namespace core