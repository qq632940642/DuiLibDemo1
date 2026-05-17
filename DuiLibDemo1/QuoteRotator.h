#pragma once
#include <vector>
#include <string>

class QuoteRotator {
public:
    QuoteRotator();
    ~QuoteRotator();

    bool LoadFromFile(const std::string& filePath);
    std::wstring GetCurrentQuote() const;
    std::wstring NextQuote();
    size_t Count() const;

private:
    std::vector<std::wstring> m_quotes;
    size_t m_index;
};