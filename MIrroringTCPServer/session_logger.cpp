#include "session_logger.h"
#include <iostream>

void session_logger::make_record(const LoggerInfo& info)
{
    std::cout << "logging some info" << std::endl;
}

session_logger::session_logger()
{

}

void session_logger::start_logger()
{
    for(;;)
    {
        auto instance = logging_queue::GetInstance();
        auto info_to_log = instance->get_info_to_log();
        make_record(info_to_log);
    }
}
