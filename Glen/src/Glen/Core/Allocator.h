#pragma once

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <utility>
#include "Glen/Core/Logger.h"
#include "EngineContext.h"

class IAllocator
{
public:
	virtual void* Alloc(size_t size) = 0;
	virtual void Free(std::byte* buffer) = 0;
	virtual void Reset() {};
	virtual void Release() {};
	virtual ~IAllocator() {}
};

class StackAllocator : public IAllocator
{
public:
	StackAllocator(size_t sizeInBytes, IAllocator* parent = nullptr) {
		if (parent == nullptr) {
			buffer = new std::byte[sizeInBytes];
		}
		else {
			void* bytes = parent->Alloc(sizeInBytes);
			buffer = new(bytes) std::byte[sizeInBytes];
		}
		current = buffer;
		totalSize = sizeInBytes;
		marker = buffer;
	}

	void Initialize(){}

	virtual void* Alloc(size_t size) override
	{
		if ((current + size) > (buffer + totalSize)) {
			Logger::logError("Out of memory");
		}
		std::byte* alloc = current;
		current += size;
		return alloc;
	}

	virtual void Free(std::byte* buff) override
	{
		current = buff;
	}

	std::byte* Push()
	{
		marker = current;
		return marker;
	}

	void Pop(std::byte* marker)
	{
		current = marker;
	}

	virtual void Reset() override
	{
		current = buffer;	
	}

	virtual void Release() override
	{
		current = buffer;
		delete[] current;
	}

	~StackAllocator() {}

private:
	std::byte* buffer = nullptr;
	std::byte* current = nullptr;
	std::byte* marker = nullptr;
	size_t totalSize = 0;
};

namespace Mem {
	template<typename T, typename ...Args>
	T* Allocate(Args&& ... args) {
		IAllocator* defaultAllocator = EngineContext::get()->sceneAllocator;
		void* alloc = defaultAllocator->Alloc(sizeof(T));
		return new(alloc) T(std::forward<Args>(args)...);

	}

	template<typename T, typename ...Args>
	T* Allocate(IAllocator* allocator, Args&& ... args) {
		IAllocator* defaultAllocator = EngineContext::get()->sceneAllocator;
		void* alloc = defaultAllocator->Alloc(sizeof(T));
		return new(alloc) T(std::forward<Args>(args)...);

	}
};

#endif // !ALLOCATOR_H
