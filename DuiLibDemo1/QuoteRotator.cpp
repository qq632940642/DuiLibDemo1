#include "QuoteRotator.h"
#include <fstream>
#include <windows.h>

static std::wstring Utf8ToWide(const std::string& utf8Str) {
    if (utf8Str.empty()) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), (int)utf8Str.size(), nullptr, 0);
    if (len <= 0) return L"";
    std::wstring wide(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), (int)utf8Str.size(), &wide[0], len);
    return wide;
}

QuoteRotator::QuoteRotator() : m_index(0) {}

QuoteRotator::~QuoteRotator() {}

bool QuoteRotator::LoadFromFile(const std::string& filePath) {
    m_quotes.clear();
    std::ifstream file(filePath);
    if (!file.is_open()) return false;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        m_quotes.push_back(Utf8ToWide(line));
    }
    file.close();
    if (m_quotes.empty()) {
        // 默认备用内容
        m_quotes.push_back(L"欢迎使用API测试工具");
        m_quotes.push_back(L"代码改变世界");
    }
    m_index = 0;
    return true;
}

std::wstring QuoteRotator::GetCurrentQuote() const {
    if (m_quotes.empty()) return L"";
    return m_quotes[m_index];
}

std::wstring QuoteRotator::NextQuote() {
    if (m_quotes.empty()) return L"";
    m_index = (m_index + 1) % m_quotes.size();
    return m_quotes[m_index];
}

size_t QuoteRotator::Count() const {
    return m_quotes.size();
}