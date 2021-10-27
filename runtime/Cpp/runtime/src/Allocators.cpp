/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

#include "Allocators.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>

namespace antlr4 {
  static constexpr int kAlignment = 8;

  template<typename T>
  static T RoundUp(T n) {
    return (n + kAlignment - 1) & (-kAlignment);
  }

  FixedAllocator::FixedAllocator(std::size_t size, std::size_t count)
    : chunkHead(nullptr)
    , blockHead(nullptr)
    , chunkSize(RoundUp(std::max(sizeof(Chunk), size)))
    , chunkCount(count) {
  }

  FixedAllocator::~FixedAllocator() {
    Purge();
  }

  bool FixedAllocator::AddBlock() {
    const std::size_t blockSize = (chunkSize * chunkCount) + RoundUp(sizeof(Block)) + kAlignment - 1;
    char *const data = static_cast<char *>(malloc(blockSize));
    if (data == nullptr) return false;

    Block *const block = reinterpret_cast<Block *>(RoundUp(reinterpret_cast<intptr_t>(data)));
    block->data = data;
    block->next = blockHead;
    blockHead = block;

    chunkHead = reinterpret_cast<Chunk *>(reinterpret_cast<intptr_t>(block) + RoundUp(sizeof(Block)));

    char *chunkNMinus1 = reinterpret_cast<char *>(chunkHead);
    char *chunkN = chunkNMinus1 + chunkSize;
    const char *const chunkEnd = chunkNMinus1 + (chunkSize * chunkCount);
    while (chunkN < chunkEnd) {
      reinterpret_cast<Chunk *>(chunkNMinus1)->next = reinterpret_cast<Chunk *>(chunkN);
      chunkNMinus1 = chunkN;
      chunkN += chunkSize;
    }
    reinterpret_cast<Chunk *>(chunkNMinus1)->next = nullptr;
    return true;
  }

  void *FixedAllocator::Allocate() {
    if ((chunkHead == nullptr) && !AddBlock()) {
      return nullptr;
    }

    void *const p = reinterpret_cast<void *>(chunkHead);
    chunkHead = chunkHead->next;
    return p;
  }

  void FixedAllocator::Free(void *const ptr) {
    Chunk *const chunk = reinterpret_cast<Chunk *>(ptr);
    chunk->next = chunkHead;
    chunkHead = chunk;
  }

  void FixedAllocator::Purge() {
    while (blockHead != nullptr) {
      Block *const next = blockHead->next;
      free(blockHead->data);
      blockHead = next;
    }
  }

  LinearAllocator::LinearAllocator(std::size_t blockSize)
    : head(nullptr)
    , blockSize(RoundUp(blockSize)) {
  }

  bool LinearAllocator::AddBlock() {
    char *const data = static_cast<char *>(malloc(blockSize + RoundUp(sizeof(Block)) + kAlignment - 1));
    if (data == nullptr) return false;

    Block *const block = reinterpret_cast<Block *>(RoundUp(reinterpret_cast<intptr_t>(data)));
    block->data = data;
    block->head = reinterpret_cast<char *>(block) + RoundUp(sizeof(Block));
    block->available = blockSize;
    block->next = head;
    head = block;

    return true;
  }

  LinearAllocator::~LinearAllocator() {
    Purge();
  }

  void *LinearAllocator::Allocate(std::size_t size) {
    size = RoundUp(size);

    if ((head == nullptr) || (head->available < size)) {
      if (!AddBlock()) return nullptr;
    }

    void *const p = static_cast<void *>(head->head);
    head->head += size;
    head->available -= size;
    return p;
  }

  void LinearAllocator::Purge() {
    while (head != nullptr) {
      Block *const next = head->next;
      free(head->data);
      head = next;
    }
  }

} // namespace antlr4
