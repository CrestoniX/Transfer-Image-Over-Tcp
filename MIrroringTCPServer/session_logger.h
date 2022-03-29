#ifndef SESSION_LOGGER_H
#define SESSION_LOGGER_H
#include "logging_queue.h"

class session_logger
{
    void make_record(const LoggerInfo&);
public:
    session_logger();
    void start_logger();
};

#endif // SESSION_LOGGER_H
