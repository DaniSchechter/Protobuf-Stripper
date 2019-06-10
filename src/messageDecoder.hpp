#ifndef MESSAGE_DECODER_HPP_
#define MESSAGE_DECODER_HPP_

#include <vector>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include <experimental/filesystem>
#include <google/protobuf/unknown_field_set.h>
#include <numeric>
class MessageDecoder
{
public:

    MessageDecoder();
    void get_decoded_messages(std::string& data);
    bool is_proto_message(const std::string& data);

private:

    google::protobuf::compiler::DiskSourceTree discSourceTree_;
    google::protobuf::DynamicMessageFactory factory_;

};

#endif //MESSAGE_DECODER_HPP_