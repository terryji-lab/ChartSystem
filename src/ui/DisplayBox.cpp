#include "DisplayBox.h"

// ==================== DisplayBox —— 只读文本展示框 ====================

DisplayBox::DisplayBox(int x, int y, int w, int h, const tstring& title, int radius)
    : Card(x, y, w, h, title, radius)
{
}

void DisplayBox::setText(const tstring& t)
{
    m_text = t;
}

void DisplayBox::draw() const
{
    Card::draw();  // 复用 Card 的阴影+背景+标题

    // 在标题下方绘制内容文本
    if (!m_text.empty())
    {
        settextstyle(18, 0, _T("Microsoft YaHei"), 0, 0, FW_SEMIBOLD, false, false, false);
        settextcolor(m_titleColor);
        outtextxy(m_x + 14, m_y + 36, m_text.c_str());
    }
}
