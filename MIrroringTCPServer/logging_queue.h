#ifndef LOGGING_QUEUE_H
#define LOGGING_QUEUE_H
#include <queue>
#include "LoggerInfo.h"
#include <mutex>
#include <condition_variable>



class logging_queue
{
    logging_queue() {};
    logging_queue& operator=(const logging_queue&) = delete;
    logging_queue&& operator=(logging_queue&&) = delete;
    logging_queue(const logging_queue&) = delete;
    logging_queue(logging_queue&&) = delete;
    std::queue<LoggerInfo> queue_of_logs;
    std::mutex mu;
    std::condition_variable data_ready;
//    static std::once_flag flag;
//    static std::shared_ptr<logging_queue> instance;
//    static void create_instance();

public:
    static std::shared_ptr<logging_queue> GetInstance();
    void push_info(const LoggerInfo&);
    LoggerInfo get_info_to_log();
};



#endif // LOGGING_QUEUE_H
