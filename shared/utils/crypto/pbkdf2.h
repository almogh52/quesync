#pragma once

#include <string>

#define PBKDF2_SALT_SIZE (128 / 8)
#define PBKDF2_ITERATIONS 15000
#define PBKDF2_HASH_SIZE (512 / 8)

namespace quesync {
namespace utils {
namespace crypto {
class pbkdf2 {
   public:
    /**
     * Calculates PBKDF2-SHA512 for a password.
     *
     * @param password The raw password.
     * @param salt The salt to be using for the hashing.
     * @return The hashed password.
     */
    static std::string sha512(std::string password, unsigned char *salt);

    /**
     * Validates a password using PBKDF2-SHA512.
     *
     * @param password The raw password.
     * @param hash The hashed password.
     * @return True if the passwords match or false otherwise.
     */
    static bool sha512_compare(std::string password, std::string hash);
};
};  // namespace crypto
};  // namespace utils
};  // namespace quesync