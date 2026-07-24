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
 * @file    crypto.hpp
 * @brief   自包含密码学原语: SHA-1, SHA-256, HMAC, Base32 编解码
 * @warning 内部实现细节，建议通过 generator/validator 间接使用
 * @dep     仅依赖 core.hpp
 */
#ifndef TOTPSDK_CRYPTO_HPP_
#define TOTPSDK_CRYPTO_HPP_

#include "core.hpp"
#include <array>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <type_traits>

namespace totp { namespace crypto {

// ======================== SHA-1 (FIPS 180-4) ========================
inline std::string sha1(const std::string& data) {
    uint32_t h0=0x67452301,h1=0xEFCDAB89,h2=0x98BADCFE,h3=0x10325476,h4=0xC3D2E1F0;
    uint64_t bits=data.size()*8;
    std::string m=data; m.push_back('\x80');
    while(m.size()%64!=56) m.push_back('\0');
    for(int i=7;i>=0;--i) m.push_back(static_cast<char>((bits>>(i*8))&0xFF));
    for(size_t c=0;c<m.size();c+=64){
        uint32_t w[80];
        for(int i=0;i<16;++i) w[i]=(uint32_t(m[c+i*4])<<24)|(uint32_t(m[c+i*4+1])<<16)|
                                    (uint32_t(m[c+i*4+2])<<8)|uint32_t(m[c+i*4+3]);
        for(int i=16;i<80;++i){uint32_t t=w[i-3]^w[i-8]^w[i-14]^w[i-16];w[i]=(t<<1)|(t>>31);}
        uint32_t a=h0,b=h1,c2=h2,d=h3,e=h4;
        for(int i=0;i<80;++i){
            uint32_t f,k;
            if(i<20){f=(b&c2)|(~b&d);k=0x5A827999;}else if(i<40){f=b^c2^d;k=0x6ED9EBA1;}
            else if(i<60){f=(b&c2)|(b&d)|(c2&d);k=0x8F1BBCDC;}else{f=b^c2^d;k=0xCA62C1D6;}
            uint32_t t=((a<<5)|(a>>27))+f+e+k+w[i];e=d;d=c2;c2=(b<<30)|(b>>2);b=a;a=t;
        }
        h0+=a;h1+=b;h2+=c2;h3+=d;h4+=e;
    }
    char r[20];
    auto s=[&](char*d,uint32_t v){d[0]=(v>>24)&0xFF;d[1]=(v>>16)&0xFF;d[2]=(v>>8)&0xFF;d[3]=v&0xFF;};
    s(r,h0);s(r+4,h1);s(r+8,h2);s(r+12,h3);s(r+16,h4);
    return std::string(r,20);
}

// ======================== SHA-256 (FIPS 180-4) ========================
inline std::string sha256(const std::string& data) {
    static constexpr uint32_t K[64]={
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
    uint32_t st[8]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    uint64_t bits=data.size()*8;
    std::string m=data;m.push_back('\x80');
    while(m.size()%64!=56)m.push_back('\0');
    for(int i=7;i>=0;--i)m.push_back(static_cast<char>((bits>>(i*8))&0xFF));
    for(size_t c=0;c<m.size();c+=64){
        uint32_t w[64];
        for(int i=0;i<16;++i)w[i]=(uint32_t(m[c+i*4])<<24)|(uint32_t(m[c+i*4+1])<<16)|
                                   (uint32_t(m[c+i*4+2])<<8)|uint32_t(m[c+i*4+3]);
        for(int i=16;i<64;++i){
            uint32_t s0=((w[i-15]>>7)|(w[i-15]<<25))^((w[i-15]>>18)|(w[i-15]<<14))^(w[i-15]>>3);
            uint32_t s1=((w[i-2]>>17)|(w[i-2]<<15))^((w[i-2]>>19)|(w[i-2]<<13))^(w[i-2]>>10);
            w[i]=w[i-16]+s0+w[i-7]+s1;
        }
        uint32_t a=st[0],b=st[1],c2=st[2],d=st[3],e=st[4],f=st[5],g=st[6],h=st[7];
        for(int i=0;i<64;++i){
            uint32_t S1=((e>>6)|(e<<26))^((e>>11)|(e<<21))^((e>>25)|(e<<7));
            uint32_t ch=(e&f)^(~e&g),t1=h+S1+ch+K[i]+w[i];
            uint32_t S0=((a>>2)|(a<<30))^((a>>13)|(a<<19))^((a>>22)|(a<<10));
            uint32_t maj=(a&b)^(a&c2)^(b&c2),t2=S0+maj;
            h=g;g=f;f=e;e=d+t1;d=c2;c2=b;b=a;a=t1+t2;
        }
        st[0]+=a;st[1]+=b;st[2]+=c2;st[3]+=d;st[4]+=e;st[5]+=f;st[6]+=g;st[7]+=h;
    }
    char r[32];
    for(int i=0;i<8;++i){r[i*4]=(st[i]>>24)&0xFF;r[i*4+1]=(st[i]>>16)&0xFF;
                          r[i*4+2]=(st[i]>>8)&0xFF;r[i*4+3]=st[i]&0xFF;}
    return std::string(r,32);
}

// ======================== 统一哈希调度 ========================
inline std::string hash(Algorithm algo, const std::string& data) {
    switch(algo){
        case Algorithm::SHA256: return sha256(data);
        case Algorithm::SHA512: /* TODO */ 
        case Algorithm::SHA1: default: return sha1(data);
    }
}

// ======================== HMAC (RFC 2104) ========================
inline std::string hmac(Algorithm algo, const std::string& key, const std::string& msg) {
    size_t bs = (algo==Algorithm::SHA512)?128:64;
    std::string k=key;
    if(k.size()>bs) k=hash(algo,k);
    k.resize(bs,'\0');
    std::string ipad(bs,'\0'),opad(bs,'\0');
    for(size_t i=0;i<bs;++i){ipad[i]=k[i]^0x36;opad[i]=k[i]^0x5C;}
    return hash(algo, opad + hash(algo, ipad + msg));
}

// ======================== Base32 编解码 (RFC 4648) ========================
inline std::string base32Decode(const std::string& in) {
    static const char A[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    std::string r; int buf=0,bits=0;
    for(char c:in){
        if(c=='='||c==' ')continue;
        const char*p=strchr(A,std::toupper(static_cast<unsigned char>(c)));
        if(!p)continue;
        buf=(buf<<5)|static_cast<int>(p-A);bits+=5;
        if(bits>=8){bits-=8;r.push_back(static_cast<char>((buf>>bits)&0xFF));}
    }
    return r;
}

inline std::string base32Encode(const std::string& in) {
    static const char A[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    std::string r; int buf=0,bits=0;
    for(unsigned char c:in){buf=(buf<<8)|c;bits+=8;
        while(bits>=5){bits-=5;r.push_back(A[(buf>>bits)&0x1F]);}}
    if(bits>0)r.push_back(A[(buf<<(5-bits))&0x1F]);
    while(r.size()%8)r.push_back('=');
    return r;
}

}} // namespace totp::crypto
#endif // TOTPSDK_CRYPTO_HPP_