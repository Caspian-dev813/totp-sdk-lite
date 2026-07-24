# totp-sdk-lite
These header files supply function prototypes, crypto algorithm interfaces and data structures for generating and verifying TOTP one-time passwords per RFC 6238.

### 🛡️ TOTP-CPP: Header-Only TOTP/HOTP Library

A lightweight, self-contained, header-only C++ library for generating and verifying Time-based One-Time Passwords (TOTP) and HMAC-based One-Time Passwords (HOTP). Fully compliant with RFC 6238 (TOTP) and RFC 4226 (HOTP).

#### ✨ Features

- **Header-Only:** Zero compilation, zero external dependencies. Just copy and include.
- **Self-Contained Crypto:** Built-in SHA-1, SHA-256, HMAC, and Base32 implementations. No OpenSSL required.
- **TOTP & HOTP:** Supports both time-based and counter-based OTP generation.
- **Configurable:** Customizable digit count (6/8), time period, and hash algorithm (SHA-1 / SHA-256).
- **Time Window Verification:** Built-in tolerance for clock skew during validation.
- **C++11 Compatible:** Works with any C++11 or later compiler.
- **Apache-2.0 Licensed:** Safe for commercial and closed-source projects.

#### 📥 Installation

Simply copy the `include/totp/` directory into your project's include path. No build step is required.

```bash
# Or add as a git submodule
git submodule add https://github.com/Caspian-dev813/totp-cpp.git vendor/totp-cpp
```

#### 🚀 Usage Examples

**1. Basic TOTP Generation**
Generate a standard 6-digit TOTP code using the default configuration (30s period, SHA-1).

```cpp
#include <totp/generator.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP"; // Base32-encoded secret

    std::cout << "Current TOTP: " << totp::generate(secret) << "\n";
    std::cout << "Seconds remaining: " << totp::remainingSeconds() << "s\n";
    
    return 0;
}
```

**2. Verification with Time Window**
Always use a time window in production to tolerate minor clock skew between the server and the user's device. A window of `1` checks the previous, current, and next time steps (±30 seconds).

```cpp
#include <totp/validator.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP";
    std::string userInput = "123456";

    bool valid = totp::verify(secret, userInput, /*window=*/1);

    std::cout << "Result: " << (valid ? "✅ Valid" : "❌ Invalid") << "\n";
    return 0;
}
```

**3. Custom Configuration**
Override the default time step, digit count, or hash algorithm by passing a `totp::Config` object.

```cpp
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
    
    return 0;
}
```

**4. Key Management (Base32 & Random Keys)**
Generate raw cryptographic keys and encode them to Base32 for safe storage, user display, or QR code generation.

```cpp
#include <totp/crypto.hpp>
#include <totp/generator.hpp>
#include <random>
#include <iostream>

int main() {
    // Generate a cryptographically random 20-byte key
    std::mt19937 rng(std::random_device{}());
    std::string rawKey(20, '\0');
    for (auto& b : rawKey) b = static_cast<char>(rng() % 256);

    // Encode to Base32
    std::string b32 = totp::crypto::base32Encode(rawKey);
    std::cout << "Base32 Secret: " << b32 << "\n";
    std::cout << "TOTP Code:     " << totp::generate(b32) << "\n";

    // Decode back and verify round-trip consistency
    std::string decoded = totp::crypto::base32Decode(b32);
    std::cout << "Round-trip OK: " << (decoded == rawKey ? "✅" : "❌") << "\n";
    
    return 0;
}
```

**5. HOTP (Counter-Based)**
Use HOTP for hardware tokens, offline devices, or event-driven scenarios where time synchronization is not possible.

```cpp
#include <totp/generator.hpp>
#include <iostream>

int main() {
    const std::string secret = "JBSWY3DPEHPK3PXP";

    for (uint64_t i = 0; i <= 9; ++i) {
        std::cout << "Counter=" << i
                  << "  HOTP=" << totp::hotp(secret, i) << "\n";
    }
    
    return 0;
}
```

#### 📚 API Reference

**Core Functions**

| Function | Header | Description |
| ------ |------ |------ |
| `totp::generate(secret, cfg?)` | `generator.hpp` | Generate current TOTP code |
| `totp::hotp(secret, counter, cfg?)` | `generator.hpp` | Generate HOTP code for a specific counter |
| `totp::remainingSeconds(period?)` | `generator.hpp` | Seconds until current TOTP expires |
| `totp::verify(secret, code, window?, cfg?)` | `validator.hpp` | Verify TOTP with time-window tolerance |
| `totp::crypto::base32Encode(data)` | `crypto.hpp` | Encode raw bytes to Base32 string |
| `totp::crypto::base32Decode(str)` | `crypto.hpp` | Decode Base32 string to raw bytes |
| `totp::crypto::hmac(algo, key, msg)` | `crypto.hpp` | Compute HMAC with selectable algorithm |

**Configuration Struct**

```cpp
struct Config {
    int       period    = 30;              // Time step in seconds (ignored for HOTP)
    int       digits    = 6;               // Code length: 6 or 8
    Algorithm algorithm = Algorithm::SHA1; // SHA1, SHA256, or SHA512
};
```

**Modular Includes**

To minimize compile-time overhead, include only the specific headers you need instead of the umbrella header:

| Need | Include |
| ------ |------ |
| Everything | `<totp/totp.hpp>` |
| Only generation | `<totp/generator.hpp>` |
| Only verification | `<totp/validator.hpp>` |
| Only crypto primitives | `<totp/crypto.hpp>` |
| Only type definitions | `<totp/core.hpp>` |

#### 🛠️ Building Examples

Compile the provided example files using any C++11 compatible compiler:

```bash
g++ -std=c++11 -I./include -o basic_generate examples/01_basic_generate.cpp
g++ -std=c++11 -I./include -o verify_window  examples/02_verify_window.cpp
g++ -std=c++11 -I./include -o custom_config  examples/03_custom_config.cpp
g++ -std=c++11 -I./include -o key_management  examples/04_key_management.cpp
g++ -std=c++11 -I./include -o hotp_demo      examples/05_hotp_demo.cpp
```

