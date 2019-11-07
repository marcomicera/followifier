// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message.proto

#ifndef PROTOBUF_INCLUDED_message_2eproto
#define PROTOBUF_INCLUDED_message_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_message_2eproto 

namespace protobuf_message_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_message_2eproto
namespace followifier {
class Batch;
class BatchDefaultTypeInternal;
extern BatchDefaultTypeInternal _Batch_default_instance_;
class ESP32Message;
class ESP32MessageDefaultTypeInternal;
extern ESP32MessageDefaultTypeInternal _ESP32Message_default_instance_;
}  // namespace followifier
namespace google {
namespace protobuf {
template<> ::followifier::Batch* Arena::CreateMaybeMessage<::followifier::Batch>(Arena*);
template<> ::followifier::ESP32Message* Arena::CreateMaybeMessage<::followifier::ESP32Message>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace followifier {

// ===================================================================

class ESP32Message : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:followifier.ESP32Message) */ {
 public:
  ESP32Message();
  virtual ~ESP32Message();

  ESP32Message(const ESP32Message& from);

  inline ESP32Message& operator=(const ESP32Message& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  ESP32Message(ESP32Message&& from) noexcept
    : ESP32Message() {
    *this = ::std::move(from);
  }

  inline ESP32Message& operator=(ESP32Message&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ESP32Message& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ESP32Message* internal_default_instance() {
    return reinterpret_cast<const ESP32Message*>(
               &_ESP32Message_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(ESP32Message* other);
  friend void swap(ESP32Message& a, ESP32Message& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline ESP32Message* New() const final {
    return CreateMaybeMessage<ESP32Message>(NULL);
  }

  ESP32Message* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<ESP32Message>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const ESP32Message& from);
  void MergeFrom(const ESP32Message& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ESP32Message* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string apMac = 1;
  bool has_apmac() const;
  void clear_apmac();
  static const int kApMacFieldNumber = 1;
  const ::std::string& apmac() const;
  void set_apmac(const ::std::string& value);
  #if LANG_CXX11
  void set_apmac(::std::string&& value);
  #endif
  void set_apmac(const char* value);
  void set_apmac(const char* value, size_t size);
  ::std::string* mutable_apmac();
  ::std::string* release_apmac();
  void set_allocated_apmac(::std::string* apmac);

  // required string ssid = 2;
  bool has_ssid() const;
  void clear_ssid();
  static const int kSsidFieldNumber = 2;
  const ::std::string& ssid() const;
  void set_ssid(const ::std::string& value);
  #if LANG_CXX11
  void set_ssid(::std::string&& value);
  #endif
  void set_ssid(const char* value);
  void set_ssid(const char* value, size_t size);
  ::std::string* mutable_ssid();
  ::std::string* release_ssid();
  void set_allocated_ssid(::std::string* ssid);

  // required bytes frame_hash = 4;
  bool has_frame_hash() const;
  void clear_frame_hash();
  static const int kFrameHashFieldNumber = 4;
  const ::std::string& frame_hash() const;
  void set_frame_hash(const ::std::string& value);
  #if LANG_CXX11
  void set_frame_hash(::std::string&& value);
  #endif
  void set_frame_hash(const char* value);
  void set_frame_hash(const void* value, size_t size);
  ::std::string* mutable_frame_hash();
  ::std::string* release_frame_hash();
  void set_allocated_frame_hash(::std::string* frame_hash);

  // required int64 timestamp = 3;
  bool has_timestamp() const;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 3;
  ::google::protobuf::int64 timestamp() const;
  void set_timestamp(::google::protobuf::int64 value);

  // required int32 rsi = 5;
  bool has_rsi() const;
  void clear_rsi();
  static const int kRsiFieldNumber = 5;
  ::google::protobuf::int32 rsi() const;
  void set_rsi(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:followifier.ESP32Message)
 private:
  void set_has_apmac();
  void clear_has_apmac();
  void set_has_ssid();
  void clear_has_ssid();
  void set_has_timestamp();
  void clear_has_timestamp();
  void set_has_frame_hash();
  void clear_has_frame_hash();
  void set_has_rsi();
  void clear_has_rsi();

  // helper for ByteSizeLong()
  size_t RequiredFieldsByteSizeFallback() const;

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr apmac_;
  ::google::protobuf::internal::ArenaStringPtr ssid_;
  ::google::protobuf::internal::ArenaStringPtr frame_hash_;
  ::google::protobuf::int64 timestamp_;
  ::google::protobuf::int32 rsi_;
  friend struct ::protobuf_message_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class Batch : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:followifier.Batch) */ {
 public:
  Batch();
  virtual ~Batch();

  Batch(const Batch& from);

  inline Batch& operator=(const Batch& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  Batch(Batch&& from) noexcept
    : Batch() {
    *this = ::std::move(from);
  }

  inline Batch& operator=(Batch&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Batch& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Batch* internal_default_instance() {
    return reinterpret_cast<const Batch*>(
               &_Batch_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(Batch* other);
  friend void swap(Batch& a, Batch& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline Batch* New() const final {
    return CreateMaybeMessage<Batch>(NULL);
  }

  Batch* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<Batch>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const Batch& from);
  void MergeFrom(const Batch& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Batch* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .followifier.ESP32Message messages = 2;
  int messages_size() const;
  void clear_messages();
  static const int kMessagesFieldNumber = 2;
  ::followifier::ESP32Message* mutable_messages(int index);
  ::google::protobuf::RepeatedPtrField< ::followifier::ESP32Message >*
      mutable_messages();
  const ::followifier::ESP32Message& messages(int index) const;
  ::followifier::ESP32Message* add_messages();
  const ::google::protobuf::RepeatedPtrField< ::followifier::ESP32Message >&
      messages() const;

  // required string boardMac = 1;
  bool has_boardmac() const;
  void clear_boardmac();
  static const int kBoardMacFieldNumber = 1;
  const ::std::string& boardmac() const;
  void set_boardmac(const ::std::string& value);
  #if LANG_CXX11
  void set_boardmac(::std::string&& value);
  #endif
  void set_boardmac(const char* value);
  void set_boardmac(const char* value, size_t size);
  ::std::string* mutable_boardmac();
  ::std::string* release_boardmac();
  void set_allocated_boardmac(::std::string* boardmac);

  // @@protoc_insertion_point(class_scope:followifier.Batch)
 private:
  void set_has_boardmac();
  void clear_has_boardmac();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::followifier::ESP32Message > messages_;
  ::google::protobuf::internal::ArenaStringPtr boardmac_;
  friend struct ::protobuf_message_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ESP32Message

// required string apMac = 1;
inline bool ESP32Message::has_apmac() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ESP32Message::set_has_apmac() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ESP32Message::clear_has_apmac() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ESP32Message::clear_apmac() {
  apmac_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_apmac();
}
inline const ::std::string& ESP32Message::apmac() const {
  // @@protoc_insertion_point(field_get:followifier.ESP32Message.apMac)
  return apmac_.GetNoArena();
}
inline void ESP32Message::set_apmac(const ::std::string& value) {
  set_has_apmac();
  apmac_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:followifier.ESP32Message.apMac)
}
#if LANG_CXX11
inline void ESP32Message::set_apmac(::std::string&& value) {
  set_has_apmac();
  apmac_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:followifier.ESP32Message.apMac)
}
#endif
inline void ESP32Message::set_apmac(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_apmac();
  apmac_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:followifier.ESP32Message.apMac)
}
inline void ESP32Message::set_apmac(const char* value, size_t size) {
  set_has_apmac();
  apmac_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:followifier.ESP32Message.apMac)
}
inline ::std::string* ESP32Message::mutable_apmac() {
  set_has_apmac();
  // @@protoc_insertion_point(field_mutable:followifier.ESP32Message.apMac)
  return apmac_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ESP32Message::release_apmac() {
  // @@protoc_insertion_point(field_release:followifier.ESP32Message.apMac)
  if (!has_apmac()) {
    return NULL;
  }
  clear_has_apmac();
  return apmac_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ESP32Message::set_allocated_apmac(::std::string* apmac) {
  if (apmac != NULL) {
    set_has_apmac();
  } else {
    clear_has_apmac();
  }
  apmac_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), apmac);
  // @@protoc_insertion_point(field_set_allocated:followifier.ESP32Message.apMac)
}

// required string ssid = 2;
inline bool ESP32Message::has_ssid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ESP32Message::set_has_ssid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ESP32Message::clear_has_ssid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ESP32Message::clear_ssid() {
  ssid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_ssid();
}
inline const ::std::string& ESP32Message::ssid() const {
  // @@protoc_insertion_point(field_get:followifier.ESP32Message.ssid)
  return ssid_.GetNoArena();
}
inline void ESP32Message::set_ssid(const ::std::string& value) {
  set_has_ssid();
  ssid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:followifier.ESP32Message.ssid)
}
#if LANG_CXX11
inline void ESP32Message::set_ssid(::std::string&& value) {
  set_has_ssid();
  ssid_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:followifier.ESP32Message.ssid)
}
#endif
inline void ESP32Message::set_ssid(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_ssid();
  ssid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:followifier.ESP32Message.ssid)
}
inline void ESP32Message::set_ssid(const char* value, size_t size) {
  set_has_ssid();
  ssid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:followifier.ESP32Message.ssid)
}
inline ::std::string* ESP32Message::mutable_ssid() {
  set_has_ssid();
  // @@protoc_insertion_point(field_mutable:followifier.ESP32Message.ssid)
  return ssid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ESP32Message::release_ssid() {
  // @@protoc_insertion_point(field_release:followifier.ESP32Message.ssid)
  if (!has_ssid()) {
    return NULL;
  }
  clear_has_ssid();
  return ssid_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ESP32Message::set_allocated_ssid(::std::string* ssid) {
  if (ssid != NULL) {
    set_has_ssid();
  } else {
    clear_has_ssid();
  }
  ssid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ssid);
  // @@protoc_insertion_point(field_set_allocated:followifier.ESP32Message.ssid)
}

// required int64 timestamp = 3;
inline bool ESP32Message::has_timestamp() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ESP32Message::set_has_timestamp() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ESP32Message::clear_has_timestamp() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ESP32Message::clear_timestamp() {
  timestamp_ = GOOGLE_LONGLONG(0);
  clear_has_timestamp();
}
inline ::google::protobuf::int64 ESP32Message::timestamp() const {
  // @@protoc_insertion_point(field_get:followifier.ESP32Message.timestamp)
  return timestamp_;
}
inline void ESP32Message::set_timestamp(::google::protobuf::int64 value) {
  set_has_timestamp();
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:followifier.ESP32Message.timestamp)
}

// required bytes frame_hash = 4;
inline bool ESP32Message::has_frame_hash() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ESP32Message::set_has_frame_hash() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ESP32Message::clear_has_frame_hash() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ESP32Message::clear_frame_hash() {
  frame_hash_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_frame_hash();
}
inline const ::std::string& ESP32Message::frame_hash() const {
  // @@protoc_insertion_point(field_get:followifier.ESP32Message.frame_hash)
  return frame_hash_.GetNoArena();
}
inline void ESP32Message::set_frame_hash(const ::std::string& value) {
  set_has_frame_hash();
  frame_hash_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:followifier.ESP32Message.frame_hash)
}
#if LANG_CXX11
inline void ESP32Message::set_frame_hash(::std::string&& value) {
  set_has_frame_hash();
  frame_hash_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:followifier.ESP32Message.frame_hash)
}
#endif
inline void ESP32Message::set_frame_hash(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_frame_hash();
  frame_hash_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:followifier.ESP32Message.frame_hash)
}
inline void ESP32Message::set_frame_hash(const void* value, size_t size) {
  set_has_frame_hash();
  frame_hash_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:followifier.ESP32Message.frame_hash)
}
inline ::std::string* ESP32Message::mutable_frame_hash() {
  set_has_frame_hash();
  // @@protoc_insertion_point(field_mutable:followifier.ESP32Message.frame_hash)
  return frame_hash_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ESP32Message::release_frame_hash() {
  // @@protoc_insertion_point(field_release:followifier.ESP32Message.frame_hash)
  if (!has_frame_hash()) {
    return NULL;
  }
  clear_has_frame_hash();
  return frame_hash_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ESP32Message::set_allocated_frame_hash(::std::string* frame_hash) {
  if (frame_hash != NULL) {
    set_has_frame_hash();
  } else {
    clear_has_frame_hash();
  }
  frame_hash_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), frame_hash);
  // @@protoc_insertion_point(field_set_allocated:followifier.ESP32Message.frame_hash)
}

// required int32 rsi = 5;
inline bool ESP32Message::has_rsi() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void ESP32Message::set_has_rsi() {
  _has_bits_[0] |= 0x00000010u;
}
inline void ESP32Message::clear_has_rsi() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void ESP32Message::clear_rsi() {
  rsi_ = 0;
  clear_has_rsi();
}
inline ::google::protobuf::int32 ESP32Message::rsi() const {
  // @@protoc_insertion_point(field_get:followifier.ESP32Message.rsi)
  return rsi_;
}
inline void ESP32Message::set_rsi(::google::protobuf::int32 value) {
  set_has_rsi();
  rsi_ = value;
  // @@protoc_insertion_point(field_set:followifier.ESP32Message.rsi)
}

// -------------------------------------------------------------------

// Batch

// required string boardMac = 1;
inline bool Batch::has_boardmac() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Batch::set_has_boardmac() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Batch::clear_has_boardmac() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Batch::clear_boardmac() {
  boardmac_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_boardmac();
}
inline const ::std::string& Batch::boardmac() const {
  // @@protoc_insertion_point(field_get:followifier.Batch.boardMac)
  return boardmac_.GetNoArena();
}
inline void Batch::set_boardmac(const ::std::string& value) {
  set_has_boardmac();
  boardmac_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:followifier.Batch.boardMac)
}
#if LANG_CXX11
inline void Batch::set_boardmac(::std::string&& value) {
  set_has_boardmac();
  boardmac_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:followifier.Batch.boardMac)
}
#endif
inline void Batch::set_boardmac(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_boardmac();
  boardmac_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:followifier.Batch.boardMac)
}
inline void Batch::set_boardmac(const char* value, size_t size) {
  set_has_boardmac();
  boardmac_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:followifier.Batch.boardMac)
}
inline ::std::string* Batch::mutable_boardmac() {
  set_has_boardmac();
  // @@protoc_insertion_point(field_mutable:followifier.Batch.boardMac)
  return boardmac_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* Batch::release_boardmac() {
  // @@protoc_insertion_point(field_release:followifier.Batch.boardMac)
  if (!has_boardmac()) {
    return NULL;
  }
  clear_has_boardmac();
  return boardmac_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void Batch::set_allocated_boardmac(::std::string* boardmac) {
  if (boardmac != NULL) {
    set_has_boardmac();
  } else {
    clear_has_boardmac();
  }
  boardmac_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), boardmac);
  // @@protoc_insertion_point(field_set_allocated:followifier.Batch.boardMac)
}

// repeated .followifier.ESP32Message messages = 2;
inline int Batch::messages_size() const {
  return messages_.size();
}
inline void Batch::clear_messages() {
  messages_.Clear();
}
inline ::followifier::ESP32Message* Batch::mutable_messages(int index) {
  // @@protoc_insertion_point(field_mutable:followifier.Batch.messages)
  return messages_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::followifier::ESP32Message >*
Batch::mutable_messages() {
  // @@protoc_insertion_point(field_mutable_list:followifier.Batch.messages)
  return &messages_;
}
inline const ::followifier::ESP32Message& Batch::messages(int index) const {
  // @@protoc_insertion_point(field_get:followifier.Batch.messages)
  return messages_.Get(index);
}
inline ::followifier::ESP32Message* Batch::add_messages() {
  // @@protoc_insertion_point(field_add:followifier.Batch.messages)
  return messages_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::followifier::ESP32Message >&
Batch::messages() const {
  // @@protoc_insertion_point(field_list:followifier.Batch.messages)
  return messages_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace followifier

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_message_2eproto
