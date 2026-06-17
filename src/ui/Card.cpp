#include "Card.h"
#include "../chart/Chart.h"   // darkenColor, lightenColor

// ==================== Card 基类 ====================

Card::Card(int x, int y, int w, int h, const tstring& title, int radius)
    : Widget(x, y, w, h)
    , m_radius(radius)
    , m_title(title)
    , m_shadowColor(RGB(200, 200, 210))         // 默认浅灰阴影
    , m_bgColor(WHITE)                          // 默认白色背景
    , m_titleColor(RGB(50, 50, 80))             // 默认深色标题
{
}

// 绘制阴影（在主体右下方偏移 2-3px 的圆角矩形）
void Card::drawShadow() const
{
    if (m_radius > 0)
        drawRoundRectFill(m_x + 2, m_y + 3, m_w, m_h, m_radius, m_shadowColor);
}

// 绘制主体背景（圆角矩形填充）
void Card::drawBody() const
{
    drawRoundRectFill(m_x, m_y, m_w, m_h, m_radius, m_bgColor);
}

// 绘制标题文字（粗体，左上角）
void Card::drawTitle() const
{
    if (!m_title.empty())
    {
        settextstyle(18, 0, _T("Microsoft YaHei"), 0, 0, FW_BOLD, false, false, false);
        settextcolor(m_titleColor);
        outtextxy(m_x + 14, m_y + 10, m_title.c_str());
    }
}

// 默认绘制：阴影 → 主体 → 标题
void Card::draw() const
{
    drawShadow();
    drawBody();
    drawTitle();
}

// 设置卡片主题配色（阴影、背景、标题）
void Card::setColors(COLORREF shadow, COLORREF bg, COLORREF title)
{
    m_shadowColor = shadow;
    m_bgColor = bg;
    m_titleColor = title;
}

void Card::setTitle(const tstring& t)
{
    m_title = t;
}
