#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <boost/noncopyable.hpp>
#include <deque>
#include <boost/thread.hpp>

template<typename T>
class BlockingQueue : private boost::noncopyable
{
public:
    BlockingQueue(){};

    void put(const T& x)
    {
        boost::mutex::scoped_lock _Lock(mutex_);
        queue_.push_back(x);
        notEmpty_.notify_all();
    }

    T take()
    {
        boost::mutex::scoped_lock _Lock(mutex_);
        while(queue_.empty())
        {
            notEmpty_.wait(_Lock);
        }

        T _Tp(queue_.front());
        queue_.pop_front();
        return _Tp;
    }


private:
    std::deque<T> queue_;
    boost::mutex mutex_;
    boost::condition_variable notEmpty_;
};

#endif // BLOCKINGQUEUE_H
