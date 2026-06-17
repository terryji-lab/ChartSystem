#pragma once
#include <memory>
#include <vector>
#include "Chart.h"
#include "../common/Types.h"

enum ChartType { CHART_BAR, CHART_PIE, CHART_LINE, CHART_AREA };

// 工厂函数
std::unique_ptr<Chart> createChart(
    ChartType type,
    const tstring& title,
    const std::vector<ChartItem>& data,
    const ColorTheme& theme
);
