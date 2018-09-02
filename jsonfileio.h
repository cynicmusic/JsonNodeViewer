#ifndef JSONFILEIO_H
#define JSONFILEIO_H
#include <QFile>
#include <QJsonDocument>

namespace JsonFileIOError {
    enum IOError {
        NoError = 0,
        CouldNotOpenFile = 1,
        CouldNotSaveFile = 2,
        CouldNotParseFile = 3,
    };
}

class JsonFileIO
{
public:
    JsonFileIO();

    static bool SaveJsonDocumentToDisk(QJsonDocument &doc, const QString filename, int &errorCode);
    static bool LoadJsonDocumentFromDisk(QJsonDocument &doc, const QString filename, int &errorCode);
};

#endif // JSONFILEIO_H
