#include "ChartFactory.h"
#include "BarChart.h"
#include "PieChart.h"
#include "LineChart.h"
#include "AreaChart.h"

std::unique_ptr<Chart> createChart(
    ChartType type,
    const tstring& title,
    const std::vector<ChartItem>& data,
    const ColorTheme& theme
)
{
    std::unique_ptr<Chart> chart;
    switch(type)
    {
    case CHART_BAR:
        chart = std::make_unique<BarChart>(title, data, 80, 60, 1040, 680);
        break;
    case CHART_PIE:
        chart = std::make_unique<PieChart>(title, data, 80, 60, 1040, 680, 280);
        break;
    case CHART_LINE:
        chart = std::make_unique<LineChart>(title, data, 80, 60, 1040, 680);
        break;
    case CHART_AREA:
        chart = std::make_unique<AreaChart>(title, data, 80, 60, 1040, 680);
        break;
    default:
        return nullptr;
    }
    // 应用主题配色
    chart->applyTheme(theme);
    return chart;
}
