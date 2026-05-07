#include "FileLogger.h"
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <windows.h>



static std::string GetExeDirectory() {
    char buffer[MAX_PATH]; // MAX_PATH 是 Windows 编程中一个经典的宏常量，其值为 260（字符数），表示文件系统路径字符串的最大长度
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string exePath(buffer);
    size_t lastSlash = exePath.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        return exePath.substr(0, lastSlash + 1);
    }
    return "./";
}

FileLogger::FileLogger() {
    std::string logPath = GetExeDirectory() + "api_debug_撒尿牛丸.log";
    // 清空旧日志
    std::ofstream file(logPath, std::ios::trunc);
    m_logPath = logPath;
}

FileLogger& FileLogger::Instance() {
    static FileLogger instance;
    return instance;
}

void FileLogger::Log(const std::string& msg) {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::ofstream file(m_logPath, std::ios::app);
    if (file.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        struct tm tm_buf;
        localtime_s(&tm_buf, &time_t);
        file << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S")
            << "." << std::setfill('0') << std::setw(3) << ms.count()
            << " " << msg << std::endl;
    }
}