//
// Created by 19254 on 2023/10/10.
//

#include <QProcess>
#include <iostream>
#include "process.hpp"

namespace Lib {

size_t ProcessList(std::vector<ProcessInfo> &list) {
    list.clear();
    QProcess process;
    process.start("tasklist", QStringList() << "/FO" << "CSV" << "/NH");
    process.waitForFinished();
    QByteArray result = process.readAllStandardOutput();

    QList<QByteArray> lines = result.split('\n');
    for (const QByteArray &line: lines) {
        QList<QByteArray> fields = line.split(',');
        if (fields.size() >= 2) {
            QString processName = QString::fromUtf8(fields.at(0).trimmed());
            QString processId = QString::fromUtf8(fields.at(1).trimmed());
            auto processNameStr = processName.toStdString();
            auto processIdStr = processId.toStdString();
            if (processNameStr.empty() || processIdStr.empty()) {
                continue;
            }
            if (processNameStr[0] == '\"' && processNameStr[processNameStr.size() - 1] == '\"') {
                processNameStr = processNameStr.substr(1, processNameStr.size() - 2);
            }
            if (processIdStr[0] == '\"' && processIdStr[processIdStr.size() - 1] == '\"') {
                processIdStr = processIdStr.substr(1, processIdStr.size() - 2);
            }
            list.push_back({processNameStr, std::stoull(processIdStr)});
        }
    }
    return list.size();
}

size_t ProcessList(std::map<std::string, uint64_t> &list) {
    list.clear();
    QProcess process;
    process.start("tasklist", QStringList() << "/FO" << "CSV" << "/NH");
    process.waitForFinished();
    QByteArray result = process.readAllStandardOutput();

    QList<QByteArray> lines = result.split('\n');
    for (const QByteArray &line: lines) {
        QList<QByteArray> fields = line.split(',');
        if (fields.size() >= 2) {
            QString processName = QString::fromUtf8(fields.at(0).trimmed());
            QString processId = QString::fromUtf8(fields.at(1).trimmed());
            auto processNameStr = processName.toStdString();
            auto processIdStr = processId.toStdString();
            if (processNameStr.empty() || processIdStr.empty()) {
                continue;
            }
            if (processNameStr[0] == '\"' && processNameStr[processNameStr.size() - 1] == '\"') {
                processNameStr = processNameStr.substr(1, processNameStr.size() - 2);
            }
            if (processIdStr[0] == '\"' && processIdStr[processIdStr.size() - 1] == '\"') {
                processIdStr = processIdStr.substr(1, processIdStr.size() - 2);
            }
            list[processNameStr] = std::stoull(processIdStr);
        }
    }
    return list.size();
}


}
