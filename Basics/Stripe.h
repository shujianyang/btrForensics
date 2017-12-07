//! \file
//! \author Shujian Yang
//!
//! Header file of Stripe.

#ifndef STRIPE
#define STRIPE

#include "Utility/Uuid.h"

namespace btrForensics {
    //! Stripe information stored in chunk item.
    class Stripe {
    public:
        uint64_t deviceId; //0x0
        uint64_t offset;

        UUID devUUID; //0x10

        Stripe(TSK_ENDIAN_ENUM endian, uint8_t arr[]);
        ~Stripe() = default; //!< Destructor

        std::string dataInfo() const;

        static const int SIZE_OF_STRIPE = 0X20; //!< Size of stripe in bytes.
    };
}

#endif

