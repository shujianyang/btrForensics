//! \file
//! \author Shujian Yang
//!
//! Define exceptions.

#ifndef EXCPT_H
#define EXCPT_H

#include <stdexcept>
#include <string>

namespace btrForensics {
    class FsDamagedException : public std::runtime_error {
    public:
        explicit FsDamagedException(const std::string& str)
                    :std::runtime_error(str) {}
    };
}

#endif
