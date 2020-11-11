
#include <condition_variable>
#include <thread> 

namespace laya {
    class semaphore {
        unsigned int count_;
        std::mutex mutex_;
        std::condition_variable condition_;
    public:
        //构造一个semaphore 初始值为initial.
        explicit semaphore(unsigned int initial) : count_(initial) {}

        //增加计数，并且通知其他的wait醒来检查。
        void signal() {
            {
                std::unique_lock<std::mutex> lock(mutex_);
                ++count_;
            }
            condition_.notify_one();
        }

        //等待，直到有人 signal，signal到count>0,wait成功会减count
        void wait() {
            std::unique_lock<std::mutex> lock(mutex_);
            while (count_ == 0) {
                condition_.wait(lock);
            }
            --count_;
        }

        //等待，如果没有人增加计数的的话在时间到后会醒来返回。成功会减count
        //这个不会要求有count
        void wait(int ms) {
            std::unique_lock<std::mutex> lock(mutex_);
            condition_.wait_until(lock, std::chrono::system_clock::now() + std::chrono::milliseconds(ms));
            --count_;
        }
    };
}