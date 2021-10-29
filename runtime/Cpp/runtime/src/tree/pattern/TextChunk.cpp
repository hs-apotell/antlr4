/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

#include "Exceptions.h"

#include "tree/pattern/TextChunk.h"

using namespace antlr4::tree::pattern;

TextChunk::TextChunk(const std::string &text) : text(text) {
  classtype |= TextChunkClass;
  if (text == "") {
    throw IllegalArgumentException("text cannot be nul");
  }

}

TextChunk::~TextChunk() {
}

const std::string& TextChunk::getText() const {
  return text;
}

std::string TextChunk::toString() const {
  return std::string("'") + text + std::string("'");
}
