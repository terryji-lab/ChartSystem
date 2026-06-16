#include "Button.h"
#include "../utils/RenderUtils.h"

// ==================== 构造 ====================
Button::Button(int x, int y, int width, int height, const tstring& text,
               std::function<void()> onClick, int cornerRadius)
    : Widget(x, y, width, height)
    , m_radius(cornerRadius)
    , m_text(text)
    , m_isHovered(false)
    , m_isPressed(false)
    , m_visible(true)
    , m_onClick(onClick)
    , m_colorNormal(RGB(70, 130, 210))
    , m_colorHover(RGB(100, 160, 235))
    , m_colorPress(RGB(50, 100, 175))
    , m_colorBorder(RGB(50, 110, 190))
    , m_colorText(WHITE)
    , m_colorShadow(RGB(180, 190, 210))
{
}

// ==================== 事件处理 ====================
bool Button::contains(int mx, int my) const
{
    return m_visible
        && mx >= m_x && mx <= m_x + m_w
        && my >= m_y && my <= m_y + m_h;
}

bool Button::handleMouseMove(int mx, int my)
{
    if (!m_visible) return false;
    bool wasHovered = m_isHovered;
    m_isHovered = contains(mx, my);
    return (wasHovered != m_isHovered);
}

bool Button::handleMouseDown(int mx, int my)
{
    if (!m_visible) return false;
    if (contains(mx, my) && !m_isPressed)
    {
        m_isPressed = true;
        return true;
    }
    return false;
}

bool Button::handleMouseUp(int mx, int my)
{
    if (!m_visible) return false;
    bool wasPressed = m_isPressed;
    m_isPressed = false;
    if (wasPressed && contains(mx, my))
    {
        if (m_onClick) m_onClick();
    }
    return wasPressed;
}

// ==================== 绘制 ====================
void Button::draw() const
{
    if (!m_visible) return;

    COLORREF fillColor = m_colorNormal;
    if (m_isPressed)
        fillColor = m_colorPress;
    else if (m_isHovered)
        fillColor = m_colorHover;

    // 1. 阴影（仅在普通/悬停状态绘制）
    if (!m_isPressed && m_radius > 0)
    {
        int so = 3;
        drawRoundRectFill(m_x + so, m_y + so, m_w, m_h, m_radius, m_colorShadow);
    }

    // 2. 按钮主体（圆角矩形）
    drawRoundRectFill(m_x, m_y, m_w, m_h, m_radius, fillColor);

    // 3. 细线边框
    setlinecolor(m_colorBorder);
    if (m_radius > 0)
    {
        int r = m_radius;
        int x = m_x, y = m_y, w = m_w, h = m_h;
        line(x + r, y,         x + w - r, y);
        line(x + r, y + h,     x + w - r, y + h);
        line(x,     y + r,     x,         y + h - r);
        line(x + w, y + r,     x + w,     y + h - r);
        const double PI = 3.141592653589793;
        arc(x, y,                 x + r * 2, y + r * 2,     PI,       PI * 1.5);
        arc(x + w - r * 2, y,     x + w,     y + r * 2,     PI * 1.5, PI * 2.0);
        arc(x + w - r * 2, y + h - r * 2, x + w, y + h,     0,        PI * 0.5);
        arc(x,     y + h - r * 2, x + r * 2, y + h,         PI * 0.5, PI);
    }
    else
    {
        rectangle(m_x, m_y, m_x + m_w, m_y + m_h);
    }

    // 4. 文字（居中）
    settextcolor(m_colorText);
    setbkmode(TRANSPARENT);
    settextstyle(22, 0, _T("Microsoft YaHei"), 0, 0, FW_SEMIBOLD, false, false, false);

    int tw = textwidth(m_text.c_str());
    int th = textheight(m_text.c_str());
    int tx = m_x + (m_w - tw) / 2;
    int ty = m_y + (m_h - th) / 2;
    if (tx < m_x + 4) tx = m_x + 4;
    outtextxy(tx, ty, m_text.c_str());
}

// ==================== Setter ====================
void Button::setText(const tstring& text)        { m_text = text; }
void Button::setPosition(int x, int y)           { m_x = x; m_y = y; }
void Button::setVisible(bool visible)            { m_visible = visible; }
void Button::setOnClick(std::function<void()> cb){ m_onClick = cb; }
void Button::setColorNormal(COLORREF c)          { m_colorNormal = c; }
void Button::setColorHover(COLORREF c)           { m_colorHover = c; }
void Button::setColorPress(COLORREF c)           { m_colorPress = c; }
void Button::setColorBorder(COLORREF c)          { m_colorBorder = c; }
void Button::setColorText(COLORREF c)            { m_colorText = c; }
void Button::setColorShadow(COLORREF c)          { m_colorShadow = c; }
void Button::setRadius(int r)                    { m_radius = r; }

const tstring& Button::getText() const           { return m_text; }
bool Button::isVisible() const                   { return m_visible; }
