#pragma once
#include "NetworkAcceptor.h"
#include <functional>

typedef std::function<void(tcp::socket*)> TcpAcceptorCallback;

class TcpAcceptor : public NetworkAcceptor
{
  public:
    TcpAcceptor(boost::asio::io_service &io_service,
                TcpAcceptorCallback &callback);
    virtual ~TcpAcceptor() {}

  private:
    TcpAcceptorCallback m_callback;

    virtual void start_accept();
    void handle_accept(tcp::socket *socket, const boost::system::error_code &ec);
};
