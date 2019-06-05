#include "messageDecoder.hpp"

MessageDecoder::MessageDecoder(){
    this->discSourceTree_.MapPath("", ".");
}

std::vector<std::string> MessageDecoder::get_decoded_messages(const std::string& data){
    std::vector<std::string> decoded;
    //need to understand hoe to instanciate from string
    google::protobuf::compiler::Importer importer(&this->discSourceTree_, NULL);

    std::experimental::filesystem::path p = std::experimental::filesystem::u8path(u8"messages");
    for (const auto& entry : std::experimental::filesystem::directory_iterator(p)) {
        const google::protobuf::FileDescriptor* parsed_file = importer.Import(entry.path().c_str());
        for (int index = 0; index < parsed_file->message_type_count(); index++)
        {
            const google::protobuf::Descriptor* message_desc = parsed_file->message_type(index);
            const google::protobuf::Message* prototype_msg = factory_.GetPrototype(message_desc);
            google::protobuf::Message* mutable_msg = prototype_msg->New();
            mutable_msg->ParseFromString(data);
            decoded.push_back(mutable_msg->DebugString());
        }        
    }
    return decoded;
}

bool MessageDecoder::is_proto_message(const std::string& data){
    google::protobuf::UnknownFieldSet unknown;
    return unknown.ParseFromString(data);
}
