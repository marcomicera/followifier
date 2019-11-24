/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: message.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "message.pb-c.h"
void   followifier__esp32_metadata__init
                     (Followifier__ESP32Metadata         *message)
{
  static const Followifier__ESP32Metadata init_value = FOLLOWIFIER__ESP32_METADATA__INIT;
  *message = init_value;
}
size_t followifier__esp32_metadata__get_packed_size
                     (const Followifier__ESP32Metadata *message)
{
  assert(message->base.descriptor == &followifier__esp32_metadata__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t followifier__esp32_metadata__pack
                     (const Followifier__ESP32Metadata *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &followifier__esp32_metadata__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t followifier__esp32_metadata__pack_to_buffer
                     (const Followifier__ESP32Metadata *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &followifier__esp32_metadata__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Followifier__ESP32Metadata *
       followifier__esp32_metadata__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Followifier__ESP32Metadata *)
     protobuf_c_message_unpack (&followifier__esp32_metadata__descriptor,
                                allocator, len, data);
}
void   followifier__esp32_metadata__free_unpacked
                     (Followifier__ESP32Metadata *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &followifier__esp32_metadata__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   followifier__esp32_message__init
                     (Followifier__ESP32Message         *message)
{
  static const Followifier__ESP32Message init_value = FOLLOWIFIER__ESP32_MESSAGE__INIT;
  *message = init_value;
}
size_t followifier__esp32_message__get_packed_size
                     (const Followifier__ESP32Message *message)
{
  assert(message->base.descriptor == &followifier__esp32_message__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t followifier__esp32_message__pack
                     (const Followifier__ESP32Message *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &followifier__esp32_message__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t followifier__esp32_message__pack_to_buffer
                     (const Followifier__ESP32Message *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &followifier__esp32_message__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Followifier__ESP32Message *
       followifier__esp32_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Followifier__ESP32Message *)
     protobuf_c_message_unpack (&followifier__esp32_message__descriptor,
                                allocator, len, data);
}
void   followifier__esp32_message__free_unpacked
                     (Followifier__ESP32Message *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &followifier__esp32_message__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   followifier__batch__init
                     (Followifier__Batch         *message)
{
  static const Followifier__Batch init_value = FOLLOWIFIER__BATCH__INIT;
  *message = init_value;
}
size_t followifier__batch__get_packed_size
                     (const Followifier__Batch *message)
{
  assert(message->base.descriptor == &followifier__batch__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t followifier__batch__pack
                     (const Followifier__Batch *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &followifier__batch__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t followifier__batch__pack_to_buffer
                     (const Followifier__Batch *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &followifier__batch__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Followifier__Batch *
       followifier__batch__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Followifier__Batch *)
     protobuf_c_message_unpack (&followifier__batch__descriptor,
                                allocator, len, data);
}
void   followifier__batch__free_unpacked
                     (Followifier__Batch *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &followifier__batch__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor followifier__esp32_metadata__field_descriptors[4] =
{
  {
    "deviceMac",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Followifier__ESP32Metadata, devicemac),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "ssid",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Followifier__ESP32Metadata, ssid),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "timestamp",
    3,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_INT64,
    0,   /* quantifier_offset */
    offsetof(Followifier__ESP32Metadata, timestamp),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rssi",
    5,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Followifier__ESP32Metadata, rssi),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned followifier__esp32_metadata__field_indices_by_name[] = {
  0,   /* field[0] = deviceMac */
  3,   /* field[3] = rssi */
  1,   /* field[1] = ssid */
  2,   /* field[2] = timestamp */
};
static const ProtobufCIntRange followifier__esp32_metadata__number_ranges[2 + 1] =
{
  { 1, 0 },
  { 5, 3 },
  { 0, 4 }
};
const ProtobufCMessageDescriptor followifier__esp32_metadata__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "followifier.ESP32Metadata",
  "ESP32Metadata",
  "Followifier__ESP32Metadata",
  "followifier",
  sizeof(Followifier__ESP32Metadata),
  4,
  followifier__esp32_metadata__field_descriptors,
  followifier__esp32_metadata__field_indices_by_name,
  2,  followifier__esp32_metadata__number_ranges,
  (ProtobufCMessageInit) followifier__esp32_metadata__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor followifier__esp32_message__field_descriptors[2] =
{
  {
    "metadata",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Followifier__ESP32Message, metadata),
    &followifier__esp32_metadata__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "frame_hash",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_BYTES,
    0,   /* quantifier_offset */
    offsetof(Followifier__ESP32Message, frame_hash),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned followifier__esp32_message__field_indices_by_name[] = {
  1,   /* field[1] = frame_hash */
  0,   /* field[0] = metadata */
};
static const ProtobufCIntRange followifier__esp32_message__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor followifier__esp32_message__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "followifier.ESP32Message",
  "ESP32Message",
  "Followifier__ESP32Message",
  "followifier",
  sizeof(Followifier__ESP32Message),
  2,
  followifier__esp32_message__field_descriptors,
  followifier__esp32_message__field_indices_by_name,
  1,  followifier__esp32_message__number_ranges,
  (ProtobufCMessageInit) followifier__esp32_message__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor followifier__batch__field_descriptors[2] =
{
  {
    "boardMac",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Followifier__Batch, boardmac),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "messages",
    2,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Followifier__Batch, n_messages),
    offsetof(Followifier__Batch, messages),
    &followifier__esp32_message__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned followifier__batch__field_indices_by_name[] = {
  0,   /* field[0] = boardMac */
  1,   /* field[1] = messages */
};
static const ProtobufCIntRange followifier__batch__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor followifier__batch__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "followifier.Batch",
  "Batch",
  "Followifier__Batch",
  "followifier",
  sizeof(Followifier__Batch),
  2,
  followifier__batch__field_descriptors,
  followifier__batch__field_indices_by_name,
  1,  followifier__batch__number_ranges,
  (ProtobufCMessageInit) followifier__batch__init,
  NULL,NULL,NULL    /* reserved[123] */
};
