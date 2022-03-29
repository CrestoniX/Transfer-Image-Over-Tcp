#include "singleton_queue.h"

singleton_queue::singleton_queue()
{

}

std::shared_ptr<logging_queue> singleton_queue::return_queue()
{
    std::call_once(queue_creation_flag, &singleton_queue::create_queue,
                       this);
    return impl;
}

void singleton_queue::create_queue()
{
    if(!impl)
    {
        impl = std::make_shared<logging_queue>(new logging_queue);
    }
}

std::shared_ptr<logging_queue> singleton_queue::GetInstance()
{
   return create_queue();
}
