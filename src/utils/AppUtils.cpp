#include "AppUtils.h"
#include "../ui/ExportPathDialog.h"
#include "../ui/PopupCard.h"
#include "../utils/FilePathUtils.h"
#include "../utils/ImageExporter.h"

const TCHAR* getChartSuffix(ChartType type)
{
    switch (type)
    {
    case CHART_BAR:  return _T("_bar.png");
    case CHART_PIE:  return _T("_pie.png");
    case CHART_LINE: return _T("_line.png");
    case CHART_AREA: return _T("_area.png");
    default:         return _T("_chart.png");
    }
}

tstring shortenForPopup(const tstring& text, size_t maxLen)
{
    if (text.length() <= maxLen)
        return text;

    if (maxLen <= 3)
        return text.substr(0, maxLen);

    return text.substr(0, maxLen - 3) + _T("...");
}

void showThemePopup(const ColorTheme& theme,
                    const tstring& title,
                    const tstring& message,
                    const tstring& detail,
                    bool isError)
{
    PopupCard popup(
        (1200 - 500) / 2, (800 - 220) / 2, 500, 220,
        title
    );
    popup.setColors(darkenColor(theme.bgColor, 35),
                    theme.cardColor,
                    theme.textColor);

    if (isError)
    {
        popup.setAccentColor(RGB(220, 60, 60));
        popup.setButtonColors(RGB(200, 55, 55), RGB(235, 75, 75), RGB(160, 35, 35));
    }
    else
    {
        popup.setAccentColor(theme.accentColor);
        popup.setButtonColors(theme.btnNormal, theme.btnHover, theme.btnPress);
    }

    popup.setMessage(message);
    if (!detail.empty())
        popup.setDetail(shortenForPopup(detail, 56));
    popup.setError(isError);
    popup.showModal();
}

bool promptExportPath(const ColorTheme& theme,
                      const tstring& title,
                      const tstring& message,
                      const tstring& placeholder,
                      const tstring& defaultPath,
                      tstring& outPath)
{
    ExportPathDialog dialog(theme, title, message, placeholder, defaultPath);
    if (!dialog.showModal())
        return false;

    outPath = FilePathUtils::trim(dialog.getPath());
    return true;
}

void exportCurrentChartWithPrompt(const ColorTheme& theme,
                                  ChartType currentChart,
                                  const tstring& loadedTitle,
                                  const std::vector<ChartItem>& loadedData)
{
    if (loadedData.empty())
    {
        showThemePopup(theme,
                       _T("Export"),
                       _T("No data loaded. Please load a CSV file first."),
                       _T(""),
                       true);
        return;
    }

    tstring inputPath;
    tstring defaultPath = FilePathUtils::safeFileName(loadedTitle) + getChartSuffix(currentChart);
    if (!promptExportPath(theme,
                          _T("Export Current"),
                          _T("Enter a save path for the current chart:"),
                          _T("Relative or absolute path, e.g. exports\\chart.png or D:\\charts\\chart.png"),
                          defaultPath,
                          inputPath))
    {
        return;
    }

    tstring savePath = FilePathUtils::buildSingleChartPath(inputPath, loadedTitle, getChartSuffix(currentChart));
    if (savePath.empty())
    {
        showThemePopup(theme,
                       _T("Export"),
                       _T("Please enter a valid save path."),
                       _T(""),
                       true);
        return;
    }

    if (!FilePathUtils::ensureParentDirectoryExists(savePath))
    {
        showThemePopup(theme,
                       _T("Export"),
                       _T("Export failed."),
                       _T("Could not create the destination folder."),
                       true);
        return;
    }

    auto chart = createChart(currentChart, loadedTitle, loadedData, theme);
    if (chart && ImageExporter::exportChart(*chart, savePath))
    {
        showThemePopup(theme,
                       _T("Export"),
                       _T("Exported current chart as PNG."),
                       savePath);
    }
    else
    {
        showThemePopup(theme,
                       _T("Export"),
                       _T("Export failed."),
                       savePath,
                       true);
    }
}

void exportAllChartsWithPrompt(const ColorTheme& theme,
                               const tstring& loadedTitle,
                               const std::vector<ChartItem>& loadedData)
{
    if (loadedData.empty())
    {
        showThemePopup(theme,
                       _T("Export"),
                       _T("No data loaded. Please load a CSV file first."),
                       _T(""),
                       true);
        return;
    }

    tstring inputPath;
    tstring defaultPath = FilePathUtils::safeFileName(loadedTitle);
    if (!promptExportPath(theme,
                          _T("Export All"),
                          _T("Enter a save path for exporting all charts:"),
                          _T("Relative or absolute path, e.g. exports\\ or D:\\charts\\all.png"),
                          defaultPath,
                          inputPath))
    {
        return;
    }

    ChartType allTypes[] = { CHART_BAR, CHART_PIE, CHART_LINE, CHART_AREA };
    const TCHAR* suffixes[] = { _T("_bar.png"), _T("_pie.png"), _T("_line.png"), _T("_area.png") };
    bool allOk = true;

    for (int i = 0; i < 4; i++)
    {
        tstring savePath = FilePathUtils::buildExportAllChartPath(inputPath, loadedTitle, suffixes[i]);
        if (savePath.empty())
        {
            allOk = false;
            continue;
        }

        if (!FilePathUtils::ensureParentDirectoryExists(savePath))
        {
            allOk = false;
            continue;
        }

        auto chart = createChart(allTypes[i], loadedTitle, loadedData, theme);
        if (!chart || !ImageExporter::exportChart(*chart, savePath))
            allOk = false;
    }

    if (allOk)
    {
        showThemePopup(theme,
                       _T("Export"),
                       _T("Exported: bar / pie / line / area PNG files."),
                       inputPath);
    }
    else
    {
        showThemePopup(theme,
                       _T("Export"),
                       _T("Export failed."),
                       _T("Please check the save path and permissions."),
                       true);
    }
}
