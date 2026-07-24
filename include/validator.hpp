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
 * @file    validator.hpp
 * @brief   TOTP 时间窗口容错验证器
 * @dep     core.hpp, crypto.hpp, generator.hpp
 */
#ifndef TOTPSDK_VALIDATOR_HPP_
#define TOTPSDK_VALIDATOR_HPP_

#include "generator.hpp"
#include <chrono>

namespace totp {

/**
 * @brief  验证 TOTP 码(带时间窗口容错)
 * @param  secret  Base32 密钥
 * @param  code    用户输入的验证码
 * @param  window  允许偏移周期数(默认±1)
 * @param  cfg     配置
 * @return true=验证通过
 */
inline bool verify(const std::string& secret, const std::string& code,
                   int window=1, const Config& cfg={}) {
    auto epoch=std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    int64_t cur=static_cast<int64_t>(epoch)/cfg.period;
    for(int i=-window;i<=window;++i) {
        if(hotp(secret, static_cast<uint64_t>(cur+i), cfg)==code) return true;
    }
    return false;
}

} // namespace totp
#endif // TOTPSDK_VALIDATOR_HPP_