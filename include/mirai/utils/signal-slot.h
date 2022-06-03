#ifndef MIRAI_INCLUDE_MIRAI_UTILS_SIGNAL_SLOT_H_
#define MIRAI_INCLUDE_MIRAI_UTILS_SIGNAL_SLOT_H_

#include <memory>
#include <functional>
#include <vector>

namespace mirai {

// Origin: https://github.com/sphair/ClanLib/tree/master/Sources/API/Core/Signals

// 处理成员函数
template<typename T, typename ResultType, class ...ArgsType>
std::function<ResultType(ArgsType...)> bindMember(T *instance, ResultType(T::*func)(ArgsType...)) {
  return [=](ArgsType &&...args) -> ResultType {
    return (instance->*func)(std::forward<ArgsType>(args)...);
  };
}

class BaseSlotImpl;

/**
 * @brief 槽
 * 接受信号的槽类型 封装了SlotImpl
 */
class Slot {
 private:
  std::shared_ptr<BaseSlotImpl> impl_;
 public:
  Slot() = default;
  operator bool() const { return static_cast<bool>(impl_); }
  template<typename T>
  explicit Slot(T impl):impl_(impl) {}
};

class BaseSlotImpl {
 public:
  BaseSlotImpl() = default;
  BaseSlotImpl(const BaseSlotImpl &) = delete;
  BaseSlotImpl &operator=(const BaseSlotImpl &) = delete;
  virtual ~BaseSlotImpl() = default;
};

template<typename SlotImplType>
class SignalImpl {
  template<typename Signature> friend
  class SlotImpl;
  template<typename Signature> friend
  class Signal;
 private:
  std::vector<std::weak_ptr<SlotImplType>> slots_;
};

template<typename Signature>
class SlotImpl : public BaseSlotImpl {
  template<typename FriendSignature> friend
  class Signal;
 private:
  std::weak_ptr<SignalImpl<SlotImpl>> signal_;
  std::function<Signature> callback_;
 public:
  SlotImpl(const std::weak_ptr<SignalImpl<SlotImpl>> &signal,
           const std::function<Signature> callback) : signal_(signal), callback_(callback) {}
  ~SlotImpl() {
    std::shared_ptr<SignalImpl<SlotImpl>> signal_ptr = signal_.lock();
    if (signal_ptr) {
      for (auto it = signal_ptr->slots_.begin(); it != signal_ptr->slots_.end(); it++) {
        if (it->expired() || it->lock().get() == this) {
          it = signal_ptr->slots_.erase(it);
          if (it == signal_ptr->slots_.end()) break;
        }
      }
    }
  }
};

/**
 * @brief 信号类
 * @tparam Signature 信号函数的函数签名
 */
template<typename Signature>
class Signal {
 public:
  using SlotType = SlotImpl<Signature>;
  using CallbackType = std::function<Signature>;
 private:
  std::shared_ptr<SignalImpl<SlotType>> impl_;
 public:
  Signal() : impl_(std::make_shared<SignalImpl<SlotType>>()) {}

  template<typename ...Args>
  void emit(Args &&... args) {
    std::vector<std::weak_ptr<SlotType>> slots = impl_->slots_;
    for (auto &weak_slot : slots) {
      auto slot = weak_slot.lock();
      if (slot) {
        slot->callback_(std::forward<Args>(args)...);
      }
    }
  }

  template<typename ...Args>
  void operator()(Args &&... args) { emit(args...); }

  /// 直接连接回调
  Slot connect(const CallbackType &func) {
    auto slot_impl = std::make_shared<SlotType>(impl_, func);
    impl_->slots_.push_back(slot_impl);
    return Slot(slot_impl);
  }

  /// 连接另一个类里面的槽函数
  template<typename InstanceType, typename MemberFuncType>
  Slot connect(InstanceType *instance, MemberFuncType func) {
    return connect(bindMember(instance, func));
  }
};

/**
 * @brief 槽的容器
 */
class SlotContainer {
 private:
  std::vector<Slot> slots_;
 public:
  template<typename Signature, typename InstanceType, typename MemberFuncType>
  void connect(Signal<Signature> &signal, InstanceType instance, MemberFuncType func) {
    slots_.push_back(signal.connect(instance, func));
  }

  template<typename Signature, typename CallbackType>
  void connect(Signal<Signature> &signal, CallbackType func) {
    slots_.push_back(signal.connect(func));
  }
};
}

#endif //MIRAI_INCLUDE_MIRAI_UTILS_SIGNAL_SLOT_H_
