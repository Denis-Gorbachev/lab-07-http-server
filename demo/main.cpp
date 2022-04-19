#include <server.hpp>
#include <memory>
#include <thread>
#include <functional>
using tcp = boost::asio::ip::tcp;
namespace net = boost::asio;

// Performs an HTTP GET and prints the response
int main() {
  try
  {
    const boost::asio::ip::address_v4 address(2130706433);
    auto const port = 80;
    auto const doc_root = std::make_shared<std::string>("/v1/api/suggest");

    // The io_context is required for all I/O
    net::io_service ioc{1};

    // The acceptor receives incoming connections
    tcp::acceptor acceptor = {ioc, boost::asio::ip::tcp::endpoint{address, port}};
    for(;;)
    {
      // This will receive the new connection
      tcp::socket socket{ioc};

      // Block until we get a connection
      acceptor.accept(socket);

      // Launch the session, transferring ownership of the socket
      std::thread{std::bind(
                      &do_session,
                      std::move(socket))}.detach();
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}