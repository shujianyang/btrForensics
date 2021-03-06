//! \file
//! \author Shujian Yang
//!
//! Define exceptions.

#ifndef EXCPT_H
#define EXCPT_H

#include <stdexcept>
#include <string>

namespace btrForensics {

    //! Exception thrown when read data does no match Btrfs format.
    class FsDamagedException : public std::runtime_error {
    public:
        explicit FsDamagedException(const std::string& str)
                    :std::runtime_error(str) {}
    };


    //! Exception thrown when device information not matched.
    class FsDeviceException : public std::runtime_error {
    public:
        explicit FsDeviceException(const std::string& str)
                    :std::runtime_error(str) {}
    };
}

#endif
