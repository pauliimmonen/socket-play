#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "GameState.hpp"
#include <map>
#include <memory>

class WebSocketServer {
public:
    WebSocketServer();
    void run(uint16_t port);
    void stop();

private:
    typedef websocketpp::server<websocketpp::config::asio> server;
    typedef std::map<websocketpp::connection_hdl, std::shared_ptr<Player>, std::owner_less<websocketpp::connection_hdl>> con_list;

    server m_server;
    con_list m_connections;
    GameState m_game_state;

    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);
    void on_fail(websocketpp::connection_hdl hdl);
    GameAction parseGameAction(const nlohmann::json& j);
    void broadcast_game_state();
    bool on_ping(websocketpp::connection_hdl hdl, std::string);
    void on_pong_timeout(websocketpp::connection_hdl hdl, std::string);
};