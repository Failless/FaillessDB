#include "boost_logger.h"

namespace failless::common::utils {

void BoostLogger::filter_logging(boost::log::trivial::severity_level lvl) {
    ::boost::log::core::get()->set_filter
        (
            boost::log::trivial::severity >= lvl
        );
}

}  // namespace failless::common::utils
