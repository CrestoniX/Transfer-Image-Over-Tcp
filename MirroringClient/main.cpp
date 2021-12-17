#include <iostream>
#include <boost/asio.hpp>
#include <functional>
#include <optional>
#include <unordered_set>
#include <fstream>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/cvconfig.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/core/types_c.h>
#include <vector>
#include <cstring>
#include <memory>

namespace io = boost::asio;
namespace ip = io::ip;
using tcp = ip::tcp;
using error_code = boost::system::error_code;
using namespace std::placeholders;

class application
{
public:
    application(io::io_context& io_context, std::string const& hostname, std::string const& port, std::string const& path_to_input_ph, std::string const& path_to_output_ph)
            : resolver(io_context)
            , socket  (io_context)
            , path_for_response(path_to_output_ph)
    {
        ImgBuf.resize(100000);
        srcmat = cv::imread(path_to_input_ph, 1);
        cv::imencode(".jpg", srcmat, ImgBuf);
        ImgBuf.push_back('.');
        ImgBuf.push_back('E');
        ImgBuf.push_back('O');
        ImgBuf.push_back('I');
        ImgBuf.push_back('.');
        auto view = streambuf_for_request.prepare(ImgBuf.size());
        std::memcpy(view.data(), ImgBuf.data(), ImgBuf.size());
        streambuf_for_request.commit(ImgBuf.size());
        resolver.async_resolve(hostname, port, std::bind(&application::on_resolve, this, _1, _2));
    }

private:

    void on_resolve(error_code error, tcp::resolver::results_type results)
    {
        std::cout << "Resolve: " << error.message() << "\n";
        io::async_connect(socket, results, std::bind(&application::on_connect, this, _1, _2));
    }

    void on_connect(error_code error, tcp::endpoint const& endpoint)
    {
        std::cout << "Connect: " << error.message() << ", endpoint: " << endpoint << "\n";
        io::async_write(socket, streambuf_for_request, boost::asio::transfer_all(), std::bind(&application::on_write, this, _1, _2));
    }

    void on_write(error_code error, std::size_t bytes_transferred)
    {
        std::cout << "Write: " << error.message() << ", bytes transferred: " << bytes_transferred << "\n";
        io::async_read(socket, boost::asio::dynamic_buffer(ImgBuf_for_response, 400000), std::bind(&application::on_read, this, _1, _2));
    }

    void on_read(error_code error, std::size_t bytes_transferred)
    {
        std::cout << "Bytes received: " << bytes_transferred << std::endl;
        srcmat = cv::imdecode(ImgBuf_for_response, 1);
        if(!ImgBuf_for_response.empty())
            cv::imwrite(path_for_response, srcmat);
    }

    tcp::resolver resolver;
    tcp::socket socket;
    io::streambuf streambuf_for_request;
    cv::Mat srcmat;
    std::vector<uchar> ImgBuf = {};
    std::vector<uchar> ImgBuf_for_response = {};
    std::string path_for_response;

};

int main(int argc, char* argv[]) {
    std::string server = argv[1];
    std::string port = argv[2];
    std::string path_to_input_ph = argv[3];
    std::string path_to_output_ph = argv[4];
    boost::asio::io_context io_context;
    application app(io_context, server,port,path_to_input_ph, path_to_output_ph);
    io_context.run();
    return 0;
}
