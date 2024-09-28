#include "waiting_once_api.hpp"

void waiting_once::call_once_waiting(init_function f)
{
	// TODO: implement this
	 std::lock_guard<std::mutex> lock(mtx);
	 if (!initialized) {
	 	f();
	 	initialized = true;
	 }

//	if (!initialized.load(std::memory_order_acquire)) {
//        std::unique_lock<std::mutex> lock(mtx);
//        if (!initialized.load(std::memory_order_relaxed)) {
//            f();
//            initialized.store(true, std::memory_order_release);
//            cv.notify_all();
//        } else {
//            cv.wait(lock, [this]() { return initialized.load(std::memory_order_relaxed); });
//        }
//    }
}
