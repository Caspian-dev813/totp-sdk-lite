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
 * @example 03_custom_config.cpp
 * @brief   演示: 自定义码长、周期、哈希算法
 * @build   g++ -std=c++11 -I../include -o 03_custom_config 03_custom_config.cpp
 */
#include <totp/totp.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP";

    std::cout << "=== 自定义配置 ===\n";

    // 8位码 + SHA256 + 60秒周期
    totp::Config secure(60, 8, totp::Algorithm::SHA256);
    std::cout << "高安全模式: " << totp::generate(secret, secure) << "\n";

    // 6位码 + SHA1 + 15秒周期（快速刷新场景）
    totp::Config fast(15, 6, totp::Algorithm::SHA1);
    std::cout << "快速刷新:   " << totp::generate(secret, fast) << "\n";

    return 0;
}