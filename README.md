# totp-sdk-lite
These header files supply function prototypes, crypto algorithm interfaces and data structures for generating and verifying TOTP one-time passwords per RFC 6238.

Here is a complete English README tailored to your TOTP Header-Only SDK, based on the actual API and examples in your source files.
TOTP-CPP: Header-Only TOTP/HOTP Library
A lightweight, self-contained, header-only C++ library for generating and verifying Time-based One-Time Passwords (TOTP) and HMAC-based One-Time Passwords (HOTP). Fully compliant with RFC 6238 (TOTP) and RFC 4226 (HOTP).
✨ Features
Header-Only: Zero compilation, zero external dependencies. Just copy and include.
Self-Contained Crypto: Built-in SHA-1, SHA-256, HMAC, and Base32 implementations. No OpenSSL required.
TOTP & HOTP: Supports both time-based and counter-based OTP generation.
Configurable: Customizable digit count (6/8), time period, and hash algorithm (SHA-1 / SHA-256).
Time Window Verification: Built-in tolerance for clock skew during validation.
C++11 Compatible: Works with any C++11 or later compiler.
Apache-2.0 Licensed: Safe for commercial and closed-source projects.
📦 Installation
Simply copy the include/totp/ directory into your project's include path. No build step required.
bash

编辑



# Or add as a git submodule
git submodule add https://github.com/Caspian-dev813/totp-cpp.git vendor/totp-cpp
🚀 Quick Start
Basic TOTP Generation
cpp

编辑



#include <totp/generator.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP"; // Base32-encoded secret

    std::cout << "Current TOTP: " << totp::generate(secret) << "\n";
    std::cout << "Seconds remaining: " << totp::remainingSeconds() << "s\n";
}
Verification with Time Window (Recommended for Production)
cpp

编辑



#include <totp/validator.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP";
    std::string userInput = "123456";

    // ±1 window = tolerates ±30 seconds of clock skew
    bool valid = totp::verify(secret, userInput, /*window=*/1);

    std::cout << "Result: " << (valid ? "✅ Valid" : "❌ Invalid") << "\n";
}
Custom Configuration (Digits, Period, Algorithm)
cpp

编辑



#include <totp/totp.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP";

    // 8-digit code, SHA-256, 60-second period
    totp::Config secure(60, 8, totp::Algorithm::SHA256);
    std::cout << "Secure mode: " << totp::generate(secret, secure) << "\n";

    // 6-digit code, SHA-1, 15-second period (fast refresh)
    totp::Config fast(15, 6, totp::Algorithm::SHA1);
    std::cout << "Fast mode:   " << totp::generate(secret, fast) << "\n";
}
HOTP (Counter-Based, Non-Time-Dependent)
Ideal for hardware tokens, offline devices, and event-driven scenarios.
cpp

编辑



#include <totp/generator.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP";

    for (uint64_t i = 0; i <= 9; ++i) {
        std::cout << "Counter=" << i
                  << "  HOTP=" << totp::hotp(secret, i) << "\n";
    }
}
Key Management (Base32 Encoding/Decoding & Random Key Generation)
cpp

编辑



#include <totp/crypto.hpp>
#include <totp/generator.hpp>
#include <random>
#include <iostream>

int main() {
    // Generate a cryptographically random 20-byte key
    std::mt19937 rng(std::random_device{}());
    std::string rawKey(20, '\0');
    for (auto& b : rawKey) b = static_cast<char>(rng() % 256);

    // Encode to Base32 for user display / QR code scanning
    std::string b32 = totp::crypto::base32Encode(rawKey);
    std::cout << "Base32 Secret: " << b32 << "\n";
    std::cout << "TOTP Code:     " << totp::generate(b32) << "\n";

    // Decode back and verify round-trip consistency
    std::string decoded = totp::crypto::base32Decode(b32);
    std::cout << "Round-trip OK: " << (decoded == rawKey ? "✅" : "❌") << "\n";
}
📖 API Reference
表格
Function	Header	Description
totp::generate(secret, cfg?)	generator.hpp	Generate current TOTP code
totp::hotp(secret, counter, cfg?)	generator.hpp	Generate HOTP code for a specific counter
totp::remainingSeconds(period?)	generator.hpp	Seconds until current TOTP expires
totp::verify(secret, code, window?, cfg?)	validator.hpp	Verify TOTP with time-window tolerance
totp::crypto::base32Encode(data)	crypto.hpp	Encode raw bytes to Base32 string
totp::crypto::base32Decode(str)	crypto.hpp	Decode Base32 string to raw bytes
totp::crypto::hmac(algo, key, msg)	crypto.hpp	Compute HMAC with selectable algorithm
totp::Config Struct
cpp

编辑



struct Config {
    int       period    = 30;              // Time step in seconds (ignored for HOTP)
    int       digits    = 6;               // Code length: 6 or 8
    Algorithm algorithm = Algorithm::SHA1; // SHA1, SHA256, or SHA512
};
Modular Includes
For minimal compile-time overhead, include only what you need:
表格
Need	Include
Everything	<totp/totp.hpp>
Only generation	<totp/generator.hpp>
Only verification	<totp/validator.hpp>
Only crypto primitives	<totp/crypto.hpp>
Only type definitions	<totp/core.hpp>
🔧 Building Examples
bash

编辑



g++ -std=c++11 -I./include -o basic_generate examples/01_basic_generate.cpp
g++ -std=c++11 -I./include -o verify_window  examples/02_verify_window.cpp
g++ -std=c++11 -I./include -o custom_config  examples/03_custom_config.cpp
g++ -std=c++11 -I./include -o key_management examples/04_key_management.cpp
g++ -std=c++11 -I./include -o hotp_counter   examples/05_hotp_counter.cpp
⚠️ Security Notes
Random Key Generation: The example uses std::mt19937 for demonstration. In production, use a CSPRNG such as /dev/urandom, BCryptGenRandom, or std::random_device with proper seeding.
Constant-Time Comparison: The built-in verify() function performs direct string comparison. For high-security applications, consider wrapping it with a constant-time comparison utility to prevent timing attacks.
SHA-512: Currently marked as TODO in crypto.hpp. SHA-1 and SHA-256 are fully implemented and tested.
📄 License
Copyright 2026 Caspian-dev813
Licensed under the Apache License, Version 2.0. See LICENSE for details.
