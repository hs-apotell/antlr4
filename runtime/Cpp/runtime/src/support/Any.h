/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

// A standard C++ class loosely modeled after boost::Any.

#pragma once

#include "antlr4-common.h"
#include "RTTI.h"

#ifdef _MSC_VER
  #pragma warning(push)
  #pragma warning(disable: 4521) // 'antlrcpp::Any': multiple copy constructors specified
#endif

namespace antlrcpp {

template<class T>
  using StorageType = typename std::decay<T>::type;

struct ANTLR4CPP_PUBLIC Any
{
  bool isNull() const { return _ptr == nullptr; }
  bool isNotNull() const { return _ptr != nullptr; }

  Any() : _ptr(nullptr) {
  }

  Any(Any& that) : _ptr(that.clone()) {
  }

  Any(Any&& that) : _ptr(that._ptr) {
    that._ptr = nullptr;
  }

  Any(const Any& that) : _ptr(that.clone()) {
  }

  Any(const Any&& that) : _ptr(that.clone()) {
  }

  template<typename U>
  Any(U&& value) : _ptr(new Derived<StorageType<U>>(std::forward<U>(value))) {
  }

  template<class U>
  bool is() const {
    auto derived = getDerived<U>(false);

    return derived != nullptr;
  }

  template<class U>
  StorageType<U>& as() {
    auto derived = getDerived<U>(true);

    return derived->value;
  }

  template<class U>
  const StorageType<U>& as() const {
    auto derived = getDerived<U>(true);

    return derived->value;
  }

  template<class U, typename std::enable_if<std::is_copy_constructible<U>::value || std::is_copy_assignable<U>::value>::value>
  operator U() {
    return as<StorageType<U>>();
  }

  template<class U, typename std::enable_if<(!std::is_copy_constructible<U>::value && !std::is_copy_assignable<U>::value) && (std::is_move_constructible<U>::value || std::is_move_assignable<U>::value)>::value>
  operator U() {
    return std::move(as<StorageType<U>>());
  }

  template<class U, typename std::enable_if<std::is_copy_constructible<U>::value || std::is_copy_assignable<U>::value>::value>
  operator const U() const {
    return as<const StorageType<U>>();
  }

  template<class U, typename std::enable_if<!(!std::is_copy_constructible<U>::value && !std::is_copy_assignable<U>::value) && (std::is_move_constructible<U>::value || std::is_move_assignable<U>::value)>::value>
  operator const U() const {
    return std::move(as<const StorageType<U>>());
  }

  Any& operator = (const Any& a) {
    if (_ptr == a._ptr)
      return *this;

    auto * old_ptr = _ptr;
    _ptr = a.clone();

    if (old_ptr)
      delete old_ptr;

    return *this;
  }

  Any& operator = (Any&& a) {
    if (_ptr == a._ptr)
      return *this;

    std::swap(_ptr, a._ptr);

    return *this;
  }

  virtual ~Any();

  virtual bool equals(const Any& other) const {
    return _ptr == other._ptr;
  }

private:
  struct Base : public antlr4::RTTI {
    ANTLR_IMPLEMENT_RTTI(Base, antlr4::RTTI)

  public:
    virtual ~Base() {};
    virtual Base* clone() const = 0;
  };
  ANTLR_IMPLEMENT_RTTI_CAST_FUNCTIONS(Base)

  template<typename T, typename = typename std::enable_if<std::is_base_of<antlr4::RTTI, T>::value>::type>
  struct Derived : Base {
  public:
    typedef Derived<T> thistype_t;
    typedef Base basetype_t;
    static constexpr typeid_t kTypeId = antlr4::internal::RTTIHash("/Derived", T::kTypeId);
    static constexpr auto kTypeIds = antlr4::internal::join(
      std::array<typeid_t, 1>{thistype_t::kTypeId}, basetype_t::kTypeIds);
  protected:
    friend class antlr4::RTTI;
    inline virtual RTTI::typeid_t GetTypeId() const override { return thistype_t::kTypeId; }
    inline virtual const RTTI::typeid_t *GetTypeIds(size_t &count) const override
    { count = thistype_t::kTypeIds.size(); return thistype_t::kTypeIds.data(); }
    inline void *AsOfTypeRecurse(typeid_t tid)
    { return (tid == thistype_t::kTypeId) ? static_cast<void *>(this) : basetype_t::AsOfTypeRecurse(tid); }
    inline const void *AsOfTypeRecurse(typeid_t tid) const
    { return (tid == thistype_t::kTypeId) ? static_cast<const void *>(this) : basetype_t::AsOfTypeRecurse(tid); }
    inline virtual void *AsOfType(typeid_t tid) override { return thistype_t::AsOfTypeRecurse(tid); }
    inline virtual const void *AsOfType(typeid_t tid) const override { return thistype_t::AsOfTypeRecurse(tid); }

  public:
    template<typename U> Derived(U&& value_) : value(std::forward<U>(value_)) {
    }

    T value;

    Base* clone() const {
      return clone<>();
    }

  private:
    template<int N = 0, typename std::enable_if<N == N && std::is_nothrow_copy_constructible<T>::value, int>::type = 0>
    Base* clone() const {
      return new Derived<T>(value);
    }

    template<int N = 0, typename std::enable_if<N == N && !std::is_nothrow_copy_constructible<T>::value, int>::type = 0>
    Base* clone() const {
      return nullptr;
    }

  };

  Base* clone() const
  {
    if (_ptr)
      return _ptr->clone();
    else
      return nullptr;
  }

  template<class U, typename T = StorageType<U>>
  Derived<T>* getDerived(bool checkCast) const {
    auto derived = antlr_cast<Derived<T>*>(_ptr);

    if (checkCast && !derived)
      throw std::bad_cast();

    return derived;
  }

  Base *_ptr;

};

  template<> inline
  Any::Any(std::nullptr_t&& ) : _ptr(nullptr) {
  }


} // namespace antlrcpp

#ifdef _MSC_VER
#pragma warning(pop)
#endif
