#pragma once

template <typename T>
class SharedPtr {
private:
	T* ptr;
	int* count;

public:
	// Constructor
	SharedPtr() noexcept : ptr(nullptr), count(nullptr) {}

	explicit SharedPtr(T* p = nullptr) : ptr(p), count(p ? new int(1) : nullptr) {}

	// Destructor
	~SharedPtr() {
		reset();
	}

	// Copy
	SharedPtr(const SharedPtr& other) : ptr(other.ptr), count(other.count) {
		if (count != nullptr) ++(*count);
	}

	SharedPtr& operator=(const SharedPtr& other) {
		if (this != &other) {
			reset();
			ptr = other.ptr;
			count = other.count;
			if (count != nullptr) ++(*count);
		}
		return *this;
	}

	// Move Semantics
	SharedPtr(SharedPtr&& other) noexcept : ptr(std::exchange(other.ptr, nullptr)), count(std::exchange(other.count, nullptr)) {}

	SharedPtr& operator=(SharedPtr&& other) noexcept {
		if (this != &other) {
			reset();
			ptr = std::exchange(other.ptr, nullptr);
			count = std::exchange(other.count, nullptr);
		}
		return *this;
	}

	// Access
	T* get() const noexcept { return ptr; }
	T* operator->() const { return ptr; }
	T& operator*() const { return *ptr; }

	// Functions
	void reset(T* other = nullptr) noexcept {
		if (count != nullptr && --(*count) == 0) {
			delete ptr;
			delete count;
		}

		ptr = other;
		count = (other != nullptr) ? new int(1) : nullptr;
	}

	int use_count() const noexcept {
		return (count != nullptr) ? *count : 0;
	}

	bool unique() const noexcept {
		return count != nullptr && *count == 1;
	}

	operator bool() const noexcept {
		return ptr != nullptr;
	}
};