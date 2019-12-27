#include "boost_logger.h"

#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace failless::common::utils {

void BoostLogger::filter_logging(boost::log::trivial::severity_level lvl) {
    /// Limiting severities
    ::boost::log::core::get()->set_filter
        (
            boost::log::trivial::severity >= lvl
        );
    /// Changing output format
    ::boost::log::add_common_attributes();
}

void BoostLogger::set_log_path(std::string log_path = "/tmp/failless/log") {
    ::boost::log::keywords::auto_flush = true;

    ::boost::filesystem::create_directory(log_path);
    std::string log_file = log_path + "/" + to_simple_string(::boost::gregorian::day_clock::local_day());

    if ( !::boost::filesystem::exists(log_file) ) {
        ::boost::filesystem::ofstream file(log_file);
        file << "Log of Failless DB\nDATE: " << ::boost::gregorian::day_clock::local_day() << "\n" << std::endl;
        file.close();
    }

    ::boost::log::add_file_log(
            log_file,
            ::boost::log::keywords::format = "[%TimeStamp%]: <%Severity%>\t %Message%",
            ::boost::log::keywords::auto_flush = true,
            ::boost::log::keywords::open_mode = std::ios_base::app);
    ::boost::log::add_console_log(std::cout, ::boost::log::keywords::format = "[%TimeStamp%]: <%Severity%>\t %Message%");
}

}  // namespace failless::common::utils
