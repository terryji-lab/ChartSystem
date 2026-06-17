#pragma once
#include <vector>
#include <easyx.h>
#include "Widget.h"

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
