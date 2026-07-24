// Copyright 2026 Caspian-dev813
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @example 02_verify_window.cpp
 * @brief   演示: 带时间窗口的 TOTP 验证（生产环境推荐）
 * @build   g++ -std=c++11 -I../include -o 02_verify_window 02_verify_window.cpp
 */
#include <totp/validator.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP";

    std::cout << "=== 时间窗口验证 ===\n";
    std::cout << "请输入当前验证码: ";
    std::string input; std::cin >> input;

    // ±1 窗口 = 允许前后各30秒偏差
    bool ok = totp::verify(secret, input, /*window=*/1);
    std::cout << "验证结果: " << (ok ? "✅ 通过" : "❌ 失败") << "\n";

    // ±2 窗口 = 允许前后各60秒偏差（更宽松）
    bool ok2 = totp::verify(secret, input, /*window=*/2);
    std::cout << "宽松模式: " << (ok2 ? "✅ 通过" : "❌ 失败") << "\n";

    return 0;
}