// 必须放在所有头文件包含之前. 预处理指令，编译优化，排除windows头文件（windows.h）中较少使用的组件
#define WIN32_LEAN_AND_MEAN	
// 必须放在所有头文件包含之前.  兼容性控制宏. 禁用微软对不安全函数（如scanf、strcpy等）的编译警告
#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <sstream>
#include <exception>
#include <format>
#include <stack>

#include <duilib/UIlib.h>
#include <windows.h>
#include <objbase.h>
#include <duilib/Core/UIContainer.h>

// libcurl 相关头文件
#include <curl/curl.h>

// JSON 解析库
#include <nlohmann/json.hpp>

using namespace DuiLib;
using json = nlohmann::json;

/**
 * @brief libcurl 数据接收回调函数
 * @param contents 服务器返回的数据块
 * @param size 单个数据块字节大小
 * @param nmemb 数据块个数
 * @param s 外部传入的string，用来拼接完整响应
 * @return 已处理的字节数
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s)
{
    size_t newLength = size * nmemb;
    try
    {
        // 把返回的数据追加到string中
        s->append((char*)contents, newLength);
    }
    catch (std::bad_alloc& e)
    {
        // 内存分配失败，返回0终止请求
        return 0;
    }
    return newLength;
}

/**
 * @brief 通用HTTP请求封装
 * @param url 请求地址
 * @param method 请求方式 GET/POST
 * @param body POST请求体JSON字符串
 * @return 服务器完整响应文本
 */
std::string HttpRequest(const char* url, const char* method, const char* body)
{
    // 初始化curl会话
    CURL* curl = curl_easy_init();
    if (!curl)
    {
        return "curl 初始化失败";
    }

    std::string response;

    // 设置请求地址
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // 允许跟随重定向
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    // 设置接收数据的回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    // 把response指针传给回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // 如果是POST请求且有请求体
    if (_stricmp(method, "POST") == 0 && body && strlen(body) > 0)
    {
        // 启用POST方式
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        // 设置POST提交的表单/JSON数据
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

        // 构造请求头：声明Content-Type为JSON
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    // 执行同步HTTP请求
    CURLcode res = curl_easy_perform(curl);
    // 请求出错
    if (res != CURLE_OK)
    {
        response = "请求失败: " + std::string(curl_easy_strerror(res));
    }

    // 释放curl资源
    curl_easy_cleanup(curl);
    return response;
}


// TODO 还需要设置字符集？

/*
class TestWindow : public CWindowWnd, public INotifyUI {
public:
    LPCTSTR GetWindowClassName() const override {
        return _T("TestWindowClass");
    }
    UINT GetClassStyle() const override {
        return CS_DBLCLKS;
    }
    void OnFinalMessage(HWND hWnd) override {
        delete this;
    }
    void Notify(TNotifyUI& msg) override {}
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override {
        if (uMsg == WM_CLOSE) {
            PostQuitMessage(0);
            return 0;
        }
        return __super::HandleMessage(uMsg, wParam, lParam);
    }
};
*/

/*
int main() {
	// 创建窗口
	TestWindow wnd;
	wnd.Create(nullptr, _T("Test"), UI_WNDSTYLE_FRAME, 0, 0, 500, 400);
	wnd.ShowModal();
}
*/

/*
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
*/

/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"到底是天地会还是整人会啊",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
*/

// 从WindowImplBase继承创建窗口类
/*
class CTestApp : public WindowImplBase {
public:
    virtual LPCTSTR GetWindowClassName() const {
        return _T("TestAppWindow");
    }

    virtual CDuiString GetSkinFile() {
        return _T("test1.xml"); // 布局文件
    }

    virtual void Notify(TNotifyUI& msg) {
        if (msg.sType == _T("click")) {
            if (msg.pSender->GetName() == _T("closebtn")) {
                Close();
            }
        }
    }

    virtual UILIB_RESOURCETYPE GetResourceType() const {
        return UILIB_FILE;  // 或UILIB_ZIPRESOURCE等
    }

    virtual CControlUI* CreateControl(LPCTSTR pstrClass) {
        return nullptr;  // 默认返回空，需根据实际需求处理
    }

    virtual CDuiString GetSkinFolder() {
        return CPaintManagerUI::GetInstancePath() + _T(".\\res\\");
    }
};
*/

// _tWinMain是Windows GUI应用程序的入口函数
/*
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    CPaintManagerUI::SetInstance(hInstance);  // 设置实例句柄
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T(".\\res\\"));    // 设置资源路径(布局文件路径)

    // 创建并显示窗口
    CTestApp app;
    app.Create(NULL, _T("TestApp"), UI_WNDSTYLE_FRAME, 0);
    app.CenterWindow();
    app.ShowModal();
    return 0;
}
*/


//===================================================================================

//const TCHAR* const kTitleControlName = _T("apptitle");
//const TCHAR* const kCloseButtonControlName = _T("closebtn");
//const TCHAR* const kMinButtonControlName = _T("minbtn");
//const TCHAR* const kMaxButtonControlName = _T("maxbtn");
//const TCHAR* const kRestoreButtonControlName = _T("restorebtn");


// 窗口实例及消息相应部分
class CFrameWindowWnd : public CWindowWnd, public INotifyUI
{
public:
    CFrameWindowWnd() {};

    // 窗口唯一标识符
    LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };

    // 定义窗口类样式。 UI_CLASSSTYLE_FRAME：窗口大小改变时触发重绘，通常用于带标题栏、边框的标准窗体。
    // CS_DBLCLKS‌：Windows 标准样式，启用窗口对鼠标双击消息的响应能力
    UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };

    // 处理窗口生命周期终结
    void OnFinalMessage(HWND /*hWnd*/) { delete this; };

    // 核心功能：控件事件分发、消息传递枢纽
    void Notify(TNotifyUI& msg) override
    {
        if (msg.sType == _T("click")) {
            //if (msg.pSender->GetName() == _T("I服了You")) { // 即点击的根控件root，代码见下面
            //    Close(); // 关闭窗口
            //}

            if (msg.pSender->GetName() == _T("closebtn")) { // 点击关闭窗口
                Close();
            } else if (msg.pSender->GetName() == _T("minbtn")) { // 最小化
                //ShowWindow(m_hWnd, SW_MINIMIZE); // 不起作用
                ::ShowWindow(m_hWnd, SW_MINIMIZE); // 通过全局作用域符强制调用Win32 API函数
            }
            else if (msg.pSender->GetName() == _T("maxbtn")) { // 最大化
                ::ShowWindow(m_hWnd, SW_MAXIMIZE);
                // 隐藏最大化按钮
                CButtonUI* maxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
                if (maxBtn) {
                    maxBtn->SetVisible(false);
                }
                // 显示恢复按钮
                CButtonUI* restoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
                if (restoreBtn) {
                    restoreBtn->SetVisible(true);
                }
            }
            else if (msg.pSender->GetName() == _T("restorebtn")) { // 恢复
                ::ShowWindow(m_hWnd, SW_RESTORE);
                CControlUI* restoreBtn = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if (restoreBtn) restoreBtn->SetVisible(false);
                CControlUI* maxBtn = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if (maxBtn) maxBtn->SetVisible(true);
            } 
            else if (msg.pSender->GetName() == _T("btn_test")) {
                // 测试读取输入框文本：
                CEditUI* editStr = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_str")));
                if (editStr) {
                    CDuiString strText = editStr->GetText(); // 获取文本内容
                    CTextUI* textStr = static_cast<CTextUI*>(m_pm.FindControl(_T("text_str")));
                    if (textStr) textStr->SetText(strText); // 刷新文本组件

                    // 测试显示消息盒
                    MessageBox(m_hWnd, L"安能摧眉折腰事权贵,使我不得开心颜！", L"消息标题", MB_OK | MB_ICONINFORMATION);
                }
            }
            else if (msg.pSender->GetName() == _T("send_btn")) {
                // 1. 获取URL输入框控件 & 文本
                CEditUI* urlEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("url_edit")));
                CDuiString urlW = urlEdit->GetText();

                // 2. 获取下拉框选中的请求方式 GET/POST
                CComboUI* combo = static_cast<CComboUI*>(m_pm.FindControl(_T("method_combo")));
                int selIdx = combo->GetCurSel();
                CListLabelElementUI* pItem = (CListLabelElementUI*)combo->GetItemAt(selIdx);
                CDuiString methodW = pItem ? pItem->GetText() : _T("GET");

                // 3. 获取请求体RichEdit输入内容
                CRichEditUI* bodyEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("body_edit")));
                CDuiString bodyW = bodyEdit->GetText();

                // 宽字符转窄字符，给libcurl使用
                auto WideToMulti = [](const std::wstring& wstr) -> std::string {
                    if (wstr.empty()) return "";
                    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
                    std::string res(len, 0);
                    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &res[0], len, NULL, NULL);
                    return res;
                    };
                std::string url = WideToMulti(urlW.GetData());
                std::string method = WideToMulti(methodW.GetData());
                std::string body = WideToMulti(bodyW.GetData());

                // 4. 开子线程执行网络请求
                // 避免阻塞UI主线程导致窗口卡死
                std::thread([=, this]() // 这里的等于号，意思是值捕获，以“值拷贝”的方式，捕获外部所有变量。 this是当前类的this指针
                    {
                        // 执行HTTP请求
                        std::string respResult = HttpRequest(url.c_str(), method.c_str(), body.c_str());

                        // 发送自定义消息到主线程，更新界面
                        // 不能在子线程直接操作Duilib控件
                         // PostMessage前面的:: 意思是调用全局的 Win32 API 函数，不调用该类里的同名函数
                        // 不直接传respResult， respResult是局部变量，出了作用域就销毁了。
                        ::PostMessage(m_hWnd, WM_USER + 100, 0, (LPARAM)new std::string(respResult));
                    }).detach(); // 分离线程，自动回收资源

                // 先给结果框显示 请求中 提示
                CRichEditUI* resultEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("result_edit")));
                if (resultEdit)
                {
                    resultEdit->SetText(_T("请求中，请稍候..."));
                }
            }
        }
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        // 自定义消息：子线程请求完成，更新响应结果到界面
        if (uMsg == WM_USER + 100)
        {
            std::string* pResp = reinterpret_cast<std::string*>(lParam);
            CRichEditUI* resultEdit = static_cast<CRichEditUI*>(m_pm.FindControl(_T("result_edit")));
            if (resultEdit && pResp)
            {
                // 窄字符转宽字符显示
                auto UTF8ToWide = [](const std::string& str) -> std::wstring {
                    if (str.empty()) return L"";
                    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
                    std::wstring res(len, L'\0');
                    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &res[0], len);
                    return res;
                    };
                std::wstring showText = UTF8ToWide(*pResp);
                resultEdit->SetText(showText.c_str());
            }
            // 释放堆内存
            delete pResp;
            return 0;
        }

        if (uMsg == WM_CREATE) { // 成功创建窗口后, 系统会立即发送 WM_CREATE 消息, 标志着窗口句柄已有效但尚未显示
            m_pm.Init(m_hWnd); // 绑定窗口句柄，初始化绘图环境
            //CControlUI* root = new CButtonUI;
            //root->SetName(_T("I服了You"));
            //root->SetBkColor(0xFFcccccc);
            //m_pm.AttachDialog(root); // 关联控件树根节点，启动界面渲染. 根控件与绘图管理器关联. 这里根控件为root

            // 这里用xml文件作为页面布局：
            CDialogBuilder builder;
            CControlUI* root = builder.Create(_T("test2.xml"), 0, nullptr, &m_pm);
            ASSERT(root && "Failed to parse XML");
            m_pm.AttachDialog(root);

            m_pm.AddNotifier(this); // 注册事件监听器，接收控件通知（如 Notify 消息）
            return 0;
        }
        else if (uMsg == WM_DESTROY) { // 窗口销毁
            ::PostQuitMessage(0); // 0表示正常退出
        }
        // 不想使用系统的标题栏和边框这些非客户区绘制，加上下面这俩分支（WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT 消息）的处理
        else if (uMsg == WM_NCACTIVATE) { // 窗口获得焦点或失去焦点时收到该消息
            if (!::IsIconic(m_hWnd)) {
                // TRUE: 允许系统将非客户区绘制为失活状态（灰色标题栏）
                // FALSE: 阻止系统默认的激活状态绘制，可能用于实现自定义标题栏样式
                return (wParam == 0) ? TRUE : FALSE;
            }
        }
        else if (uMsg == WM_NCCALCSIZE) { // 当窗口大小或位置发生变化时，系统会发送该消息
            return 0; // 返回0屏蔽系统默认的标题栏计算. 这使得开发者可以用客户区完全模拟自定义标题栏，实现更灵活的界面设计
        }
        else if (uMsg == WM_NCPAINT) { // 用于非客户区绘制的消息,在duilib中主要用于屏蔽系统默认标题栏以实现自定义界面
            return 0;
        } 
        else if (uMsg == WM_LBUTTONDOWN) {
            // 获取鼠标点击坐标
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            //ScreenToClient(m_hWnd, &pt); // 已经是客户区坐标了，不需要这一步转换
            
            // 找到标题栏控件
            CControlUI* pTitleBar = m_pm.FindControl(_T("title_bar"));
            if (pTitleBar) {
                // 判断鼠标点击是否在标题栏控件范围内
                RECT rcTitle = pTitleBar->GetPos();
                if (PtInRect(&rcTitle, pt)) {
                    CControlUI* pControl = m_pm.FindControl(pt);
                    bool isClickBtn = pControl && pControl->IsVisible() && pControl->IsEnabled() 
                        && dynamic_cast<CButtonUI*>(pControl) != nullptr;
                    if (!isClickBtn) {
                        // 发送消息让系统处理窗口移动
                        ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
                        return 0;
                    }
                }
            }
        }


        LRESULT lRes = 0;
        // 将 Windows 消息（uMsg）交由 CPaintManagerUI 处理(即交给对应控件处理)，若返回 true 表示消息已被消费，直接返回处理结果
        if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam); // 基类默认处理消息的流程
    }

   /* LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    }*/

public:
    CPaintManagerUI m_pm; // 承担界面绘制、消息处理和控件管理的核心职责
};

// 程序入口及Duilib初始化部分
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    //CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
    CDuiString path = CPaintManagerUI::GetInstancePath();
    // 设置资源路径：
    CPaintManagerUI::SetResourcePath(_T("skin\\")); // 相对路径

    CFrameWindowWnd* pFrame = new CFrameWindowWnd();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, _T("别人笑我太疯癫，我笑他人看不穿"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pFrame->ShowWindow(true);
    pFrame->CenterWindow(); // 居中显示
    CPaintManagerUI::MessageLoop();

    return 0;
}







