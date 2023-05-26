// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: uart_comm.proto

#include "uart_comm.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace uart {
namespace comm {
PROTOBUF_CONSTEXPR MessageK2D::MessageK2D(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_._has_bits_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_.action_)*/0
  , /*decltype(_impl_.payload_)*/{}
  , /*decltype(_impl_._oneof_case_)*/{}} {}
struct MessageK2DDefaultTypeInternal {
  PROTOBUF_CONSTEXPR MessageK2DDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~MessageK2DDefaultTypeInternal() {}
  union {
    MessageK2D _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 MessageK2DDefaultTypeInternal _MessageK2D_default_instance_;
PROTOBUF_CONSTEXPR Nop::Nop(
    ::_pbi::ConstantInitialized) {}
struct NopDefaultTypeInternal {
  PROTOBUF_CONSTEXPR NopDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~NopDefaultTypeInternal() {}
  union {
    Nop _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 NopDefaultTypeInternal _Nop_default_instance_;
PROTOBUF_CONSTEXPR FnState::FnState(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_._has_bits_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_.pressed_)*/false} {}
struct FnStateDefaultTypeInternal {
  PROTOBUF_CONSTEXPR FnStateDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~FnStateDefaultTypeInternal() {}
  union {
    FnState _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 FnStateDefaultTypeInternal _FnState_default_instance_;
}  // namespace comm
}  // namespace uart
static ::_pb::Metadata file_level_metadata_uart_5fcomm_2eproto[3];
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_uart_5fcomm_2eproto[1];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_uart_5fcomm_2eproto = nullptr;

const uint32_t TableStruct_uart_5fcomm_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::uart::comm::MessageK2D, _impl_._has_bits_),
  PROTOBUF_FIELD_OFFSET(::uart::comm::MessageK2D, _internal_metadata_),
  ~0u,  // no _extensions_
  PROTOBUF_FIELD_OFFSET(::uart::comm::MessageK2D, _impl_._oneof_case_[0]),
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::uart::comm::MessageK2D, _impl_.action_),
  ::_pbi::kInvalidFieldOffsetTag,
  ::_pbi::kInvalidFieldOffsetTag,
  PROTOBUF_FIELD_OFFSET(::uart::comm::MessageK2D, _impl_.payload_),
  0,
  ~0u,
  ~0u,
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::uart::comm::Nop, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::uart::comm::FnState, _impl_._has_bits_),
  PROTOBUF_FIELD_OFFSET(::uart::comm::FnState, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::uart::comm::FnState, _impl_.pressed_),
  0,
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 10, -1, sizeof(::uart::comm::MessageK2D)},
  { 13, -1, -1, sizeof(::uart::comm::Nop)},
  { 19, 26, -1, sizeof(::uart::comm::FnState)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::uart::comm::_MessageK2D_default_instance_._instance,
  &::uart::comm::_Nop_default_instance_._instance,
  &::uart::comm::_FnState_default_instance_._instance,
};

const char descriptor_table_protodef_uart_5fcomm_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\017uart_comm.proto\022\tuart.comm\"\201\001\n\nMessage"
  "K2D\022!\n\006action\030\001 \002(\0162\021.uart.comm.Action\022\035"
  "\n\003nop\030\002 \001(\0132\016.uart.comm.NopH\000\022&\n\010fn_stat"
  "e\030\003 \001(\0132\022.uart.comm.FnStateH\000B\t\n\007payload"
  "\"\005\n\003Nop\"\032\n\007FnState\022\017\n\007pressed\030\001 \002(\010*1\n\006A"
  "ction\022\007\n\003NOP\020\000\022\010\n\004PING\020\001\022\024\n\020FN_STATE_CHA"
  "NGED\020\002"
  ;
static ::_pbi::once_flag descriptor_table_uart_5fcomm_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_uart_5fcomm_2eproto = {
    false, false, 246, descriptor_table_protodef_uart_5fcomm_2eproto,
    "uart_comm.proto",
    &descriptor_table_uart_5fcomm_2eproto_once, nullptr, 0, 3,
    schemas, file_default_instances, TableStruct_uart_5fcomm_2eproto::offsets,
    file_level_metadata_uart_5fcomm_2eproto, file_level_enum_descriptors_uart_5fcomm_2eproto,
    file_level_service_descriptors_uart_5fcomm_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_uart_5fcomm_2eproto_getter() {
  return &descriptor_table_uart_5fcomm_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_uart_5fcomm_2eproto(&descriptor_table_uart_5fcomm_2eproto);
namespace uart {
namespace comm {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Action_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_uart_5fcomm_2eproto);
  return file_level_enum_descriptors_uart_5fcomm_2eproto[0];
}
bool Action_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}


// ===================================================================

class MessageK2D::_Internal {
 public:
  using HasBits = decltype(std::declval<MessageK2D>()._impl_._has_bits_);
  static void set_has_action(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static const ::uart::comm::Nop& nop(const MessageK2D* msg);
  static const ::uart::comm::FnState& fn_state(const MessageK2D* msg);
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000001) ^ 0x00000001) != 0;
  }
};

const ::uart::comm::Nop&
MessageK2D::_Internal::nop(const MessageK2D* msg) {
  return *msg->_impl_.payload_.nop_;
}
const ::uart::comm::FnState&
MessageK2D::_Internal::fn_state(const MessageK2D* msg) {
  return *msg->_impl_.payload_.fn_state_;
}
void MessageK2D::set_allocated_nop(::uart::comm::Nop* nop) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  clear_payload();
  if (nop) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(nop);
    if (message_arena != submessage_arena) {
      nop = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, nop, submessage_arena);
    }
    set_has_nop();
    _impl_.payload_.nop_ = nop;
  }
  // @@protoc_insertion_point(field_set_allocated:uart.comm.MessageK2D.nop)
}
void MessageK2D::set_allocated_fn_state(::uart::comm::FnState* fn_state) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArenaForAllocation();
  clear_payload();
  if (fn_state) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::InternalGetOwningArena(fn_state);
    if (message_arena != submessage_arena) {
      fn_state = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, fn_state, submessage_arena);
    }
    set_has_fn_state();
    _impl_.payload_.fn_state_ = fn_state;
  }
  // @@protoc_insertion_point(field_set_allocated:uart.comm.MessageK2D.fn_state)
}
MessageK2D::MessageK2D(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:uart.comm.MessageK2D)
}
MessageK2D::MessageK2D(const MessageK2D& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  MessageK2D* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){from._impl_._has_bits_}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.action_){}
    , decltype(_impl_.payload_){}
    , /*decltype(_impl_._oneof_case_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.action_ = from._impl_.action_;
  clear_has_payload();
  switch (from.payload_case()) {
    case kNop: {
      _this->_internal_mutable_nop()->::uart::comm::Nop::MergeFrom(
          from._internal_nop());
      break;
    }
    case kFnState: {
      _this->_internal_mutable_fn_state()->::uart::comm::FnState::MergeFrom(
          from._internal_fn_state());
      break;
    }
    case PAYLOAD_NOT_SET: {
      break;
    }
  }
  // @@protoc_insertion_point(copy_constructor:uart.comm.MessageK2D)
}

inline void MessageK2D::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.action_){0}
    , decltype(_impl_.payload_){}
    , /*decltype(_impl_._oneof_case_)*/{}
  };
  clear_has_payload();
}

MessageK2D::~MessageK2D() {
  // @@protoc_insertion_point(destructor:uart.comm.MessageK2D)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void MessageK2D::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (has_payload()) {
    clear_payload();
  }
}

void MessageK2D::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void MessageK2D::clear_payload() {
// @@protoc_insertion_point(one_of_clear_start:uart.comm.MessageK2D)
  switch (payload_case()) {
    case kNop: {
      if (GetArenaForAllocation() == nullptr) {
        delete _impl_.payload_.nop_;
      }
      break;
    }
    case kFnState: {
      if (GetArenaForAllocation() == nullptr) {
        delete _impl_.payload_.fn_state_;
      }
      break;
    }
    case PAYLOAD_NOT_SET: {
      break;
    }
  }
  _impl_._oneof_case_[0] = PAYLOAD_NOT_SET;
}


void MessageK2D::Clear() {
// @@protoc_insertion_point(message_clear_start:uart.comm.MessageK2D)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.action_ = 0;
  clear_payload();
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* MessageK2D::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required .uart.comm.Action action = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          uint64_t val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::uart::comm::Action_IsValid(val))) {
            _internal_set_action(static_cast<::uart::comm::Action>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(1, val, mutable_unknown_fields());
          }
        } else
          goto handle_unusual;
        continue;
      // .uart.comm.Nop nop = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr = ctx->ParseMessage(_internal_mutable_nop(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // .uart.comm.FnState fn_state = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          ptr = ctx->ParseMessage(_internal_mutable_fn_state(), ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  _impl_._has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* MessageK2D::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:uart.comm.MessageK2D)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  // required .uart.comm.Action action = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteEnumToArray(
      1, this->_internal_action(), target);
  }

  switch (payload_case()) {
    case kNop: {
      target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
        InternalWriteMessage(2, _Internal::nop(this),
          _Internal::nop(this).GetCachedSize(), target, stream);
      break;
    }
    case kFnState: {
      target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
        InternalWriteMessage(3, _Internal::fn_state(this),
          _Internal::fn_state(this).GetCachedSize(), target, stream);
      break;
    }
    default: ;
  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:uart.comm.MessageK2D)
  return target;
}

size_t MessageK2D::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:uart.comm.MessageK2D)
  size_t total_size = 0;

  // required .uart.comm.Action action = 1;
  if (_internal_has_action()) {
    total_size += 1 +
      ::_pbi::WireFormatLite::EnumSize(this->_internal_action());
  }
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  switch (payload_case()) {
    // .uart.comm.Nop nop = 2;
    case kNop: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
          *_impl_.payload_.nop_);
      break;
    }
    // .uart.comm.FnState fn_state = 3;
    case kFnState: {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
          *_impl_.payload_.fn_state_);
      break;
    }
    case PAYLOAD_NOT_SET: {
      break;
    }
  }
  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData MessageK2D::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    MessageK2D::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*MessageK2D::GetClassData() const { return &_class_data_; }


void MessageK2D::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<MessageK2D*>(&to_msg);
  auto& from = static_cast<const MessageK2D&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:uart.comm.MessageK2D)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_action()) {
    _this->_internal_set_action(from._internal_action());
  }
  switch (from.payload_case()) {
    case kNop: {
      _this->_internal_mutable_nop()->::uart::comm::Nop::MergeFrom(
          from._internal_nop());
      break;
    }
    case kFnState: {
      _this->_internal_mutable_fn_state()->::uart::comm::FnState::MergeFrom(
          from._internal_fn_state());
      break;
    }
    case PAYLOAD_NOT_SET: {
      break;
    }
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void MessageK2D::CopyFrom(const MessageK2D& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:uart.comm.MessageK2D)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MessageK2D::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_impl_._has_bits_)) return false;
  switch (payload_case()) {
    case kNop: {
      break;
    }
    case kFnState: {
      if (_internal_has_fn_state()) {
        if (!_impl_.payload_.fn_state_->IsInitialized()) return false;
      }
      break;
    }
    case PAYLOAD_NOT_SET: {
      break;
    }
  }
  return true;
}

void MessageK2D::InternalSwap(MessageK2D* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  swap(_impl_.action_, other->_impl_.action_);
  swap(_impl_.payload_, other->_impl_.payload_);
  swap(_impl_._oneof_case_[0], other->_impl_._oneof_case_[0]);
}

::PROTOBUF_NAMESPACE_ID::Metadata MessageK2D::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_uart_5fcomm_2eproto_getter, &descriptor_table_uart_5fcomm_2eproto_once,
      file_level_metadata_uart_5fcomm_2eproto[0]);
}

// ===================================================================

class Nop::_Internal {
 public:
};

Nop::Nop(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase(arena, is_message_owned) {
  // @@protoc_insertion_point(arena_constructor:uart.comm.Nop)
}
Nop::Nop(const Nop& from)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase() {
  Nop* const _this = this; (void)_this;
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:uart.comm.Nop)
}





const ::PROTOBUF_NAMESPACE_ID::Message::ClassData Nop::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::CopyImpl,
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::MergeImpl,
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*Nop::GetClassData() const { return &_class_data_; }







::PROTOBUF_NAMESPACE_ID::Metadata Nop::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_uart_5fcomm_2eproto_getter, &descriptor_table_uart_5fcomm_2eproto_once,
      file_level_metadata_uart_5fcomm_2eproto[1]);
}

// ===================================================================

class FnState::_Internal {
 public:
  using HasBits = decltype(std::declval<FnState>()._impl_._has_bits_);
  static void set_has_pressed(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000001) ^ 0x00000001) != 0;
  }
};

FnState::FnState(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:uart.comm.FnState)
}
FnState::FnState(const FnState& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  FnState* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){from._impl_._has_bits_}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.pressed_){}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.pressed_ = from._impl_.pressed_;
  // @@protoc_insertion_point(copy_constructor:uart.comm.FnState)
}

inline void FnState::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.pressed_){false}
  };
}

FnState::~FnState() {
  // @@protoc_insertion_point(destructor:uart.comm.FnState)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void FnState::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void FnState::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void FnState::Clear() {
// @@protoc_insertion_point(message_clear_start:uart.comm.FnState)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.pressed_ = false;
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* FnState::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // required bool pressed = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _Internal::set_has_pressed(&has_bits);
          _impl_.pressed_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  _impl_._has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* FnState::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:uart.comm.FnState)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  // required bool pressed = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteBoolToArray(1, this->_internal_pressed(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:uart.comm.FnState)
  return target;
}

size_t FnState::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:uart.comm.FnState)
  size_t total_size = 0;

  // required bool pressed = 1;
  if (_internal_has_pressed()) {
    total_size += 1 + 1;
  }
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData FnState::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    FnState::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*FnState::GetClassData() const { return &_class_data_; }


void FnState::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<FnState*>(&to_msg);
  auto& from = static_cast<const FnState&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:uart.comm.FnState)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_pressed()) {
    _this->_internal_set_pressed(from._internal_pressed());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void FnState::CopyFrom(const FnState& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:uart.comm.FnState)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FnState::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_impl_._has_bits_)) return false;
  return true;
}

void FnState::InternalSwap(FnState* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  swap(_impl_.pressed_, other->_impl_.pressed_);
}

::PROTOBUF_NAMESPACE_ID::Metadata FnState::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_uart_5fcomm_2eproto_getter, &descriptor_table_uart_5fcomm_2eproto_once,
      file_level_metadata_uart_5fcomm_2eproto[2]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace comm
}  // namespace uart
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::uart::comm::MessageK2D*
Arena::CreateMaybeMessage< ::uart::comm::MessageK2D >(Arena* arena) {
  return Arena::CreateMessageInternal< ::uart::comm::MessageK2D >(arena);
}
template<> PROTOBUF_NOINLINE ::uart::comm::Nop*
Arena::CreateMaybeMessage< ::uart::comm::Nop >(Arena* arena) {
  return Arena::CreateMessageInternal< ::uart::comm::Nop >(arena);
}
template<> PROTOBUF_NOINLINE ::uart::comm::FnState*
Arena::CreateMaybeMessage< ::uart::comm::FnState >(Arena* arena) {
  return Arena::CreateMessageInternal< ::uart::comm::FnState >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
