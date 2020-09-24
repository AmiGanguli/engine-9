#include <functional>
#include <iostream>

#include <spdlog/spdlog.h>
#include <docopt/docopt.h>

#include <fcgio.h>

static constexpr auto USAGE =
  R"(Engine9

    Usage:
          engine9
          engine9 (-h | --help)
          engine9 --version
    Options:
          -h --help     Show this screen.
          --version     Show version.
)";

int main(int argc, const char **argv)
{
  std::map<std::string, docopt::value> args = docopt::docopt(
    USAGE,
    { std::next(argv), std::next(argv, argc) },
    true,// show help if requested
    "Engine9");// version string

  for (auto const &arg : args) {
    std::cout << arg.first << arg.second << std::endl;
  }

  spdlog::info("Starting FASTCGI server!");

  std::streambuf *cin_streambuf = std::cin.rdbuf();
  std::streambuf *cout_streambuf = std::cout.rdbuf();
  std::streambuf *cerr_streambuf = std::cerr.rdbuf();

  FCGX_Request request;

  FCGX_Init();
  FCGX_InitRequest(&request, 0, 0);

  while (FCGX_Accept_r(&request) == 0) {
    fcgi_streambuf cin_fcgi_streambuf(request.in);
    fcgi_streambuf cout_fcgi_streambuf(request.out);
    fcgi_streambuf cerr_fcgi_streambuf(request.err);

    std::cin.rdbuf(&cin_fcgi_streambuf);
    std::cout.rdbuf(&cout_fcgi_streambuf);
    std::cerr.rdbuf(&cerr_fcgi_streambuf);

    std::cout << "Content-type: text/html\r\n"
              << "\r\n"
              << "<html>\n"
              << "  <head>\n"
              << "    <title>Hello, World!</title>\n"
              << "  </head>\n"
              << "  <body>\n"
              << "    <h1>Hello, World!</h1>\n"
              << "  </body>\n"
              << "</html>\n";

    // Note: the fcgi_streambuf destructor will auto flush
  }

  // restore stdio streambufs
  std::cin.rdbuf(cin_streambuf);
  std::cout.rdbuf(cout_streambuf);
  std::cerr.rdbuf(cerr_streambuf);

  return 0;
}
