#pragma once
#include "SharedPtr.h"

// 원본 스마트 포인터는 Shared Ptr과 Weak Ptr이 Control Block을 공유
// Control Block은 use count, weak count, raw data, destructor를 가지며
// 객체의 관리 및 삭제를 전담. 스마트 포인터들은 Control Block을 참조, 관리만 함

template <typename T>
class WeakPtr {
private:
	T* ptr;
	int* count;

public:
	// Constructor
	WeakPtr() noexcept : ptr(nullptr) {}

	explicit WeakPtr(SharedPtr* sp) : ptr(sp.ptr), count(sp.count) {}

	// Destructor
	~WeakPtr() {}

	// Copy
	WeakPtr(const WeakPtr& other) : ptr(other.ptr), count(other.count) {}

	WeakPtr& operator=(const WeakPtr& other) {
		if (this != &other) {
			ptr = other.ptr;
			count = other.count;
		}
		return *this;
	}

	// Move Semantics
	WeakPtr(WeakPtr&& other) noexcept : ptr(std::exchange(other.ptr, nullptr)), count(std::exchange(other.count, nullptr)) {}

	WeakPtr& operator=(WeakPtr&& other) noexcept {
		if (this != &other) {
			ptr = std::exchange(other.ptr, nullptr);
			count = std::exchange(other.count, nullptr);
		}
		return *this;
	}

	// Access => control block 또는 shared ptr에 대한 참조가 필요
	//SharedPtr* lock() {
	//	
	//}

	// Functions
	void reset() noexcept {
		ptr = nullptr;
	}

	int use_count() noexcept {
		return count != nullptr ? *count : 0;
	}

	bool expired() noexcept {
		return ptr == nullptr;
	}

	operator bool() const noexcept {
		return ptr != nullptr;
	}

};