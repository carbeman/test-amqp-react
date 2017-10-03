/**
 *  MyConnection.cpp
 *
 *  @copyright 2014 Copernica BV
 */

#include "myconnection.h"

/**
 *  Constructor
 */
MyConnection::MyConnection(React::Loop *loop, const std::string& host) :
    _loop(loop),
    _connection(loop, this, host, 5672, AMQP::Login("guest", "guest"), "/"),
    _channel(nullptr)
{}

/**
 *  Method that is called when the connection succeeded
 *  @param  connection  Pointer to the connection
 */
void MyConnection::onConnected(React::AMQP::Connection *connection)
{
    // report connection
    std::cout << "connected" << std::endl;

    // create a new channel
    _channel = std::unique_ptr<React::AMQP::Channel>(new React::AMQP::Channel(&_connection));

    // watch for the channel becoming ready
    _channel->onReady([this]() {
        // show that we are ready
        std::cout << "AMQP channel ready, id: " << (int) _channel->id() << std::endl;
    });

    // and of course for channel errors
    _channel->onError([this](const std::string& message) {
        // inform the user of the error
        std::cerr << "AMQP channel error on channel " << _channel->id() << ": " << message << std::endl;

        // delete the channel
        _channel = nullptr;

        // close the connection
        _connection.close();
    });

    std::string exchange = "amq.direct";
    std::string key = "ema.test";
    std::string testo = "Messaggio di test react";



    //for ( int i = 0; i < 1000000; ++i)
    //{

    _channel->publish(exchange, key, testo);


    //}


    _channel->close().onSuccess([this]() {
            // channel was closed
            std::cout << "Channel closed" << std::endl;

            // close the connection too
            _connection.close();
        });
}

/**
 *  Method that is called when the socket is closed (as a result of a TcpSocket::close() call)
 *  @param  connection  Pointer to the connection
 */
void MyConnection::onClosed(React::AMQP::Connection *connection)
{
    // show
    std::cout << "myconnection closed" << std::endl;

    // close the channel and connection
    _channel = nullptr;

    // stop the loop
    _loop->stop();
}

/**
 *  When the connection ends up in an error state this method is called.
 *  This happens when data comes in that does not match the AMQP protocol
 *
 *  After this method is called, the connection no longer is in a valid
 *  state and can be used. In normal circumstances this method is not called.
 *
 *  @param  connection      The connection that entered the error state
 *  @param  message         Error message
 */
void MyConnection::onError(React::AMQP::Connection *connection, const char *message)
{
    // report error
    std::cout << "AMQP Connection error: " << message << std::endl;
}

