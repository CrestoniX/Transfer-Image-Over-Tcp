#include "logging_queue.h"

//void logging_queue::create_instance()
//{
//    if()
//}

std::shared_ptr<logging_queue> logging_queue::GetInstance()
{
//    std::call_once(flag, create_instance);
    static std::shared_ptr<logging_queue> instance(new logging_queue);
    return instance;
}

void logging_queue::push_info(const LoggerInfo &info)
{
    std::unique_lock locked(mu);
    queue_of_logs.push(info);
    locked.unlock();
    data_ready.notify_one();
}

LoggerInfo logging_queue::get_info_to_log()
{
    std::unique_lock locked(mu);
    data_ready.wait(locked, [this](){ return !queue_of_logs.empty(); });
    auto info_to_log = queue_of_logs.front();
    queue_of_logs.pop();
    locked.unlock();
    return info_to_log;
}
