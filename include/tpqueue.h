// Copyright 2025 NNTU-CS
#ifndef INCLUDE_TPQUEUE_H_
#define INCLUDE_TPQUEUE_H_

#include <cstddef>
#include <utility>

//------------------------------------------------------------------------------
// A singly‑linked list based priority queue.
// Insertion:  O(n) – we scan for the right spot.
// Removal:    O(1) – we always pop from the head.
//------------------------------------------------------------------------------

template <typename T>
class TPQueue {
 public:
  TPQueue() : head_(nullptr) {}

  TPQueue(const TPQueue& other) : head_(nullptr) { CopyFrom(other); }

  TPQueue& operator=(const TPQueue& other) {
    if (this != &other) {
      Clear();
      CopyFrom(other);
    }
    return *this;
  }

  TPQueue(TPQueue&& other) noexcept : head_(other.head_) { other.head_ = nullptr; }

  TPQueue& operator=(TPQueue&& other) noexcept {
    if (this != &other) {
      Clear();
      head_ = other.head_;
      other.head_ = nullptr;
    }
    return *this;
  }

  ~TPQueue() { Clear(); }

  //------------------------------------------------------------------------------
  // Public interface
  //------------------------------------------------------------------------------
  bool empty() const { return head_ == nullptr; }

  void push(const T& value) {
    Node* new_node = new Node(value);

    // Insert at the head if queue is empty or priority is higher than head.
    if (empty() || value.prior > head_->data.prior) {
      new_node->next = head_;
      head_ = new_node;
      return;
    }

    // Walk while next element has priority not less than the new one.
    Node* prev = head_;
    while (prev->next != nullptr && prev->next->data.prior >= value.prior) {
      prev = prev->next;
    }

    // Insert after 'prev'.
    new_node->next = prev->next;
    prev->next = new_node;
  }

  // Removes and returns element with the highest priority.
  T pop() {
    if (empty()) {
      return T{};  // For the purpose of laboratory tests we return default value.
    }

    Node* to_delete = head_;
    head_ = head_->next;
    T result = to_delete->data;
    delete to_delete;
    return result;
  }

 private:
  struct Node {
    explicit Node(const T& d) : data(d), next(nullptr) {}
    T data;
    Node* next;
  };

  Node* head_;

  void Clear() {
    while (!empty()) {
      pop();
    }
  }

  void CopyFrom(const TPQueue& other) {
    if (other.empty()) return;

    head_ = new Node(other.head_->data);
    Node* dst_cur = head_;
    Node* src_cur = other.head_->next;

    while (src_cur != nullptr) {
      dst_cur->next = new Node(src_cur->data);
      dst_cur = dst_cur->next;
      src_cur = src_cur->next;
    }
  }
};

//------------------------------------------------------------------------------
// Example data structure used in laboratory tests.
//------------------------------------------------------------------------------
struct SYM {
  char ch;   // symbol itself
  int prior; // priority: 1 (low) … 10 (high)
};

#endif  // INCLUDE_TPQUEUE_H_
