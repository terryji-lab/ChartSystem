# ChartSystem

基于 **C++14** 和 **EasyX** 图形库的 Windows 桌面图表可视化工具。加载 CSV 数据，以柱状图、饼图、折线图、面积图四种形式呈现，支持数据排序、统计分析和 PNG 导出。

## 编译配置

### 环境要求

| 组件 | 说明 |
| ---- | ---- |
| **MinGW-w64** | g++ ≥ 5.0，支持 C++14。推荐 [WinLibs](https://winlibs.com/) 或 [MSYS2](https://www.msys2.org/) 发行版 |
| **EasyX for MinGW** | Windows 轻量图形库，需手动安装到 MinGW 目录（见下方） |
| **Windows 7+** | EasyX 依赖 GDI32 等 Win32 API，仅支持 Windows |

### 安装 EasyX for MinGW

EasyX 官方提供 [EasyX_2022xxxx_mingw.zip](https://easyx.cn/download/easyx4mingw/) 离线包，安装步骤：

1. 下载 `EasyX4MinGW_xxxxxxxx.zip` 并解压
2. 将 `include/` 下的 `easyx.h`、`graphics.h` 复制到 MinGW 的 include 目录，例如：

   ```text
   <mingw>\x86_64-w64-mingw32\include\
   ```

3. 将 `lib/` 下的 `libeasyx.a` 复制到 MinGW 的 lib 目录，例如：

   ```text
   <mingw>\x86_64-w64-mingw32\lib\
   ```

> **验证安装**：在终端执行 `g++ -leasyx -dM -E - < nul 2>&1 | findstr easyx`，无报错即安装成功。

### 命令行编译

将 `<mingw>` 替换为你的 MinGW 安装路径，在项目根目录执行：

```bash
# === 编译（Debug 模式，带调试符号） ===
<mingw>\bin\g++ -std=c++14 -g -Isrc \
    src/main.cpp \
    src/chart/Chart.cpp \
    src/chart/BarChart.cpp \
    src/chart/PieChart.cpp \
    src/chart/LineChart.cpp \
    src/chart/AreaChart.cpp \
    src/ui/Button.cpp \
    src/ui/Card.cpp \
    src/ui/TextInput.cpp \
    src/ui/Pages.cpp \
    src/ui/ExportPathDialog.cpp \
    src/utils/FileDataReader.cpp \
    src/utils/ImageExporter.cpp \
    src/utils/FilePathUtils.cpp \
    -o main.exe \
    -leasyx -lgdi32 -lole32 -loleaut32 -luuid -lwinmm -lmsimg32

# === 编译（Release 模式，优化体积和速度） ===
<mingw>\bin\g++ -std=c++14 -O2 -s -Isrc \
    src/main.cpp \
    src/chart/Chart.cpp \
    ...（源文件同上）...
    -o main.exe \
    -leasyx -lgdi32 -lole32 -loleaut32 -luuid -lwinmm -lmsimg32 -mwindows

# === 运行 ===
./main.exe
```

> **注意**：Windows 原生 cmd 不支持 `\` 换行，请将命令写为一行，或使用 Git Bash / PowerShell。

#### 链接库说明

| 库 | 用途 |
| -- | ---- |
| `-leasyx` | EasyX 图形库（窗口、绘图、双缓冲） |
| `-lgdi32` | GDI32 — 底层图形设备接口，EasyX 依赖它完成像素级渲染 |
| `-lole32` | COM 基础组件，EasyX 图像加载需要 |
| `-loleaut32` | OLE Automation，配合 OLE32 使用 |
| `-luuid` | UUID 生成，COM 组件依赖 |
| `-lwinmm` | Windows 多媒体 API，EasyX 音频/计时相关 |
| `-lmsimg32` | 透明位图 AlphaBlend，EasyX 图像混合依赖 |

#### Debug vs Release

| 模式 | 编译选项 | 产物 | 适用场景 |
| ---- | -------- | ---- | -------- |
| **Debug** | `-g` | `main.exe`（较大，含符号） | 开发调试，可用 GDB 断点 |
| **Release** | `-O2 -s -mwindows` | `main.exe`（较小，去符号，无控制台窗口） | 分发使用 |

### VS Code 编译（Ctrl+Shift+B）

项目 `.vscode/tasks.json` 预置了两个任务：

| 任务 | 快捷键 | 说明 |
| ---- | ------ | ---- |
| `build ChartSystem` | `Ctrl+Shift+B` | 编译生成 `main.exe` |
| `run ChartSystem` | `Ctrl+Shift+B` → 选 run | 先编译再运行 |

> **⚠️ 首次使用请修改路径**：`tasks.json` 和 `c_cpp_properties.json` 中 `compilerPath` 写死为 `E:/mingw64/bin/g++.exe`，请替换为你本机的 MinGW 安装路径。搜索 `E:/mingw64` 全局替换即可。

## 功能

| 功能 | 说明 |
| ---- | ---- |
| **CSV 加载** | 支持 UTF-8 BOM / ANSI，自动跳过标题行 |
| **四种图表** | Bar（柱状图）、Pie（饼图）、Line（折线图）、Area（面积图） |
| **六套主题** | Classic / Ocean / Warm / Forest / Purple / Dark，图表与 UI 同步切换 |
| **数据统计** | 实时显示 Count / Max / Min / Average / Median |
| **多维排序** | 按值升/降序、按名称升/降序，Reset 一键恢复原始数据 |
| **批量导出** | Export All 一次性导出四种图表 PNG，Export Current 导出当前图表 |
| **导出路径对话框** | ExportPathDialog — 独立路径输入弹窗，支持相对/绝对路径 |
| **模态弹窗** | CSV 加载成功/失败/路径为空 均有 PopupCard 弹窗反馈 |
| **无数据提示** | 未加载数据时图表区居中显示 "No Data"（红色大字体） |
| **自适应标签** | 数据量 >10 条自动缩小字体，>25 条自动跳过标签避免重叠 |
| **双缓冲渲染** | `BeginBatchDraw()` / `EndBatchDraw()` 消除闪烁 |

## 使用

### CSV 格式

两列，首行为标题行（自动跳过）：

```csv
City,Revenue
Beijing,850
Shanghai,720
Shenzhen,680
```

### 操作

1. 输入 CSV 文件路径 → **Load CSV**
2. 选择图表类型：Bar / Pie / Line / Area
3. 图表页面内：
   - **Sort ↑ / Sort ↓** — 按值排序
   - **Sort Name ↑ / Sort Name ↓** — 按名称排序
   - **Reset** — 恢复原始数据
   - **Export Current** — 导出当前图表 PNG
4. 主页 **Export All** — 批量导出四种图表
5. **Theme** — 循环切换六套主题

### 快捷键

| 按键 | 场景 | 功能 |
| ---- | ---- | ---- |
| `ESC` | 全局 | 退出程序 |
| `Enter` / `Space` / `ESC` | 弹窗 | 关闭弹窗 |
| `Backspace` / `Delete` | 输入框 | 删除光标前/后字符 |
| `←` `→` | 输入框 | 移动光标 |
| `Home` / `End` | 输入框 | 光标跳至开头/末尾 |
| `Ctrl+A` | 输入框 | 全选 |
| `Ctrl+C` | 输入框 | 复制全部文本 |
| `Ctrl+V` | 输入框 | 粘贴 |
| `Ctrl+X` | 输入框 | 剪切全部文本 |

## 项目结构

```text
ChartSystem/
├── src/
│   ├── main.cpp                       # 入口：窗口、页面、事件循环
│   ├── common/
│   │   └── Types.h                    # 公共类型：tstring 统一定义
│   ├── chart/
│   │   ├── Chart.h / Chart.cpp        # 抽象基类 + ColorTheme + 颜色工具
│   │   ├── BarChart.h / .cpp          # 柱状图
│   │   ├── PieChart.h / .cpp          # 饼图
│   │   ├── LineChart.h / .cpp         # 折线图（含 PlotCoords 嵌套结构体）
│   │   └── AreaChart.h / .cpp         # 面积图（继承 LineChart）
│   ├── ui/
│   │   ├── Widget.h                   # 控件抽象基类
│   │   ├── Button.h / .cpp            # 圆角按钮（悬停/按下/回调）
│   │   ├── Card.h / .cpp              # Card 基类 + DisplayBox + PopupCard
│   │   ├── TextInput.h / .cpp         # 文本输入框（继承 Card）
│   │   ├── ExportPathDialog.h / .cpp  # 导出路径输入对话框
│   │   └── Pages.h / .cpp             # Page 基类 + MainPage + ChartPage + createChart
│   └── utils/
│       ├── RenderUtils.h              # drawRoundRectFill()
│       ├── FileDataReader.h / .cpp    # CSV 读取/解析
│       ├── DataAnalyzer.h             # 统计 + 排序
│       ├── ImageExporter.h / .cpp     # PNG 导出
│       └── FilePathUtils.h / .cpp     # 路径处理工具（修剪/拼接/创建目录）
├── data/                              # 示例数据（5 个 CSV）
├── example/                           # 导出示例（PNG 截图）
├── .vscode/                           # VS Code 编译/调试配置
└── README.md
```

## 类设计

### 图表继承树

```text
Chart（抽象基类 — draw() / applyTheme() 纯虚）
├── BarChart      柱状图：坐标轴 + 网格 + 多色柱子 + 数值标签
├── PieChart      饼图：扇形 + 图例 + 百分比标签
└── LineChart     折线图：坐标轴 + 网格 + 连线 + 数据点
    │   └── 嵌套结构体 PlotCoords（ptX/ptY/originX/Y/plot…）
    └── AreaChart    面积图：复用 LineChart 坐标计算，折线下填充
```

所有子类实现 `applyTheme(theme)` 接收主题色，一键切换图表配色。

### UI 继承树

```text
Widget（抽象基类 — 位置/尺寸/事件虚函数）
├── Button       独立按钮：圆角 + 三态颜色 + 投影 + onClick 回调
└── Card         圆角背景框：阴影 + 标题
    ├── TextInput    输入框：叠加文字 + 光标 + 键盘处理
    ├── DisplayBox   只读展示框：可变文本
    └── PopupCard    模态弹窗：accent 色条 + 图标 + 消息 + 详情 + OK 按钮

Page（页面基类 — vector<Widget*> 统一管理事件分发与绘制）
├── MainPage     主页：5 张卡片 + 输入框 + 7 个按钮
└── ChartPage    图表页：顶栏 + 统计卡片 + 图表区 + 7 个按钮

ExportPathDialog（独立对话框 — 组合 TextInput + Button，非 Widget 子类）
```

### 工厂函数

```cpp
// 定义于 Pages.h，根据 ChartType 枚举创建对应图表
std::unique_ptr<Chart> createChart(ChartType, title, data, theme);
```

### 设计要点

- **容器化事件分发**：`Page` 基类持有 `vector<Widget*>`，事件（MouseMove/Down/Up/KeyDown/Char）遍历分发，子类只覆盖 `draw()`。新增控件只需 `push_back`。
- **Card 继承复用**：`TextInput`、`DisplayBox` 和 `PopupCard` 继承 `Card`，背景和阴影由基类绘制，子类仅处理自身逻辑。
- **多态主题**：`Chart::applyTheme()` 和 `Page::applyTheme()` 各自实现，主题切换时图表和 UI 同步变色。
- **tstring 统一定义**：`src/common/Types.h` 显式 `#include <windows.h>` 后 `typedef std::basic_string<TCHAR> tstring`，所有文件通过 include 该头文件获取，不再依赖 EasyX 间接拉入 `TCHAR`。

## 颜色主题

六套主题，`ColorTheme` 结构体定义 10 个颜色字段：

| 字段 | 用途 |
| ---- | ---- |
| `barPalette[8]` | 柱子色 / 扇区色 / 线条色 / 填充色 |
| `axisColor` | 坐标轴颜色 |
| `gridColor` | 网格线颜色 |
| `textColor` | 文字颜色 |
| `bgColor` | 窗口背景色 |
| `cardColor` | 卡片背景色 |
| `btnNormal/Hover/Press` | 按钮三态颜色 |
| `accentColor` | 强调色（标题、描边） |

`darkenColor()` 和 `lightenColor()` 是两个内联颜色工具函数，定义在 `Chart.h` 中。

| 主题 | 色系 | 风格 |
| ---- | ---- | ---- |
| Classic | 多彩 | 通用演示 |
| Ocean | 蓝色 | 科技/商务 |
| Warm | 橙红 | 数据热度 |
| Forest | 绿色 | 自然/环境 |
| Purple | 紫色 | 创意/科技 |
| Dark | 深色 | 暗色背景 |

## 依赖

- **MinGW-w64 (g++ ≥ 5.0)** — C++14 支持
- **[EasyX](https://easyx.cn/)** for MinGW — Windows 轻量图形库
- **Windows 7+** — Win32 API
- **GDI32 / OLE32 / OLEAut32** — 系统图形与 COM 库

## 示例数据

| 文件 | 条数 | 内容 |
| ---- | ---- | ---- |
| `sales.csv` | 8 | 城市销售额 |
| `products.csv` | 15 | 产品销量 |
| `budget.csv` | 8 | 预算分类 |
| `monthly.csv` | 12 | 月度收入 |
| `large.csv` | ~110 | 区域数据（压力测试） |

## License

MIT
