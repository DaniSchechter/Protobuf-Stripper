#ifndef MESSAGEDECODER_HPP_
#define MESSAGEDECODER_HPP_

#include <vector>
#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include <experimental/filesystem>
#include <google/protobuf/unknown_field_set.h>
class MessageDecoder
{

public:
MessageDecoder();
std::vector<std::string> get_decoded_messages(const std::string& data);
bool isProtoMessage(const std::string& data);

private:
google::protobuf::compiler::DiskSourceTree _discSourceTree;
google::protobuf::DynamicMessageFactory _factory;
};

#endif 