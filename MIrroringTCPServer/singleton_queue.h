#ifndef SINGLETON_QUEUE_H
#define SINGLETON_QUEUE_H
#include "logging_queue.h"
#include <memory>

class singleton_queue
{
    singleton_queue();
    singleton_queue& operator=(const singleton_queue&) = delete;
    singleton_queue(const singleton_queue&) = delete;
    singleton_queue&& operator=(singleton_queue&&) = delete;
    singleton_queue(singleton_queue&&) = delete;
    std::shared_ptr<logging_queue> impl;
    std::once_flag queue_creation_flag;
    std::shared_ptr<logging_queue> return_queue();
    void create_queue();

public:
    static std::shared_ptr<logging_queue> GetInstance();

};

#endif // SINGLETON_QUEUE_H
