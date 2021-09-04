#ifndef ANTLR_RTTI_H
#define ANTLR_RTTI_H

#pragma once

#include "antlr4-common.h"

#include <cstdint>
#include <type_traits>

namespace antlr4
{
  static const uint64_t kFNV1aInitialHash64 = 0xCBF29CE484222325ULL;
  static const uint64_t kFNV1aPrime64 = 0x100000001B3ULL;

  inline constexpr uint64_t RTTIHash(const char *const str, uint64_t hash) noexcept
  {
    return (*str == '\0')
      ? hash
      : RTTIHash(str + 1, (hash ^ static_cast<uint64_t>(*str)) * kFNV1aPrime64);
  }

  class ANTLR4CPP_PUBLIC RTTI
  {
  protected:
    typedef uint64_t typeid_t;
    static constexpr typeid_t kTypeId = RTTIHash("RTTI", kFNV1aInitialHash64);

  protected:
    RTTI() = default;

  protected:
    virtual typeid_t GetTypeId() const;
    virtual void* AsType(typeid_t id) = 0;
    virtual const void* AsType(typeid_t tid) const = 0;

  public:
    virtual ~RTTI() = default;

    template<typename T>
    inline T* Cast()
    {
      return static_cast<T *>(AsType(T::kTypeId));
    }

    template<typename T>
    inline const T* Cast() const
    {
      return static_cast<const T *>(AsType(T::kTypeId));
    }
  };

  inline RTTI::typeid_t RTTI::GetTypeId() const
  {
    return RTTI::kTypeId;
  }

  inline void* RTTI::AsType(typeid_t tid)
  {
    return static_cast<void *>((tid == RTTI::kTypeId) ? this : nullptr);
  }

  inline const void* RTTI::AsType(typeid_t tid) const
  {
    return static_cast<const void *>((tid == RTTI::kTypeId) ? this : nullptr);
  }
} // namespace antlr4

#define IMPLEMENT_RTTI(classType, baseType)                                                         \
  protected:                                                                                        \
    typedef baseType Base;                                                                          \
    friend class antlr4::RTTI;                                                                      \
    static constexpr typeid_t kTypeId = antlr4::RTTIHash("/" #classType, Base::kTypeId);            \
    virtual RTTI::typeid_t GetTypeId() const override                                               \
    { return static_cast<RTTI::typeid_t>(classType::kTypeId); }                                     \
    virtual void* AsType(typeid_t tid) override                                                     \
    { return static_cast<void *>((tid == classType::kTypeId) ? this : Base::AsType(tid)); }         \
    virtual const void* AsType(typeid_t tid) const override                                         \
    { return static_cast<const void *>((tid == classType::kTypeId) ? this : Base::AsType(tid)); }   \
  private:

#define IMPLEMENT_RTTI_2_BASES(classType, baseType1, baseType2)                                             \
  protected:                                                                                                \
    typedef baseType1 Base1;                                                                                \
    typedef baseType2 Base2;                                                                                \
    friend class antlr4::RTTI;                                                                              \
    static constexpr typeid_t kTypeId = antlr4::RTTIHash("/" #classType, Base1::kTypeId ^ Base2::kTypeId);  \
    virtual RTTI::typeid_t GetTypeId() const override                                                       \
    { return static_cast<RTTI::typeid_t>(classType::kTypeId); }                                             \
    virtual void* AsType(typeid_t tid) override                                                             \
    {                                                                                                       \
      void *p = nullptr;                                                                                    \
      if (tid == classType::kTypeId) p = static_cast<void *>(this);                                         \
      if (p == nullptr) p = Base1::AsType(tid);                                                             \
      if (p == nullptr) p = Base2::AsType(tid);                                                             \
      return p;                                                                                             \
    }                                                                                                       \
    virtual const void* AsType(typeid_t tid) const override                                                 \
    {                                                                                                       \
      const void *p = nullptr;                                                                              \
      if (tid == classType::kTypeId) p = static_cast<const void *>(this);                                   \
      if (p == nullptr) p = Base1::AsType(tid);                                                             \
      if (p == nullptr) p = Base2::AsType(tid);                                                             \
      return p;                                                                                             \
    }                                                                                                       \
  private:

#define IMPLEMENT_CAST_FUNCTIONS(fname, baseType)                                                                           \
  template<typename T, typename = typename std::enable_if<std::is_base_of<baseType, T>::value>::type>                       \
  inline ANTLR4CPP_PUBLIC T *fname(baseType *const u) noexcept {                                                            \
    return (u == nullptr) ? nullptr : u->template Cast<T>();                                                                \
  }                                                                                                                         \
  template<typename T, typename = typename std::enable_if<std::is_base_of<baseType, T>::value>::type>                       \
  inline ANTLR4CPP_PUBLIC const T *fname(const baseType *const u) noexcept {                                                \
    return (u == nullptr) ? nullptr : u->template Cast<const T>();                                                          \
  }                                                                                                                         \
  template<typename T, typename = typename std::enable_if<std::is_base_of<baseType, T>::value>::type>                       \
  inline ANTLR4CPP_PUBLIC Ref<T> fname(Ref<baseType> const &u) noexcept {                                                   \
    return (u && (u->template Cast<T>() != nullptr)) ? std::static_pointer_cast<T>(u) : Ref<T>(nullptr);                    \
  }                                                                                                                         \
  template<typename T, typename = typename std::enable_if<std::is_base_of<baseType, T>::value>::type>                       \
  inline ANTLR4CPP_PUBLIC Ref<const T> fname(Ref<const baseType> const &u) noexcept {                                       \
    return (u && (u->template Cast<const T>() != nullptr)) ? std::static_pointer_cast<const T>(u) : Ref<const T>(nullptr);  \
  } 

IMPLEMENT_CAST_FUNCTIONS(rtti_cast, antlr4::RTTI)

#endif  /// ANTLR_RTTI_H
