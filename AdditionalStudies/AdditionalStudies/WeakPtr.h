#pragma once
#include "SharedPtr.h"

// ���� ����Ʈ �����ʹ� Shared Ptr�� Weak Ptr�� Control Block�� ����
// Control Block�� use count, weak count, raw data, destructor�� ������
// ��ü�� ���� �� ������ ����. ����Ʈ �����͵��� Control Block�� ����, ������ ��

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

	// Access => control block �Ǵ� shared ptr�� ���� ������ �ʿ�
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