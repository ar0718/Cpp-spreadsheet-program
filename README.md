# 📊 C++ Command-Line Spreadsheet

A terminal-based spreadsheet engine built in C++, supporting formula evaluation, cell dependencies, range validation, and responsive viewport navigation.

## 🚀 Features

- Supports sheets up to **999×18,278 cells (A1 to ZZZ999)**
- **Formula evaluation** with operators (+, -, *, /)
- **Dependency graph** for tracking references between cells
- **Cycle detection** using DFS
- **Topological sorting** for minimal updates
- **10×10 viewport navigation** via `W`, `A`, `S`, `D` keys
-  Built-in **aggregate functions**: `MAX`, `MIN`, `AVG`, `STDEV`, `SUM` over rectangular cell ranges
- Efficient selective reevaluation after updates

