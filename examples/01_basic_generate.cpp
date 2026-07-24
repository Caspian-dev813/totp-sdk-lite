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
 * @example 01_basic_generate.cpp
 * @brief   演示: 最基本的 TOTP 验证码生成
 * @build   g++ -std=c++11 -I../include -o 01_basic_generate 01_basic_generate.cpp
 */
#include <totp/generator.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP";

    std::cout << "=== 基础 TOTP 生成 ===\n";
    std::cout << "当前验证码: " << totp::generate(secret) << "\n";
    std::cout << "剩余有效秒: " << totp::remainingSeconds() << "s\n";

    return 0;
}