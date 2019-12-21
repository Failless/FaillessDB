#include "boost_logger.h"

#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace failless::common::utils {

void BoostLogger::filter_logging(boost::log::trivial::severity_level lvl) {
    /// Limiting severities
    ::boost::log::core::get()->set_filter
        (
            boost::log::trivial::severity >= lvl
        );
    /// Changing output format
    ::boost::log::add_common_attributes();
    ::boost::log::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%]: <%Severity%>\t %Message%");
}

}  // namespace failless::common::utils
