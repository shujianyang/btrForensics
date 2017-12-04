/**
 * \file
 * \author Shujian Yang
 *
 * Header file of string process.
 */

#ifndef STR_PRO_H
#define STR_PRO_H

#include <string>
#include <vector>
#include <tsk/libtsk.h>

namespace btrForensics {
    std::vector<std::string> strSplit(std::string, std::string);

    std::string strStrip(std::string);
}

#endif
