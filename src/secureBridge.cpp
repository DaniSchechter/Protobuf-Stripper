#include "secureBridge.hpp"

SecureBridge::SecureBridge(std::shared_ptr<boost::asio::io_context> io_context,
                           boost::asio::ssl::context& ssl_context)
  :  Bridge(io_context),
     ssl_stream_(*io_context, ssl_context)
     {
         
     }

