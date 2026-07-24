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
 * @example 05_hotp_counter.cpp
 * @brief   演示: 基于计数器的事件驱动 HOTP（非时间依赖）
 * @build   g++ -std=c++11 -I../include -o 05_hotp_counter 05_hotp_counter.cpp
 */
#include <totp/generator.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP";

    std::cout << "=== HOTP 计数器模式 ===\n";
    std::cout << "适用于: 硬件令牌、离线设备、事件触发场景\n\n";

    // RFC 4226 附录D 测试向量验证
    for(uint64_t i = 0; i <= 9; ++i) {
        std::cout << "Counter=" << i
                  << "  HOTP=" << totp::hotp(secret, i) << "\n";
    }

    return 0;
}