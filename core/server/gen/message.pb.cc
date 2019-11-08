// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message.proto

#include "message.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace protobuf_message_2eproto {
extern PROTOBUF_INTERNAL_EXPORT_protobuf_message_2eproto ::google::protobuf::internal::SCCInfo<0> scc_info_ESP32Message;
}  // namespace protobuf_message_2eproto
namespace followifier {
class ESP32MessageDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<ESP32Message>
      _instance;
} _ESP32Message_default_instance_;
class BatchDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Batch>
      _instance;
} _Batch_default_instance_;
}  // namespace followifier
namespace protobuf_message_2eproto {
static void InitDefaultsESP32Message() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::followifier::_ESP32Message_default_instance_;
    new (ptr) ::followifier::ESP32Message();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::followifier::ESP32Message::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_ESP32Message =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsESP32Message}, {}};

static void InitDefaultsBatch() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::followifier::_Batch_default_instance_;
    new (ptr) ::followifier::Batch();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::followifier::Batch::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<1> scc_info_Batch =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 1, InitDefaultsBatch}, {
      &protobuf_message_2eproto::scc_info_ESP32Message.base,}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_ESP32Message.base);
  ::google::protobuf::internal::InitSCC(&scc_info_Batch.base);
}

::google::protobuf::Metadata file_level_metadata[2];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::ESP32Message, _has_bits_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::ESP32Message, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::ESP32Message, apmac_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::ESP32Message, ssid_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::ESP32Message, timestamp_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::ESP32Message, frame_hash_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::ESP32Message, rsi_),
  0,
  1,
  3,
  2,
  4,
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::Batch, _has_bits_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::Batch, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::Batch, boardmac_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::followifier::Batch, messages_),
  0,
  ~0u,
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 10, sizeof(::followifier::ESP32Message)},
  { 15, 22, sizeof(::followifier::Batch)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::followifier::_ESP32Message_default_instance_),
  reinterpret_cast<const ::google::protobuf::Message*>(&::followifier::_Batch_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "message.proto", schemas, file_default_instances, TableStruct::offsets,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 2);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\rmessage.proto\022\013followifier\"_\n\014ESP32Mes"
      "sage\022\r\n\005apMac\030\001 \002(\t\022\014\n\004ssid\030\002 \002(\t\022\021\n\ttim"
      "estamp\030\003 \002(\003\022\022\n\nframe_hash\030\004 \002(\014\022\013\n\003rsi\030"
      "\005 \002(\005\"F\n\005Batch\022\020\n\010boardMac\030\001 \002(\t\022+\n\010mess"
      "ages\030\002 \003(\0132\031.followifier.ESP32Message"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 197);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "message.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_message_2eproto
namespace followifier {

// ===================================================================

void ESP32Message::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int ESP32Message::kApMacFieldNumber;
const int ESP32Message::kSsidFieldNumber;
const int ESP32Message::kTimestampFieldNumber;
const int ESP32Message::kFrameHashFieldNumber;
const int ESP32Message::kRsiFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

ESP32Message::ESP32Message()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_message_2eproto::scc_info_ESP32Message.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:followifier.ESP32Message)
}
ESP32Message::ESP32Message(const ESP32Message& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  apmac_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_apmac()) {
    apmac_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.apmac_);
  }
  ssid_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_ssid()) {
    ssid_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.ssid_);
  }
  frame_hash_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_frame_hash()) {
    frame_hash_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.frame_hash_);
  }
  ::memcpy(&timestamp_, &from.timestamp_,
    static_cast<size_t>(reinterpret_cast<char*>(&rsi_) -
    reinterpret_cast<char*>(&timestamp_)) + sizeof(rsi_));
  // @@protoc_insertion_point(copy_constructor:followifier.ESP32Message)
}

void ESP32Message::SharedCtor() {
  apmac_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ssid_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  frame_hash_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&timestamp_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&rsi_) -
      reinterpret_cast<char*>(&timestamp_)) + sizeof(rsi_));
}

ESP32Message::~ESP32Message() {
  // @@protoc_insertion_point(destructor:followifier.ESP32Message)
  SharedDtor();
}

void ESP32Message::SharedDtor() {
  apmac_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ssid_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  frame_hash_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void ESP32Message::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* ESP32Message::descriptor() {
  ::protobuf_message_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_message_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const ESP32Message& ESP32Message::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_message_2eproto::scc_info_ESP32Message.base);
  return *internal_default_instance();
}


void ESP32Message::Clear() {
// @@protoc_insertion_point(message_clear_start:followifier.ESP32Message)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 7u) {
    if (cached_has_bits & 0x00000001u) {
      apmac_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000002u) {
      ssid_.ClearNonDefaultToEmptyNoArena();
    }
    if (cached_has_bits & 0x00000004u) {
      frame_hash_.ClearNonDefaultToEmptyNoArena();
    }
  }
  if (cached_has_bits & 24u) {
    ::memset(&timestamp_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&rsi_) -
        reinterpret_cast<char*>(&timestamp_)) + sizeof(rsi_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool ESP32Message::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:followifier.ESP32Message)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string apMac = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_apmac()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->apmac().data(), static_cast<int>(this->apmac().length()),
            ::google::protobuf::internal::WireFormat::PARSE,
            "followifier.ESP32Message.apMac");
        } else {
          goto handle_unusual;
        }
        break;
      }

      // required string ssid = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u /* 18 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_ssid()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->ssid().data(), static_cast<int>(this->ssid().length()),
            ::google::protobuf::internal::WireFormat::PARSE,
            "followifier.ESP32Message.ssid");
        } else {
          goto handle_unusual;
        }
        break;
      }

      // required int64 timestamp = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(24u /* 24 & 0xFF */)) {
          set_has_timestamp();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &timestamp_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // required bytes frame_hash = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u /* 34 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_frame_hash()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // required int32 rsi = 5;
      case 5: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(40u /* 40 & 0xFF */)) {
          set_has_rsi();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &rsi_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:followifier.ESP32Message)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:followifier.ESP32Message)
  return false;
#undef DO_
}

void ESP32Message::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:followifier.ESP32Message)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required string apMac = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->apmac().data(), static_cast<int>(this->apmac().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "followifier.ESP32Message.apMac");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->apmac(), output);
  }

  // required string ssid = 2;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->ssid().data(), static_cast<int>(this->ssid().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "followifier.ESP32Message.ssid");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->ssid(), output);
  }

  // required int64 timestamp = 3;
  if (cached_has_bits & 0x00000008u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(3, this->timestamp(), output);
  }

  // required bytes frame_hash = 4;
  if (cached_has_bits & 0x00000004u) {
    ::google::protobuf::internal::WireFormatLite::WriteBytesMaybeAliased(
      4, this->frame_hash(), output);
  }

  // required int32 rsi = 5;
  if (cached_has_bits & 0x00000010u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(5, this->rsi(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:followifier.ESP32Message)
}

::google::protobuf::uint8* ESP32Message::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:followifier.ESP32Message)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required string apMac = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->apmac().data(), static_cast<int>(this->apmac().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "followifier.ESP32Message.apMac");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->apmac(), target);
  }

  // required string ssid = 2;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->ssid().data(), static_cast<int>(this->ssid().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "followifier.ESP32Message.ssid");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->ssid(), target);
  }

  // required int64 timestamp = 3;
  if (cached_has_bits & 0x00000008u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(3, this->timestamp(), target);
  }

  // required bytes frame_hash = 4;
  if (cached_has_bits & 0x00000004u) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        4, this->frame_hash(), target);
  }

  // required int32 rsi = 5;
  if (cached_has_bits & 0x00000010u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(5, this->rsi(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:followifier.ESP32Message)
  return target;
}

size_t ESP32Message::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:followifier.ESP32Message)
  size_t total_size = 0;

  if (has_apmac()) {
    // required string apMac = 1;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->apmac());
  }

  if (has_ssid()) {
    // required string ssid = 2;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->ssid());
  }

  if (has_frame_hash()) {
    // required bytes frame_hash = 4;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::BytesSize(
        this->frame_hash());
  }

  if (has_timestamp()) {
    // required int64 timestamp = 3;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int64Size(
        this->timestamp());
  }

  if (has_rsi()) {
    // required int32 rsi = 5;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->rsi());
  }

  return total_size;
}
size_t ESP32Message::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:followifier.ESP32Message)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  if (((_has_bits_[0] & 0x0000001f) ^ 0x0000001f) == 0) {  // All required fields are present.
    // required string apMac = 1;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->apmac());

    // required string ssid = 2;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->ssid());

    // required bytes frame_hash = 4;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::BytesSize(
        this->frame_hash());

    // required int64 timestamp = 3;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int64Size(
        this->timestamp());

    // required int32 rsi = 5;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->rsi());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ESP32Message::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:followifier.ESP32Message)
  GOOGLE_DCHECK_NE(&from, this);
  const ESP32Message* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const ESP32Message>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:followifier.ESP32Message)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:followifier.ESP32Message)
    MergeFrom(*source);
  }
}

void ESP32Message::MergeFrom(const ESP32Message& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:followifier.ESP32Message)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 31u) {
    if (cached_has_bits & 0x00000001u) {
      set_has_apmac();
      apmac_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.apmac_);
    }
    if (cached_has_bits & 0x00000002u) {
      set_has_ssid();
      ssid_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.ssid_);
    }
    if (cached_has_bits & 0x00000004u) {
      set_has_frame_hash();
      frame_hash_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.frame_hash_);
    }
    if (cached_has_bits & 0x00000008u) {
      timestamp_ = from.timestamp_;
    }
    if (cached_has_bits & 0x00000010u) {
      rsi_ = from.rsi_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void ESP32Message::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:followifier.ESP32Message)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ESP32Message::CopyFrom(const ESP32Message& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:followifier.ESP32Message)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ESP32Message::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000001f) != 0x0000001f) return false;
  return true;
}

void ESP32Message::Swap(ESP32Message* other) {
  if (other == this) return;
  InternalSwap(other);
}
void ESP32Message::InternalSwap(ESP32Message* other) {
  using std::swap;
  apmac_.Swap(&other->apmac_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  ssid_.Swap(&other->ssid_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  frame_hash_.Swap(&other->frame_hash_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(timestamp_, other->timestamp_);
  swap(rsi_, other->rsi_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata ESP32Message::GetMetadata() const {
  protobuf_message_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_message_2eproto::file_level_metadata[kIndexInFileMessages];
}


// ===================================================================

void Batch::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Batch::kBoardMacFieldNumber;
const int Batch::kMessagesFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Batch::Batch()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_message_2eproto::scc_info_Batch.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:followifier.Batch)
}
Batch::Batch(const Batch& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_),
      messages_(from.messages_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  boardmac_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_boardmac()) {
    boardmac_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.boardmac_);
  }
  // @@protoc_insertion_point(copy_constructor:followifier.Batch)
}

void Batch::SharedCtor() {
  boardmac_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

Batch::~Batch() {
  // @@protoc_insertion_point(destructor:followifier.Batch)
  SharedDtor();
}

void Batch::SharedDtor() {
  boardmac_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void Batch::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* Batch::descriptor() {
  ::protobuf_message_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_message_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Batch& Batch::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_message_2eproto::scc_info_Batch.base);
  return *internal_default_instance();
}


void Batch::Clear() {
// @@protoc_insertion_point(message_clear_start:followifier.Batch)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  messages_.Clear();
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    boardmac_.ClearNonDefaultToEmptyNoArena();
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool Batch::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:followifier.Batch)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string boardMac = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_boardmac()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->boardmac().data(), static_cast<int>(this->boardmac().length()),
            ::google::protobuf::internal::WireFormat::PARSE,
            "followifier.Batch.boardMac");
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated .followifier.ESP32Message messages = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u /* 18 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessage(
                input, add_messages()));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:followifier.Batch)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:followifier.Batch)
  return false;
#undef DO_
}

void Batch::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:followifier.Batch)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required string boardMac = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->boardmac().data(), static_cast<int>(this->boardmac().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "followifier.Batch.boardMac");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->boardmac(), output);
  }

  // repeated .followifier.ESP32Message messages = 2;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->messages_size()); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      2,
      this->messages(static_cast<int>(i)),
      output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:followifier.Batch)
}

::google::protobuf::uint8* Batch::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:followifier.Batch)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required string boardMac = 1;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->boardmac().data(), static_cast<int>(this->boardmac().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "followifier.Batch.boardMac");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->boardmac(), target);
  }

  // repeated .followifier.ESP32Message messages = 2;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->messages_size()); i < n; i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      InternalWriteMessageToArray(
        2, this->messages(static_cast<int>(i)), deterministic, target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:followifier.Batch)
  return target;
}

size_t Batch::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:followifier.Batch)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  // required string boardMac = 1;
  if (has_boardmac()) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->boardmac());
  }
  // repeated .followifier.ESP32Message messages = 2;
  {
    unsigned int count = static_cast<unsigned int>(this->messages_size());
    total_size += 1UL * count;
    for (unsigned int i = 0; i < count; i++) {
      total_size +=
        ::google::protobuf::internal::WireFormatLite::MessageSize(
          this->messages(static_cast<int>(i)));
    }
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Batch::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:followifier.Batch)
  GOOGLE_DCHECK_NE(&from, this);
  const Batch* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Batch>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:followifier.Batch)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:followifier.Batch)
    MergeFrom(*source);
  }
}

void Batch::MergeFrom(const Batch& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:followifier.Batch)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  messages_.MergeFrom(from.messages_);
  if (from.has_boardmac()) {
    set_has_boardmac();
    boardmac_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.boardmac_);
  }
}

void Batch::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:followifier.Batch)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Batch::CopyFrom(const Batch& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:followifier.Batch)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Batch::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  if (!::google::protobuf::internal::AllAreInitialized(this->messages())) return false;
  return true;
}

void Batch::Swap(Batch* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Batch::InternalSwap(Batch* other) {
  using std::swap;
  CastToBase(&messages_)->InternalSwap(CastToBase(&other->messages_));
  boardmac_.Swap(&other->boardmac_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata Batch::GetMetadata() const {
  protobuf_message_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_message_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace followifier
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::followifier::ESP32Message* Arena::CreateMaybeMessage< ::followifier::ESP32Message >(Arena* arena) {
  return Arena::CreateInternal< ::followifier::ESP32Message >(arena);
}
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::followifier::Batch* Arena::CreateMaybeMessage< ::followifier::Batch >(Arena* arena) {
  return Arena::CreateInternal< ::followifier::Batch >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
