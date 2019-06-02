#include "messageDecoder.hpp"

MessageDecoder::MessageDecoder(const std::string& file_path){
    boost::property_tree::ptree classes_names_json_object;
    boost::property_tree::read_json(file_path, classes_names_json_object);
    // boost::property_tree::ptree listOfNames = classes_names_json_object.get_child("classesNames");
    for (boost::property_tree::ptree::value_type &className : classes_names_json_object.get_child("classesNames"))
    {
        std::string name = className.second.data();
        this->_classes_names.push_back(name);
    }
    
}

std::vector<std::string> MessageDecoder::get_decoded_messages(const std::string& data){
    std::vector<std::string> decoded;
    //need to understand hoe to instanciate from string
    demo::Task t1;
    demo::TaskFlaut tf1;
    demo::Person p1;
    
    try
    {
        std::cout << "shay1" << std::endl;
        std::string formatted;
        std::cout << t1.ParseFromString(data) << std::endl;
        std::string text = t1.text();
        std::cout << text << std::endl;
        google::protobuf::TextFormat::PrintToString(t1, &formatted);
        decoded.push_back(formatted);

        std::cout << "shay2" << std::endl;
        std::cout << tf1.ParseFromString(data) << std::endl ;
        google::protobuf::TextFormat::PrintToString(tf1, &formatted);
        decoded.push_back(formatted);

        std::cout << "shay3" << std::endl;
        std::cout << p1.ParseFromString(data) << std::endl;
        google::protobuf::TextFormat::PrintToString(p1, &formatted);
        decoded.push_back(formatted);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    return decoded;
}

bool MessageDecoder::isProtoMessage(const std::string& data){
    demo::Task t1;
    return t1.ParseFromString(data);
}

