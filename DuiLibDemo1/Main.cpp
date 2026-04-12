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

using namespace DuiLib;

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
    void Notify(TNotifyUI& msg)
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
        }
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
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







