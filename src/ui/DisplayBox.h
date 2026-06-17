#pragma once
#include "Card.h"

// ==================== DisplayBox —— 只读文本展示框 ====================
class DisplayBox : public Card
{
public:
    DisplayBox(int x, int y, int w, int h,
               const tstring& title = _T(""), int radius = 6);

    void setText(const tstring& t);
    void draw() const override;

private:
    tstring m_text;
};
