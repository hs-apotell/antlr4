/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

#pragma once

#include <cstddef>

namespace antlr4 {

  class FixedAllocator final {
  public:
    FixedAllocator(std::size_t size, std::size_t count);
    ~FixedAllocator();

    void *Allocate();
    void Free(void *const ptr);
    void Purge();

  private:
    bool AddBlock();

  private:
    struct Chunk {
      Chunk *next = nullptr;
    };

    struct Block {
      void *data = nullptr;
      Block *next = nullptr;
    };

    Chunk *chunkHead = nullptr;
    Block *blockHead = nullptr;
    const std::size_t chunkSize = 0;
    const std::size_t chunkCount = 0;
  };

  class LinearAllocator final {
  public:
    LinearAllocator(std::size_t blockSize);
    ~LinearAllocator();

    void *Allocate(std::size_t size);
    void Purge();

  private:
    bool AddBlock();

  private:
    struct Block {
      char *data = nullptr;
      char *head = nullptr;
      std::size_t available = 0;
      Block *next = nullptr;
    };

    Block *head = nullptr;
    const std::size_t blockSize = 0;

  private:
    LinearAllocator(const LinearAllocator &) = delete;
    LinearAllocator &operator = (const LinearAllocator &) = delete;
  };

} // namespace antlr4
