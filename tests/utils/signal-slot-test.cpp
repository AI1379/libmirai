#include <gtest/gtest.h>

#include "mirai/utils/signal-slot.h"

#include <iostream>

TEST(MiraiUtilTest, SignalSlotTest1) {
  class Emitter {
   public:
    mirai::Signal<void(void)> greet;
  };
  Emitter emitter;
  std::string res;
  mirai::Slot slot = emitter.greet.connect([&res]() {
    res = "We were both young when I first saw you.";
  });
  emitter.greet();
  ASSERT_EQ(res, "We were both young when I first saw you.");
}

TEST(MiraiUtilTest, SignalSlotTest2) {
  class Emitter {
   public:
    mirai::Signal<void(int)> add;
  };
  class Handler {
   private:
    int x_;
   public:
    explicit Handler(int x) : x_(x) {}
    void add(int a) {
      x_ += a;
    }
    int get() { return x_; }
  };
  Emitter emitter;
  Handler handler(13);
  mirai::Slot slot = emitter.add.connect(&handler, &Handler::add);
  emitter.add(79);
  ASSERT_EQ(handler.get(), 92);
}
