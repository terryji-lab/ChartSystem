#pragma once
#include <vector>
#include "../common/Types.h"
#include "../chart/ChartFactory.h"

// 获取图表类型对应的 PNG 文件后缀，例如 CHART_BAR 返回 "_bar.png"
const TCHAR* getChartSuffix(ChartType type);

// 缩短用于弹窗显示的字符串
tstring shortenForPopup(const tstring& text, size_t maxLen);

// 显示带有主题配色的弹窗
void showThemePopup(const ColorTheme& theme,
                    const tstring& title,
                    const tstring& message,
                    const tstring& detail = _T(""),
                    bool isError = false);

// 提示用户输入导出路径
bool promptExportPath(const ColorTheme& theme,
                      const tstring& title,
                      const tstring& message,
                      const tstring& placeholder,
                      const tstring& defaultPath,
                      tstring& outPath);

// 导出当前图表，并带有路径提示弹窗
void exportCurrentChartWithPrompt(const ColorTheme& theme,
                                  ChartType currentChart,
                                  const tstring& loadedTitle,
                                  const std::vector<ChartItem>& loadedData);

// 导出所有类型图表，并带有路径提示弹窗
void exportAllChartsWithPrompt(const ColorTheme& theme,
                               const tstring& loadedTitle,
                               const std::vector<ChartItem>& loadedData);
