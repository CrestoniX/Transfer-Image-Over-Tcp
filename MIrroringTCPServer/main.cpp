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

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket &&socket) :
            _socket(std::move(socket)) {

    }
    void start_session() {
        std::cout << "Session started" << std::endl;
        async_read();
    }

private:
    boost::asio::ip::tcp::socket _socket;
    std::vector<uchar> ImgBuf;
    std::vector<uchar> ImgBuf_for_response;
    cv::Mat mat_for_read;
    cv::Mat mat_for_write;
    boost::asio::streambuf streambuf_for_response;

    void async_read() {
        std::cout << "Async_read: started receiving bytes" << std::endl;
        boost::asio::async_read_until(_socket, boost::asio::dynamic_buffer(ImgBuf, 400000),".EOI.",[self = shared_from_this()](boost::system::error_code errorCode,
                                                                                    std::uint16_t bytes_transfered) {
            if (!errorCode) {
                std::cout << "Bytes received: " << bytes_transfered << std::endl;
                self->mat_for_read = cv::imdecode(self->ImgBuf, 1);
                if(!self->ImgBuf.empty())
                cv::imwrite("/home/crestonix/CLionProjects/Gray_Image.jpg", self->mat_for_read);
                cv::flip(self->mat_for_read, self->mat_for_write, 1);
                cv::imwrite("/home/crestonix/CLionProjects/Flipped.jpg", self->mat_for_write);
                self->ImgBuf_for_response.resize(100000);
                cv::imencode(".jpg", self->mat_for_write, self->ImgBuf_for_response);
                self->ImgBuf_for_response.push_back('.');
                self->ImgBuf_for_response.push_back('E');
                self->ImgBuf_for_response.push_back('O');
                self->ImgBuf_for_response.push_back('I');
                self->ImgBuf_for_response.push_back('.');
                auto view = self->streambuf_for_response.prepare(self->ImgBuf_for_response.size());
                std::memcpy(view.data(), self->ImgBuf_for_response.data(), self->ImgBuf_for_response.size());
                self->streambuf_for_response.commit(self->ImgBuf_for_response.size());
                self->async_write();
           }
        });
    }

    void async_write() {
        std::cout << "Async_write: started transfering bytes" << std::endl;
        boost::asio::async_write(_socket, streambuf_for_response, boost::asio::transfer_all(),[self = shared_from_this()](boost::system::error_code errorCode, std::uint16_t bytes_transfered)
        {
            if (!errorCode)
                std::cout << "Bytes delivered: " << bytes_transfered << std::endl;
        });
    }
};

class Server {
public:
    Server(boost::asio::io_context &io_context, std::uint16_t port) :
            _ioc(io_context) {
        std::cout << "Server constructed" << std::endl;
        _acceptor.emplace(_ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    }

    void async_accept() {
        _socket.emplace(_ioc);
        _acceptor->async_accept(*_socket, [&](boost::system::error_code errorCode) {
            std::cout << "Connection accepted" << std::endl;
            auto session = std::make_shared<Session>(std::move(*_socket));
            session->start_session();
            async_accept();
        });
    }

private:
    boost::asio::io_context &_ioc;
    std::optional<boost::asio::ip::tcp::acceptor> _acceptor;
    std::optional<boost::asio::ip::tcp::socket> _socket;
};

int main() {
    boost::asio::io_context io_context;
    Server server(io_context, 15001);
    server.async_accept();
    io_context.run();
    return 0;
}
