/**
 * @file    core.hpp
 * @brief   TOTP SDK 核心类型定义、枚举与配置结构体
 * @note    本文件仅包含类型定义，无任何实现代码，可安全被任意模块包含
 */
#ifndef TOTPSDK_CORE_HPP_
#define TOTPSDK_CORE_HPP_

#include <cstdint>
#include <string>
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

namespace totp {

/// HMAC 哈希算法选择器
enum class Algorithm {
    SHA1,       ///< RFC 6238 默认, 兼容性最佳
    SHA256,     ///< 更高安全性, 现代验证器支持
    SHA512      ///< 最高安全等级
};

/// TOTP/HOTP 参数配置
struct Config {
    int       period    = 30;           ///< 时间步长(秒), HOTP时忽略
    int       digits    = 6;            ///< 验证码位数 [6,8]
    Algorithm algorithm = Algorithm::SHA1;

    constexpr Config() noexcept = default;
    constexpr Config(int p, int d, Algorithm a = Algorithm::SHA1) noexcept
        : period(p), digits(d), algorithm(a) {}
};

} // namespace totp
#endif // TOTPSDK_CORE_HPP_