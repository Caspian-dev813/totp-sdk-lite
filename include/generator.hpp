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
 * @file    generator.hpp
 * @brief   TOTP / HOTP 验证码生成器
 * @dep     core.hpp, crypto.hpp
 */
#ifndef TOTPSDK_GENERATOR_HPP_
#define TOTPSDK_GENERATOR_HPP_

#include "core.hpp"
#include "crypto.hpp"
#include <chrono>
#include <cmath>

namespace totp {

/// 生成指定计数器的 HOTP 码 (RFC 4226)
inline std::string hotp(const std::string& secret, uint64_t counter, const Config& cfg = {}) {
    std::string key = crypto::base32Decode(secret);
    char msg[8];
    for(int i=7;i>=0;--i){msg[i]=static_cast<char>(counter&0xFF);counter>>=8;}
    std::string h = crypto::hmac(cfg.algorithm, key, std::string(msg,8));
    if(h.size()<20) return std::string(cfg.digits,'0');
    int off=h.back()&0x0F;
    uint32_t bin=((h[off]&0x7F)<<24)|((h[off+1]&0xFF)<<16)|((h[off+2]&0xFF)<<8)|(h[off+3]&0xFF);
    uint32_t otp=bin%static_cast<uint32_t>(std::pow(10,cfg.digits));
    std::string code=std::to_string(otp);
    while(static_cast<int>(code.size())<cfg.digits) code="0"+code;
    return code;
}

/// 生成当前时间的 TOTP 码 (RFC 6238)
inline std::string generate(const std::string& secret, const Config& cfg = {}) {
    auto epoch=std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return hotp(secret, static_cast<uint64_t>(epoch)/cfg.period, cfg);
}

/// 获取当前码剩余有效秒数
inline int remainingSeconds(int period=30) {
    auto epoch=std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return period - static_cast<int>(epoch % period);
}

} // namespace totp
#endif // TOTPSDK_GENERATOR_HPP_