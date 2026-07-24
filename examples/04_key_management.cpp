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
 * @example 04_key_management.cpp
 * @brief   演示: Base32 密钥编码/解码与密钥生成
 * @build   g++ -std=c++11 -I../include -o 04_key_management 04_key_management.cpp
 */
#include <totp/crypto.hpp>
#include <totp/generator.hpp>
#include <iostream>
#include <random>

int main() {
    std::cout << "=== 密钥管理 ===\n";

    // 生成20字节随机密钥
    std::mt19937 rng(std::random_device{}());
    std::string rawKey(20, '\0');
    for(auto& b : rawKey) b = static_cast<char>(rng() % 256);

    // 编码为 Base32 供用户扫码/手动输入
    std::string b32 = totp::crypto::base32Encode(rawKey);
    std::cout << "原始密钥(Base32): " << b32 << "\n";

    // 用该密钥生成验证码验证可用性
    std::cout << "对应TOTP验证码:   " << totp::generate(b32) << "\n";

    // 反向解码验证一致性
    std::string decoded = totp::crypto::base32Decode(b32);
    std::cout << "解码一致:         " << (decoded == rawKey ? "✅" : "❌") << "\n";

    return 0;
}