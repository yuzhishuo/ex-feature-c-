#include <gtest/gtest.h>

#include <future>
#include <thread>

TEST(promise, test) {
  std::promise<int> p;
  std::future<int> f = p.get_future();
  std::thread([&p]() { p.set_value(2); }).detach();
  EXPECT_EQ(2, f.get());
}

TEST(promise, test2) {
  std::promise<int> p;
  std::future<int> f = p.get_future();
  std::thread([&p]() { p.set_value(2); }).detach();
  EXPECT_EQ(2, f.get());
}

TEST(promise, test3) {
  std::promise<int> p;
  std::future<int> f = p.get_future();
  auto t = std::async(std::launch::async, [&p]() { p.set_value(2); });
  EXPECT_EQ(2, f.get());
}

TEST(promise, test4) {
  std::promise<int> p;
  std::future<int> f = p.get_future();
  auto t = std::async(std::launch::async, [&p]() {
    try {
      throw std::runtime_error("test");
    } catch (...) {
      p.set_exception(std::current_exception());
    }
  });
  EXPECT_THROW(f.get(), std::runtime_error);
}