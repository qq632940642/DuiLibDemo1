#pragma once

#include <string>
#include <mutex>

class FileLogger {
public:
    static FileLogger& Instance();

    void Log(const std::string& msg);

private:
    FileLogger();
    ~FileLogger() = default;
    FileLogger(const FileLogger&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;

    std::mutex m_mutex;
    std::string m_logPath;
};