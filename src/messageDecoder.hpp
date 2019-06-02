#ifndef MESSAGEDECODER_HPP_
#define MESSAGEDECODER_HPP_

#include <unordered_map>
#include <vector>
#include <google/protobuf/text_format.h>
#include "../messages/messagesHeaders.hpp"
#include <boost/property_tree/json_parser.hpp>

class MessageDecoder
{

public:
MessageDecoder(const std::string& file_path = "include/messagesClassName.json");
std::vector<std::string> get_decoded_messages(const std::string& data);
bool isProtoMessage(const std::string& data);

private:
std::vector<std::string> _classes_names;
};

#endif 