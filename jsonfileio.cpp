#include "jsonfileio.h"
JsonFileIO::JsonFileIO(){}

bool JsonFileIO::SaveJsonDocumentToDisk(QJsonDocument &doc, const QString filename, int &errorCode) {
    errorCode = JsonFileIOError::NoError;
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        errorCode = JsonFileIOError::CouldNotOpenFile;
        return false;
    } else if (saveFile.write(doc.toJson()) != -1) {
        return true;
    } else {
        errorCode = JsonFileIOError::CouldNotSaveFile;
        return false;
    }

    return true;
}

bool JsonFileIO::LoadJsonDocumentFromDisk(QJsonDocument &doc, const QString filename, int &errorCode)
{
    errorCode = JsonFileIOError::NoError;
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray ba = file.readAll();
        QJsonParseError jsonParseError;
        doc = QJsonDocument::fromJson(ba, &jsonParseError);
        if(jsonParseError.error != QJsonParseError::NoError) {
            errorCode = JsonFileIOError::CouldNotParseFile;
            return false;
        } else {
            //rootJsonObject = jsonDoc.object();
            return true; // happy path
        }
    } else {
        errorCode = JsonFileIOError::CouldNotOpenFile;
        return false;
    }
    return true;
}
