#include "ftpBridge.hpp"
#include <typeinfo>
#include <string.h>
#include <iostream>

FtpBridge::FtpBridge(std::shared_ptr<boost::asio::io_context> io_context,
                       HttpSocketType& client_socket)
  : Bridge(io_context)
{
    set_client_socket(std::make_shared<HttpSocketType>(std::move(client_socket)));
    client_host_ = boost::lexical_cast<std::string>( client_socket_->remote_endpoint());

}

void FtpBridge::start_by_connect(char client_buffer [max_data_length],
                                                    const boost::system::error_code& error,
                                                    std::size_t bytes_transferred,
                                                    endpoint_type endpoint,
                                                    const std::string& domain)
{
    // save the message  
    strncpy(client_buffer_, client_buffer, max_data_length);

    Logger::log(
        "3) Attempting to connect to " + domain + 
        " [C] " + boost::lexical_cast<std::string>(client_socket_->lowest_layer().remote_endpoint()),
        Logger::LOG_LEVEL::INFO
    );

    // Create a new server socket and insert to the map for future reuse
    std::shared_ptr<HttpSocketType> new_server_socket = std::make_shared<HttpSocketType>(*io_context_);
    server_socket_map_[boost::lexical_cast<std::string>(endpoint)] = new_server_socket;
    std::cout << boost::lexical_cast<std::string>(endpoint) << "shay10" << std::endl;
    boost::system::error_code error1 = boost::asio::error::host_not_found;
    boost::asio::streambuf response;
    std::istream response_stream(&response);
    boost::asio::streambuf request;
    std::ostream request_stream(&request);

    new_server_socket->close();
    new_server_socket->connect(endpoint, error1);
    std::size_t n = boost::asio::read_until(*new_server_socket, response, "\r\n");
    std::string status_message;
    std::cout  << &response <<  std::endl;
    strcpy(this->client_buffer_, "USER anonymous\r\n");
    std::string data = "USER anonymous\r\n";
    new_server_socket->write_some(boost::asio::buffer(data, bytes_transferred));
    // boost::asio::write(*new_server_socket, request);

    // new_server_socket->write_some(boost::asio::buffer(, bytes_transferred));
    // std::string data;
    
    n = boost::asio::read_until(*new_server_socket, response, "\r\n");
    
        
    std::cout  << &response <<  std::endl;

    data = "PASS\r\n";
    new_server_socket->write_some(boost::asio::buffer(data, bytes_transferred));
    n = boost::asio::read_until(*new_server_socket, response, "\r\n");
    std::cout << this->client_buffer_;
    std::cout << &response << "shaya1" << std::endl;


    data = "SYST\r\n";
    new_server_socket->write_some(boost::asio::buffer(data , bytes_transferred));
    n = boost::asio::read_until(*new_server_socket, response, "\r\n");
    std::cout << data;
    std::cout  << &response << "shaya2" << std::endl;






    data = "PASV\r\n";
    new_server_socket->write_some(boost::asio::buffer(data, bytes_transferred));
    n = boost::asio::read_until(*new_server_socket, response, "\r\n");
    std::string tmp;
    getline(response_stream, tmp);
    std::cout << "shaya3" << std::endl;
    // std::cout << &response << std::endl;
    std::cout << tmp << std::endl;
    std::cout << "shaya4" << std::endl;
    std::cout << data << "done reading" << std::endl;
    std::regex regex_ip(".+\\(([0-9]{1,}),([0-9]{1,}),([0-9]{1,}),([0-9]{1,}),([0-9]{1,}),([0-9]{1,})\\).*");
    std::smatch match;
    std::regex_search(tmp, match, regex_ip);
    std::cout << match[0] << " " << match[1] << " " << match[2] << " " << match[3] << " " << match[4] << " " << match[5] << " " << match[6] << std::endl;
    std::string sipaddress = match[1].str() + "." + match[2].str() + "." + match[3].str() + "." + match[4].str();
    unsigned int itmp1 = boost::lexical_cast<unsigned int>(match[5]);
    unsigned int itmp2 = boost::lexical_cast<unsigned int>(match[6]);
    std::string sport = boost::lexical_cast<std::string>(itmp1*256+itmp2);
    boost::asio::ip::tcp::resolver resolver( *io_context_ );
    boost::asio::ip::tcp::resolver::query query_trsans(sipaddress, sport);
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator_trsans = resolver.resolve(query_trsans);

    boost::asio::ip::tcp::endpoint endpoint1 = endpoint_iterator_trsans->endpoint();
    std::shared_ptr<HttpSocketType> socket_trsans = std::make_shared<HttpSocketType>(*io_context_);
    boost::system::error_code error2 = boost::asio::error::host_not_found;
    socket_trsans->connect(endpoint1, error2);

    data = "LIST\r\n";
    new_server_socket->write_some(boost::asio::buffer(data, bytes_transferred));
    n = boost::asio::read_until(*new_server_socket, response, "\r\n");
    std::cout << data;
    std::cout  << &response << std::endl;

    std::ostringstream ss;
    std::string data1;
    data1.resize(8128);
    n = socket_trsans->read_some(boost::asio::buffer(data1, max_data_length));
    std::cout << data1 << std::endl << " shay data" << std::endl;
    
    // std::cout << &response;
    
    // ss << &response;
    // data = ss.str();
    // std::cout << "shay avivi a" << std::endl;
    // std::cout << data << std::endl;
    
    // data = "";
    // socket_trsans->read_some(boost::asio::buffer(data, bytes_transferred));
    // std::cout << "shay avivi" << std::endl << data << std::endl;
    // std::cout << "shay avivi1" << std::endl << &response << std::endl;
    this->client_socket_->write_some(boost::asio::buffer(data1, n));

    data = "QUIT\r\n";
    new_server_socket->write_some(boost::asio::buffer(data, bytes_transferred));
    n = boost::asio::read_until(*new_server_socket, response, "\r\n");
    std::cout << data;
    std::cout  << &response << std::endl;
    // std::shared_ptr<HttpSocketType> new_server_socket1 = std::make_shared<HttpSocketType>(*io_context_);
    // strcpy(this->client_buffer_, "LIST\r\nQUIT\r\n");
    // std::cout << this->client_buffer_ << std::endl;
    // new_server_socket1->close();
    // new_server_socket1->connect(endpoint, error1);
    // new_server_socket1->write_some(boost::asio::buffer(client_buffer_, bytes_transferred));
    // new_server_socket->read_some(boost::asio::buffer(server_buffer_,max_data_length));
    // std::cout << client_buffer_ << std::endl;
    // std::cout << server_buffer_ << std::endl;
    // memset(this->client_buffer_, 0, (sizeof this->client_buffer_) );
    // strcpy(this->client_buffer_, "PASS mozilla@example.com\r\n");
    // new_server_socket->write_some(boost::asio::buffer(client_buffer_, bytes_transferred));
    // new_server_socket->read_some(boost::asio::buffer(server_buffer_,max_data_length));
    // std::cout << client_buffer_ << std::endl;
    // std::cout << server_buffer_ << std::endl;
    // memset(this->client_buffer_, 0, sizeof (this->client_buffer_) );
    // strcpy(this->client_buffer_, "SYST\r\n");
    // new_server_socket->write_some(boost::asio::buffer(client_buffer_, bytes_transferred));
    // new_server_socket->read_some(boost::asio::buffer(server_buffer_,max_data_length));
    // std::cout << client_buffer_ << std::endl;
    // std::cout << server_buffer_ << std::endl;
    // memset(this->client_buffer_, 0, sizeof (this->client_buffer_) );
    // strcpy(this->client_buffer_, "PWD\r\n");
    // new_server_socket->write_some(boost::asio::buffer(client_buffer_, bytes_transferred));
    // new_server_socket->read_some(boost::asio::buffer(server_buffer_,max_data_length));
    // std::cout << client_buffer_ << std::endl;
    // std::cout << server_buffer_ << std::endl;
    // memset(this->client_buffer_, 0, sizeof (this->client_buffer_) );
    // strcpy(this->client_buffer_, "TYPE I\r\n");
    // new_server_socket->write_some(boost::asio::buffer(client_buffer_, bytes_transferred));
    // new_server_socket->read_some(boost::asio::buffer(server_buffer_,max_data_length));
    // std::cout << client_buffer_ << std::endl;
    // std::cout << server_buffer_ << std::endl;
    // memset(this->client_buffer_, 0, sizeof (this->client_buffer_) );
    // strcpy(this->client_buffer_, "SIZE /\r\n");
    // new_server_socket->write_some(boost::asio::buffer(client_buffer_, bytes_transferred));
    // new_server_socket->read_some(boost::asio::buffer(server_buffer_,max_data_length));
    // std::cout << client_buffer_ << std::endl;
    // std::cout << server_buffer_ << std::endl;
    // memset(this->client_buffer_, 0, sizeof (this->client_buffer_) );
    // strcpy(this->client_buffer_, "CWD /\r\n");
    // new_server_socket->write_some(boost::asio::buffer(client_buffer_, bytes_transferred));
    // new_server_socket->read_some(boost::asio::buffer(server_buffer_,max_data_length));
    // std::cout << client_buffer_ << std::endl;
    // std::cout << server_buffer_ << std::endl;
    // memset(this->client_buffer_, 0, sizeof (this->client_buffer_) );
    // strcpy(this->client_buffer_, "PASV\r\n");
    // new_server_socket->write_some(boost::asio::buffer(client_buffer_, bytes_transferred));
    // new_server_socket->read_some(boost::asio::buffer(server_buffer_,max_data_length));
    // std::cout << client_buffer_ << std::endl;
    // std::cout << server_buffer_ << std::endl;
    // memset(this->client_buffer_, 0, sizeof (this->client_buffer_) );
    // strcpy(this->client_buffer_, "LIST\r\n");
    // new_server_socket->write_some(boost::asio::buffer(client_buffer_, bytes_transferred));
    // new_server_socket->read_some(boost::asio::buffer(server_buffer_,max_data_length));
    // std::cout << client_buffer_ << std::endl;
    // std::cout << server_buffer_ << std::endl;
    // memset(this->client_buffer_, 0, sizeof (this->client_buffer_) );
    // strcpy(this->client_buffer_, "QUIT\r\n");
    // new_server_socket->write_some(boost::asio::buffer(client_buffer_, bytes_transferred));
    // new_server_socket->read_some(boost::asio::buffer(server_buffer_,max_data_length));
    // std::cout << client_buffer_ << std::endl;
    // std::cout << server_buffer_ << std::endl;
    // std::cout << "socket connected: " << new_server_socket->is_open() << " shay11" << std::endl;
    // std::cout << typeid(*endpoint_iterator).name() << std::endl;
    // std::cout << typeid(endpoint).name() << std::endl;

    // this->handle_server_connect(new_server_socket, error1, bytes_transferred, boost::lexical_cast<std::string>(*(endpoint_iterator - 1))
    // new_server_socket->connect(, resolver.resolve(query));
    // boost::bind(
    //         &Bridge::handle_server_connect,
    //         this->shared_from_this(),
    //         new_server_socket,
    //         error,
    //         bytes_transferred,
    //         boost::lexical_cast<std::string>(endpoint));
    //         x.;

    // new_server_socket->lowest_layer().async_connect(
    //     endpoint,
    //     boost::bind(
    //         &Bridge::handle_server_connect,
    //         this->shared_from_this(),
    //         new_server_socket,
    //         error,
    //         bytes_transferred,
    //         boost::lexical_cast<std::string>(endpoint_iterator->endpoint())
    //     )
    // ); 
}

void FtpBridge::do_handshake(std::shared_ptr<HttpSocketType> socket,
                               boost::asio::ssl::stream_base::handshake_type handshake_type)
{
    return;
}

HttpSocketType& FtpBridge::get_actual_socket(HttpSocketType& socket)
{
  return socket;
}

std::shared_ptr<HttpSocketType> FtpBridge::create_new_server_socket()
{
  return std::make_shared<HttpSocketType>(*io_context_);
}