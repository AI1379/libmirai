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
    int get() const { return x_; }
  };
  Emitter emitter;
  Handler handler(13);
  mirai::Slot slot = emitter.add.connect(&handler, &Handler::add);
  emitter.add(79);
  ASSERT_EQ(handler.get(), 92);
}

TEST(MiraiUtilTest, SignalSlotTest3) {
  class EmitterA {
   public:
    mirai::Signal<void(void)> addA;
  };
  class EmitterB {
   public:
    mirai::Signal<void(void)> addB;
    mirai::Signal<void(void)> addC;
  };
  class Handler {
   public:
    int counter = 0;
    void handle() {
      counter++;
    }
    mirai::SlotContainer slots;
  };
  EmitterA emitter_a;
  EmitterB emitter_b;
  Handler handler;
  handler.slots.connect(emitter_a.addA, &handler, &Handler::handle);
  handler.slots.connect(emitter_b.addB, &handler, &Handler::handle);
  handler.slots.connect(emitter_b.addC, &handler, &Handler::handle);
  emitter_a.addA();
  ASSERT_EQ(handler.counter, 1);
  emitter_b.addB();
  ASSERT_EQ(handler.counter, 2);
  emitter_b.addC();
  ASSERT_EQ(handler.counter, 3);
}
