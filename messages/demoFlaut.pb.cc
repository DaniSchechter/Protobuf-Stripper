// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: demoFlaut.proto

#include "demoFlaut.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

namespace demo {
class TaskFlautDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<TaskFlaut> _instance;
} _TaskFlaut_default_instance_;
}  // namespace demo
static void InitDefaultsTaskFlaut_demoFlaut_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::demo::_TaskFlaut_default_instance_;
    new (ptr) ::demo::TaskFlaut();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::demo::TaskFlaut::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_TaskFlaut_demoFlaut_2eproto =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsTaskFlaut_demoFlaut_2eproto}, {}};

void InitDefaults_demoFlaut_2eproto() {
  ::google::protobuf::internal::InitSCC(&scc_info_TaskFlaut_demoFlaut_2eproto.base);
}

::google::protobuf::Metadata file_level_metadata_demoFlaut_2eproto[1];
constexpr ::google::protobuf::EnumDescriptor const** file_level_enum_descriptors_demoFlaut_2eproto = nullptr;
constexpr ::google::protobuf::ServiceDescriptor const** file_level_service_descriptors_demoFlaut_2eproto = nullptr;

const ::google::protobuf::uint32 TableStruct_demoFlaut_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::demo::TaskFlaut, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::demo::TaskFlaut, arr_),
  PROTOBUF_FIELD_OFFSET(::demo::TaskFlaut, done_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::demo::TaskFlaut)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::demo::_TaskFlaut_default_instance_),
};

::google::protobuf::internal::AssignDescriptorsTable assign_descriptors_table_demoFlaut_2eproto = {
  {}, AddDescriptors_demoFlaut_2eproto, "demoFlaut.proto", schemas,
  file_default_instances, TableStruct_demoFlaut_2eproto::offsets,
  file_level_metadata_demoFlaut_2eproto, 1, file_level_enum_descriptors_demoFlaut_2eproto, file_level_service_descriptors_demoFlaut_2eproto,
};

const char descriptor_table_protodef_demoFlaut_2eproto[] =
  "\n\017demoFlaut.proto\022\004demo\"*\n\tTaskFlaut\022\017\n\003"
  "arr\030\001 \003(\002B\002\020\001\022\014\n\004done\030\002 \001(\010b\006proto3"
  ;
::google::protobuf::internal::DescriptorTable descriptor_table_demoFlaut_2eproto = {
  false, InitDefaults_demoFlaut_2eproto, 
  descriptor_table_protodef_demoFlaut_2eproto,
  "demoFlaut.proto", &assign_descriptors_table_demoFlaut_2eproto, 75,
};

void AddDescriptors_demoFlaut_2eproto() {
  static constexpr ::google::protobuf::internal::InitFunc deps[1] =
  {
  };
 ::google::protobuf::internal::AddDescriptors(&descriptor_table_demoFlaut_2eproto, deps, 0);
}

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_demoFlaut_2eproto = []() { AddDescriptors_demoFlaut_2eproto(); return true; }();
namespace demo {

// ===================================================================

void TaskFlaut::InitAsDefaultInstance() {
}
class TaskFlaut::HasBitSetters {
 public:
};

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int TaskFlaut::kArrFieldNumber;
const int TaskFlaut::kDoneFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

TaskFlaut::TaskFlaut()
  : ::google::protobuf::Message(), _internal_metadata_(nullptr) {
  SharedCtor();
  // @@protoc_insertion_point(constructor:demo.TaskFlaut)
}
TaskFlaut::TaskFlaut(const TaskFlaut& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(nullptr),
      arr_(from.arr_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  done_ = from.done_;
  // @@protoc_insertion_point(copy_constructor:demo.TaskFlaut)
}

void TaskFlaut::SharedCtor() {
  done_ = false;
}

TaskFlaut::~TaskFlaut() {
  // @@protoc_insertion_point(destructor:demo.TaskFlaut)
  SharedDtor();
}

void TaskFlaut::SharedDtor() {
}

void TaskFlaut::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const TaskFlaut& TaskFlaut::default_instance() {
  ::google::protobuf::internal::InitSCC(&::scc_info_TaskFlaut_demoFlaut_2eproto.base);
  return *internal_default_instance();
}


void TaskFlaut::Clear() {
// @@protoc_insertion_point(message_clear_start:demo.TaskFlaut)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  arr_.Clear();
  done_ = false;
  _internal_metadata_.Clear();
}

#if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
const char* TaskFlaut::_InternalParse(const char* begin, const char* end, void* object,
                  ::google::protobuf::internal::ParseContext* ctx) {
  auto msg = static_cast<TaskFlaut*>(object);
  ::google::protobuf::int32 size; (void)size;
  int depth; (void)depth;
  ::google::protobuf::uint32 tag;
  ::google::protobuf::internal::ParseFunc parser_till_end; (void)parser_till_end;
  auto ptr = begin;
  while (ptr < end) {
    ptr = ::google::protobuf::io::Parse32(ptr, &tag);
    GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
    switch (tag >> 3) {
      // repeated float arr = 1 [packed = true];
      case 1: {
        if (static_cast<::google::protobuf::uint8>(tag) == 10) {
          ptr = ::google::protobuf::io::ReadSize(ptr, &size);
          GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
          parser_till_end = ::google::protobuf::internal::PackedFloatParser;
          object = msg->mutable_arr();
          if (size > end - ptr) goto len_delim_till_end;
          auto newend = ptr + size;
          if (size) ptr = parser_till_end(ptr, newend, object, ctx);
          GOOGLE_PROTOBUF_PARSER_ASSERT(ptr == newend);
          break;
        } else if (static_cast<::google::protobuf::uint8>(tag) != 13) goto handle_unusual;
        do {
          msg->add_arr(::google::protobuf::io::UnalignedLoad<float>(ptr));
          ptr += sizeof(float);
          if (ptr >= end) break;
        } while ((::google::protobuf::io::UnalignedLoad<::google::protobuf::uint64>(ptr) & 255) == 13 && (ptr += 1));
        break;
      }
      // bool done = 2;
      case 2: {
        if (static_cast<::google::protobuf::uint8>(tag) != 16) goto handle_unusual;
        msg->set_done(::google::protobuf::internal::ReadVarint(&ptr));
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr);
        break;
      }
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->EndGroup(tag);
          return ptr;
        }
        auto res = UnknownFieldParse(tag, {_InternalParse, msg},
          ptr, end, msg->_internal_metadata_.mutable_unknown_fields(), ctx);
        ptr = res.first;
        GOOGLE_PROTOBUF_PARSER_ASSERT(ptr != nullptr);
        if (res.second) return ptr;
      }
    }  // switch
  }  // while
  return ptr;
len_delim_till_end:
  return ctx->StoreAndTailCall(ptr, end, {_InternalParse, msg},
                               {parser_till_end, object}, size);
}
#else  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
bool TaskFlaut::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!PROTOBUF_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:demo.TaskFlaut)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated float arr = 1 [packed = true];
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (10 & 0xFF)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, this->mutable_arr())));
        } else if (static_cast< ::google::protobuf::uint8>(tag) == (13 & 0xFF)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitiveNoInline<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 1, 10u, input, this->mutable_arr())));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // bool done = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) == (16 & 0xFF)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &done_)));
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
  // @@protoc_insertion_point(parse_success:demo.TaskFlaut)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:demo.TaskFlaut)
  return false;
#undef DO_
}
#endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER

void TaskFlaut::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:demo.TaskFlaut)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated float arr = 1 [packed = true];
  if (this->arr_size() > 0) {
    ::google::protobuf::internal::WireFormatLite::WriteTag(1, ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED, output);
    output->WriteVarint32(_arr_cached_byte_size_.load(
        std::memory_order_relaxed));
    ::google::protobuf::internal::WireFormatLite::WriteFloatArray(
      this->arr().data(), this->arr_size(), output);
  }

  // bool done = 2;
  if (this->done() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(2, this->done(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:demo.TaskFlaut)
}

::google::protobuf::uint8* TaskFlaut::InternalSerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:demo.TaskFlaut)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated float arr = 1 [packed = true];
  if (this->arr_size() > 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteTagToArray(
      1,
      ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
      target);
    target = ::google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(
        _arr_cached_byte_size_.load(std::memory_order_relaxed),
         target);
    target = ::google::protobuf::internal::WireFormatLite::
      WriteFloatNoTagToArray(this->arr_, target);
  }

  // bool done = 2;
  if (this->done() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(2, this->done(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:demo.TaskFlaut)
  return target;
}

size_t TaskFlaut::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:demo.TaskFlaut)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated float arr = 1 [packed = true];
  {
    unsigned int count = static_cast<unsigned int>(this->arr_size());
    size_t data_size = 4UL * count;
    if (data_size > 0) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
            static_cast<::google::protobuf::int32>(data_size));
    }
    int cached_size = ::google::protobuf::internal::ToCachedSize(data_size);
    _arr_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // bool done = 2;
  if (this->done() != 0) {
    total_size += 1 + 1;
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void TaskFlaut::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:demo.TaskFlaut)
  GOOGLE_DCHECK_NE(&from, this);
  const TaskFlaut* source =
      ::google::protobuf::DynamicCastToGenerated<TaskFlaut>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:demo.TaskFlaut)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:demo.TaskFlaut)
    MergeFrom(*source);
  }
}

void TaskFlaut::MergeFrom(const TaskFlaut& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:demo.TaskFlaut)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  arr_.MergeFrom(from.arr_);
  if (from.done() != 0) {
    set_done(from.done());
  }
}

void TaskFlaut::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:demo.TaskFlaut)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void TaskFlaut::CopyFrom(const TaskFlaut& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:demo.TaskFlaut)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool TaskFlaut::IsInitialized() const {
  return true;
}

void TaskFlaut::Swap(TaskFlaut* other) {
  if (other == this) return;
  InternalSwap(other);
}
void TaskFlaut::InternalSwap(TaskFlaut* other) {
  using std::swap;
  _internal_metadata_.Swap(&other->_internal_metadata_);
  arr_.InternalSwap(&other->arr_);
  swap(done_, other->done_);
}

::google::protobuf::Metadata TaskFlaut::GetMetadata() const {
  ::google::protobuf::internal::AssignDescriptors(&::assign_descriptors_table_demoFlaut_2eproto);
  return ::file_level_metadata_demoFlaut_2eproto[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace demo
namespace google {
namespace protobuf {
template<> PROTOBUF_NOINLINE ::demo::TaskFlaut* Arena::CreateMaybeMessage< ::demo::TaskFlaut >(Arena* arena) {
  return Arena::CreateInternal< ::demo::TaskFlaut >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
