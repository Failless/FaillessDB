#ifndef FAILLESS_LLSS3P_UTILS_BOOST_LOGGER_H
#define FAILLESS_LLSS3P_UTILS_BOOST_LOGGER_H


#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

namespace failless::common::utils {

namespace BoostLogger {
    void filter_logging(boost::log::trivial::severity_level lvl);
};

}  // namespace failless::common::utils


#endif //FAILLESS_LLSS3P_UTILS_BOOST_LOGGER_H
