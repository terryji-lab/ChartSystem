#include <easyx.h>
#include <iostream>
#include <memory>
#include "ui/Button.h"
#include "ui/ExportPathDialog.h"
#include "ui/MainPage.h"
#include "ui/ChartPage.h"
#include "ui/TextInput.h"
#include "utils/FileDataReader.h"
#include "utils/FilePathUtils.h"
#include "utils/ImageExporter.h"
#include "utils/DataAnalyzer.h"
#include "common/Themes.h"
#include "utils/AppUtils.h"

int main()
{
    // ==================== 共享状态（在 main 中集中管理） ====================
    std::vector<ChartItem> loadedData;       // 当前显示的图表数据
    std::vector<ChartItem> originalData;     // 原始数据备份（用于 Reset）
    tstring loadedTitle = _T("Chart");       // 当前图表标题（取自 CSV 文件名）
    int  themeIdx = 0;                       // 当前主题索引（0-5 循环）
    ChartType currentChart = CHART_BAR;      // 当前图表类型
    bool needRedraw = true;                  // 是否需要重绘标志

    // ── 初始化 EasyX 图形窗口（1200×800）──
    initgraph(1200, 800);
    setbkcolor(PRESET_THEMES[themeIdx].bgColor);

    // 获取窗口句柄，用于 MessageBox 和 Clipboard 等 Win32 API 调用
    HWND hwnd = GetHWnd();

    // 当前活动页面指针（用于事件分发和绘制）
    Page* currentPage = nullptr;
    std::unique_ptr<MainPage>  mainPage;
    std::unique_ptr<ChartPage> chartPage;

    // ==================== 创建主页面 ====================
    const auto& initialTheme = PRESET_THEMES[themeIdx];
    mainPage = std::make_unique<MainPage>(
        initialTheme,
        // ── onChartSelect：选择了图表类型，切换到 ChartPage ──
        [&](ChartType t){
            currentChart = t;
            chartPage->setChartData(t, loadedData, loadedTitle, PRESET_THEMES[themeIdx]);
            currentPage = chartPage.get();
            needRedraw = true;
        },
        // ── onLoadCSV：加载 CSV 文件 ──
        [&](const tstring& path){
            if (path.empty())
            {
                // 路径为空时弹出错误提示
                const auto& popupTheme = PRESET_THEMES[themeIdx];
                showThemePopup(popupTheme, _T("Load CSV"), _T("Please enter a CSV file path."), _T(""), true);
                return;
            }
            FileDataReader reader;
            if (reader.loadFromPath(path))
            {
                // 加载成功
                loadedData = reader.getData();
                originalData = loadedData;               // 保存原始数据备份
                // 从文件路径中提取文件名作为图表标题
                size_t slash = path.find_last_of(_T("\\/"));
                size_t dot   = path.find_last_of(_T('.'));
                if (slash == tstring::npos) slash = -1;
                if (dot == tstring::npos || dot <= slash) dot = path.length();
                loadedTitle = path.substr(slash + 1, dot - slash - 1);

                // 文件名截断（超过 28 字符时用 "..." 省略）
                tstring displayName = loadedTitle.empty() ? _T("CSV file") : loadedTitle;
                if (displayName.length() > 28)
                    displayName = displayName.substr(0, 25) + _T("...");

                // 使用 PopupCard 弹窗显示成功信息
                const auto& popupTheme = PRESET_THEMES[themeIdx];
                showThemePopup(popupTheme, _T("File Imported"), _T("Data file loaded successfully:"), displayName, false);

                needRedraw = true;
            }
            else
            {
                // 加载失败，弹出错误提示
                tstring errMsg = reader.getErrorMessage();
                if (errMsg.length() > 55)
                    errMsg = errMsg.substr(0, 52) + _T("...");

                const auto& popupTheme = PRESET_THEMES[themeIdx];
                showThemePopup(popupTheme, _T("Load Failed"), _T("Could not read the file:"), errMsg, true);
            }
        },
        // ── onExport：批量导出四种图表的 PNG ──
        [&](){
            exportAllChartsWithPrompt(PRESET_THEMES[themeIdx], loadedTitle, loadedData);
        },
        // ── onThemeSwitch：循环切换颜色主题 ──
        [&](){
            themeIdx = (themeIdx + 1) % int(PRESET_THEMES.size());
            const auto& newTheme = PRESET_THEMES[themeIdx];
            mainPage->setThemeButtonText(_T("Theme: ") + newTheme.name);
            mainPage->applyTheme(newTheme);
            chartPage->applyTheme(newTheme);
            setbkcolor(newTheme.bgColor);
            needRedraw = true;
        }
    );
    mainPage->setThemeButtonText(_T("Theme: Classic"));

    // ==================== 创建图表页面 ====================
    chartPage = std::make_unique<ChartPage>(
        initialTheme,
        // ── onBack：返回主页面 ──
        [&](){
            currentPage = mainPage.get();
            needRedraw = true;
        },
        // ── onExport：导出当前图表为 PNG ──
        [&](){
            exportCurrentChartWithPrompt(PRESET_THEMES[themeIdx], currentChart, loadedTitle, loadedData);
        },
        // ── onSortAsc：按数值升序排序 ──
        [&](){
            if (loadedData.empty()) return;
            DataAnalyzer da(loadedData);
            loadedData = da.sortByValue(true);
            chartPage->setChartData(currentChart, loadedData, loadedTitle, PRESET_THEMES[themeIdx]);
            needRedraw = true;
        },
        // ── onSortDesc：按数值降序排序 ──
        [&](){
            if (loadedData.empty()) return;
            DataAnalyzer da(loadedData);
            loadedData = da.sortByValue(false);
            chartPage->setChartData(currentChart, loadedData, loadedTitle, PRESET_THEMES[themeIdx]);
            needRedraw = true;
        },
        // ── onSortNameAsc：按名称升序排序 ──
        [&](){
            if (loadedData.empty()) return;
            DataAnalyzer da(loadedData);
            loadedData = da.sortByName(true);
            chartPage->setChartData(currentChart, loadedData, loadedTitle, PRESET_THEMES[themeIdx]);
            needRedraw = true;
        },
        // ── onSortNameDesc：按名称降序排序 ──
        [&](){
            if (loadedData.empty()) return;
            DataAnalyzer da(loadedData);
            loadedData = da.sortByName(false);
            chartPage->setChartData(currentChart, loadedData, loadedTitle, PRESET_THEMES[themeIdx]);
            needRedraw = true;
        },
        // ── onReset：恢复为原始数据 ──
        [&](){
            if (originalData.empty()) return;
            loadedData = originalData;
            chartPage->setChartData(currentChart, loadedData, loadedTitle, PRESET_THEMES[themeIdx]);
            needRedraw = true;
        }
    );

    // 默认显示主页面
    currentPage = mainPage.get();

    // ── 首次绘制（使用双缓冲避免闪烁）──
    BeginBatchDraw();
    currentPage->draw();
    EndBatchDraw();

    // ==================== 主事件循环 ====================
    // 监听鼠标、键盘、窗口消息，按需分发到当前页面并触发重绘。
    // 使用 needRedraw 标志避免不必要的重绘，提升性能。
    ExMessage msg;
    bool running = true;

    while (running && IsWindow(hwnd))
    {
        // 获取下一条消息（同时监听鼠标、键盘、字符输入和窗口事件）
        msg = getmessage(EX_MOUSE | EX_KEY | EX_CHAR | EX_WINDOW);

        switch (msg.message)
        {
        case WM_CLOSE:                            // 窗口关闭按钮
            running = false;
            break;

        case WM_KEYDOWN:                          // 键盘按下
            if (msg.vkcode == VK_ESCAPE) running = false;  // ESC 退出
            needRedraw |= currentPage->handleKeyDown(msg.vkcode);
            break;

        case WM_CHAR:                             // 字符输入（用于 TextInput）
            needRedraw |= currentPage->handleChar(msg.ch);
            break;

        case WM_MOUSEMOVE:                        // 鼠标移动
            needRedraw |= currentPage->handleMouseMove(msg.x, msg.y);
            break;

        case WM_LBUTTONDOWN:                      // 鼠标左键按下
            needRedraw |= currentPage->handleMouseDown(msg.x, msg.y);
            break;

        case WM_LBUTTONUP:                        // 鼠标左键释放
            needRedraw |= currentPage->handleMouseUp(msg.x, msg.y);
            break;
        }

        // 仅在需要时重绘（双缓冲）
        if (needRedraw)
        {
            needRedraw = false;
            BeginBatchDraw();
            cleardevice();                        // 清空屏幕
            currentPage->draw();                  // 重新绘制当前页面
            EndBatchDraw();
        }
    }

    // ── 程序退出 ──
    std::cout << "Program exited";
    closegraph();                                 // 关闭图形窗口
    return 0;
}
