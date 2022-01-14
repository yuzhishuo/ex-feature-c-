#include <gtest/gtest.h>

#include <future>
#include <thread>

TEST(FutureTest, test) {
  std::future f = std::async(std::launch::async, []() { return 2; });
  EXPECT_EQ(2, f.get());
}

TEST(FutureTest, test2) {
  std::future f = std::async(std::launch::async, []() { return 2; });

  auto shared = f.share();
  EXPECT_FALSE(f.valid());
  EXPECT_TRUE(shared.valid());
  EXPECT_EQ(2, shared.get());
}

TEST(FutureTest, test3) {
  std::future f = std::async(std::launch::async, []() { return 2; });

  auto shared = f.share();  // than f will be invalid

  EXPECT_TRUE(shared.valid());
  EXPECT_EQ(2, shared.get());
  EXPECT_FALSE(f.valid());
}

TEST(FutureTest, shared_future_test2) {
  std::future f = std::async(std::launch::async, []() { return 2; });

  auto shared = f.share();

  EXPECT_THROW(f.get(), std::future_error);  // f invalid
}

TEST(FutureTest, shared_future_test) {
  std::future f = std::async(std::launch::async, []() { return 2; });

  auto shared = f.share();

  EXPECT_EQ(2, shared.get());  // shared_future could repeat get()
  EXPECT_EQ(2, shared.get());
}

TEST(FutureTest, shared_future_test3) {
  std::future f = std::async(std::launch::deferred, []() { return 2; });

  f.wait();

  EXPECT_EQ(f.get(), 2);
}

TEST(FutureTest, shared_future_test4) {
  constexpr int kNumThreads = 10'00;
  std::future f = std::async(std::launch::deferred, [=]() {
    std::this_thread::sleep_for(std::chrono::seconds(kNumThreads));
    return 2;
  });

  auto status = f.wait_for(std::chrono::seconds(kNumThreads - 500));

  EXPECT_EQ(status, std::future_status::deferred);
  //   EXPECT_EQ(status, std::future_status::timeout);
}

TEST(FutureTest, shared_future_test5) {
  constexpr int kNumThreads = 10;
  std::future f = std::async(std::launch::async, [=]() {
    std::this_thread::sleep_for(std::chrono::seconds(kNumThreads));
    return 2;
  });

  auto status = f.wait_for(std::chrono::seconds(kNumThreads - 5));

  EXPECT_EQ(status, std::future_status::timeout);
}