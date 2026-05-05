#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>   // 需要包含 json 头文件

using json = nlohmann::json;

// 历史记录项
struct HistoryItem {
    std::string method;
    std::string url;
    std::string headers;
    std::string body;
};

// 历史记录管理器（数据层，与UI解耦）
class HistoryManager {
public:
    HistoryManager();
    ~HistoryManager();

    // 添加一条（自动存在内存，并保存到文件）
    void Add(const std::string& method, const std::string& url,
        const std::string& headers, const std::string& body);

    // 获取所有记录（只读）
    const std::vector<HistoryItem>& GetAll() const { return m_items; }

    // 清空所有
    void Clear();

    // 加载文件（在窗口初始化时调用）
    void LoadFromFile();

    // 保存到文件（Add内部自动调用，也可手动调用）
    void SaveToFile() const;

private:
    std::vector<HistoryItem> m_items;
    static constexpr size_t MAX_HISTORY = 50;          // 最多保留50条
    std::string m_filePath;                            // history.json 的完整路径
    void EnsureDirectoryExists();                      // 确保目录存在
};