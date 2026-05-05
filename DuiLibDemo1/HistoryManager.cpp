#include "HistoryManager.h"
#include <fstream>
#include <windows.h>      // 用于获取 exe 路径
#include <shlobj.h>       // 可选，为了获取AppData目录，这里用exe同目录简单处理

HistoryManager::HistoryManager() {
    // 获取 exe 所在目录，将 history.json 放在 exe 同目录下（方便调试）
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string exePath(buffer);
    size_t lastSlash = exePath.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        m_filePath = exePath.substr(0, lastSlash + 1) + "history.json";
    }
    else {
        m_filePath = "history.json";
    }
    LoadFromFile();
}

HistoryManager::~HistoryManager() {
    SaveToFile();
}

void HistoryManager::Add(const std::string& method, const std::string& url,
    const std::string& headers, const std::string& body) {
    if (method.empty() || url.empty()) return;
    HistoryItem item{ method, url, headers, body };
    m_items.insert(m_items.begin(), item);   // 最新放在最前面
    if (m_items.size() > MAX_HISTORY) {
        m_items.pop_back();
    }
    SaveToFile();   // 立即持久化
}

void HistoryManager::Clear() {
    m_items.clear();
    SaveToFile();
}

void HistoryManager::LoadFromFile() {
    std::ifstream file(m_filePath);
    if (!file.is_open()) return;
    try {
        json j;
        file >> j;
        if (!j.is_array()) return;
        m_items.clear();
        for (const auto& obj : j) {
            HistoryItem item;
            item.method = obj.value("method", "GET");
            item.url = obj.value("url", "");
            item.headers = obj.value("headers", "");
            item.body = obj.value("body", "");
            m_items.push_back(item);
        }
    }
    catch (...) {
        // 解析失败就忽略
    }
}

void HistoryManager::SaveToFile() const {
    json j = json::array();
    for (const auto& item : m_items) {
        json obj;
        obj["method"] = item.method;
        obj["url"] = item.url;
        obj["headers"] = item.headers;
        obj["body"] = item.body;
        j.push_back(obj);
    }
    std::ofstream file(m_filePath);
    if (file.is_open()) {
        file << j.dump(4);
    }
}