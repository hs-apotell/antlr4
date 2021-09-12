/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

#pragma once

#include "antlr4-common.h"
#include "RTTI.h"

namespace antlr4 {

  // An exception hierarchy modelled loosely after java.lang.* exceptions.
  class ANTLR4CPP_PUBLIC RuntimeException : public std::exception, public RTTI {
    ANTLR_IMPLEMENT_RTTI(RuntimeException, RTTI)
  private:
    std::string _message;
  public:
    RuntimeException(const std::string &msg = "");

    virtual const char* what() const NOEXCEPT override;
  };

  class ANTLR4CPP_PUBLIC IllegalStateException : public RuntimeException {
    ANTLR_IMPLEMENT_RTTI(IllegalStateException, RuntimeException)
  public:
    IllegalStateException(const std::string &msg = "") : RuntimeException(msg) {}
    IllegalStateException(IllegalStateException const&) = default;
    ~IllegalStateException();
    IllegalStateException& operator=(IllegalStateException const&) = default;
  };

  class ANTLR4CPP_PUBLIC IllegalArgumentException : public RuntimeException {
    ANTLR_IMPLEMENT_RTTI(IllegalArgumentException, RuntimeException)
  public:
    IllegalArgumentException(IllegalArgumentException const&) = default;
    IllegalArgumentException(const std::string &msg = "") : RuntimeException(msg) {}
    ~IllegalArgumentException();
    IllegalArgumentException& operator=(IllegalArgumentException const&) = default;
  };

  class ANTLR4CPP_PUBLIC NullPointerException : public RuntimeException {
    ANTLR_IMPLEMENT_RTTI(NullPointerException, RuntimeException)
  public:
    NullPointerException(const std::string &msg = "") : RuntimeException(msg) {}
    NullPointerException(NullPointerException const&) = default;
    ~NullPointerException();
    NullPointerException& operator=(NullPointerException const&) = default;
  };

  class ANTLR4CPP_PUBLIC IndexOutOfBoundsException : public RuntimeException {
    ANTLR_IMPLEMENT_RTTI(IndexOutOfBoundsException, RuntimeException)
  public:
    IndexOutOfBoundsException(const std::string &msg = "") : RuntimeException(msg) {}
    IndexOutOfBoundsException(IndexOutOfBoundsException const&) = default;
    ~IndexOutOfBoundsException();
    IndexOutOfBoundsException& operator=(IndexOutOfBoundsException const&) = default;
  };

  class ANTLR4CPP_PUBLIC UnsupportedOperationException : public RuntimeException {
    ANTLR_IMPLEMENT_RTTI(UnsupportedOperationException, RuntimeException)
  public:
    UnsupportedOperationException(const std::string &msg = "") : RuntimeException(msg) {}
    UnsupportedOperationException(UnsupportedOperationException const&) = default;
    ~UnsupportedOperationException();
    UnsupportedOperationException& operator=(UnsupportedOperationException const&) = default;

  };

  class ANTLR4CPP_PUBLIC EmptyStackException : public RuntimeException {
    ANTLR_IMPLEMENT_RTTI(EmptyStackException, RuntimeException)
  public:
    EmptyStackException(const std::string &msg = "") : RuntimeException(msg) {}
    EmptyStackException(EmptyStackException const&) = default;
    ~EmptyStackException();
    EmptyStackException& operator=(EmptyStackException const&) = default;
  };

  // IOException is not a runtime exception (in the java hierarchy).
  // Hence we have to duplicate the RuntimeException implementation.
  class ANTLR4CPP_PUBLIC IOException : public std::exception, public RTTI {
    ANTLR_IMPLEMENT_RTTI(IOException, RTTI)
  private:
    std::string _message;

  public:
    IOException(const std::string &msg = "");

    virtual const char* what() const NOEXCEPT override;
  };

  class ANTLR4CPP_PUBLIC CancellationException : public IllegalStateException {
    ANTLR_IMPLEMENT_RTTI(CancellationException, IllegalStateException)
  public:
    CancellationException(const std::string &msg = "") : IllegalStateException(msg) {}
    CancellationException(CancellationException const&) = default;
    ~CancellationException();
    CancellationException& operator=(CancellationException const&) = default;
  };

  class ANTLR4CPP_PUBLIC ParseCancellationException : public CancellationException {
    ANTLR_IMPLEMENT_RTTI(ParseCancellationException, CancellationException)
  public:
    ParseCancellationException(const std::string &msg = "") : CancellationException(msg) {}
    ParseCancellationException(ParseCancellationException const&) = default;
    ~ParseCancellationException();
    ParseCancellationException& operator=(ParseCancellationException const&) = default;
  };

} // namespace antlr4

ANTLR_IMPLEMENT_RTTI_CAST_FUNCTIONS(antlr4::RuntimeException)
