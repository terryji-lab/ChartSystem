#pragma once
#include "Card.h"
#include "Button.h"

// ==================== PopupCard —— 模态弹窗 ====================
// 继承 Card，复用 drawBody() 画背景，叠加 accent 色条、图标、文字和 OK 按钮。
class PopupCard : public Card
{
public:
    PopupCard(int x, int y, int w, int h, const tstring& title);

    void setMessage(const tstring& msg);
    void setDetail(const tstring& detail);
    void setAccentColor(COLORREF c);
    void setButtonColors(COLORREF normal, COLORREF hover, COLORREF press);
    void setError(bool isError);

    void draw() const override;
    void showModal();

private:
    tstring  m_message;
    tstring  m_detail;
    COLORREF m_accentColor;
    Button   m_okBtn;
    bool     m_shouldClose;
    bool     m_isError;
};
