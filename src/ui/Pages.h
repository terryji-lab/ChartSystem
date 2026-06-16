#pragma once
#include <easyx.h>
#include <vector>
#include <memory>
#include <functional>
#include "Widget.h"
#include "Button.h"
#include "TextInput.h"
#include "Card.h"
#include "chart/Chart.h"

enum ChartType { CHART_BAR, CHART_PIE, CHART_LINE, CHART_AREA };

// 工厂函数
std::unique_ptr<Chart> createChart(
    ChartType type,
    const tstring& title,
    const std::vector<ChartItem>& data,
    const ColorTheme& theme
);

// ==================== Page 基类 —— 统一管理所有 Widget ====================
class Page
{
protected:
    std::vector<Widget*> m_widgets;   // Card / Button / TextInput / PopupCard …

public:
    virtual ~Page() = default;

    // ── 默认绘制 ──
    virtual void draw()
    {
        for (auto* w : m_widgets) w->draw();
    }

    // ── 默认事件分发（通过 Widget 虚函数多态）──
    virtual bool handleMouseMove(int mx, int my)
    {
        bool changed = false;
        for (auto* w : m_widgets) changed |= w->handleMouseMove(mx, my);
        return changed;
    }

    virtual bool handleMouseDown(int mx, int my)
    {
        bool changed = false;
        for (auto* w : m_widgets) changed |= w->handleMouseDown(mx, my);
        return changed;
    }

    virtual bool handleMouseUp(int mx, int my)
    {
        bool changed = false;
        for (auto* w : m_widgets) changed |= w->handleMouseUp(mx, my);
        return changed;
    }

    virtual bool handleKeyDown(WPARAM vkcode)
    {
        for (auto* w : m_widgets)
        {
            if (w->wantsKeyInput()) { w->handleKeyDown(vkcode); return true; }
        }
        return false;
    }

    virtual bool handleChar(WPARAM ch)
    {
        for (auto* w : m_widgets)
        {
            if (w->wantsCharInput()) { w->handleChar(ch); return true; }
        }
        return false;
    }
};

// ==================== 主页面 ====================
class MainPage : public Page
{
public:
    using Callback = std::function<void()>;
    using ChartSelectCallback = std::function<void(ChartType)>;
    using LoadCSVCallback = std::function<void(const tstring& path)>;

    MainPage(
        const ColorTheme& theme,
        ChartSelectCallback onChartSelect,
        LoadCSVCallback onLoadCSV,
        Callback onExport,
        Callback onThemeSwitch
    );

    void draw() override;

    const tstring& getCSVPath() const;
    void setThemeButtonText(const tstring& text);
    void applyTheme(const ColorTheme& theme);

private:
    void drawSidePanel();

    ChartSelectCallback m_onChartSelect;
    LoadCSVCallback      m_onLoadCSV;
    Callback             m_onExport;
    Callback             m_onThemeSwitch;
    ColorTheme           m_theme;

    Card      m_cardData;
    Card      m_cardChart;
    Card      m_cardActions;
    Card      m_cardInstr;
    Card      m_cardTips;

    TextInput txtCSV;
    Button    btnReadCSV;
    Button    btnBarChart;
    Button    btnPieChart;
    Button    btnLineChart;
    Button    btnAreaChart;
    Button    btnExport;
    Button    btnTheme;
};

// ==================== 图表页面 ====================
class ChartPage : public Page
{
public:
    using Callback = std::function<void()>;

    ChartPage(const ColorTheme& theme,
              Callback onBack,
              Callback onExport,
              Callback onSortAsc, Callback onSortDesc,
              Callback onSortNameAsc, Callback onSortNameDesc,
              Callback onReset);

    void draw() override;

    void setChartData(ChartType type, const std::vector<ChartItem>& data,
                      const tstring& title, const ColorTheme& theme);
    void applyTheme(const ColorTheme& theme);

private:
    Callback   m_onBack;
    Callback   m_onExport;
    Callback   m_onSortAsc;
    Callback   m_onSortDesc;
    Callback   m_onSortNameAsc;
    Callback   m_onSortNameDesc;
    Callback   m_onReset;
    ColorTheme m_theme;

    Card      m_cardTopBar;
    Card      m_cardStats;

    Button    btnExport;
    Button    btnReset;
    Button    btnSortNameAsc;
    Button    btnSortNameDesc;
    Button    btnSortAsc;
    Button    btnSortDesc;
    Button    btnBack;

    ChartType              m_chartType;
    std::vector<ChartItem> m_data;
    tstring                m_title;
    ColorTheme             m_chartTheme;
    bool                   m_hasData;
};
