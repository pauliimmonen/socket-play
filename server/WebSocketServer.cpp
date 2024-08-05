#include "WebSocketServer.hpp"
#include <iostream>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

WebSocketServer::WebSocketServer() {
    m_server.init_asio();

    m_server.set_reuse_addr(true);

    m_server.set_open_handler(bind(&WebSocketServer::on_open, this, ::_1));
    m_server.set_close_handler(bind(&WebSocketServer::on_close, this, ::_1));
    m_server.set_message_handler(bind(&WebSocketServer::on_message, this, ::_1, ::_2));
    m_server.set_fail_handler(bind(&WebSocketServer::on_fail, this, ::_1));

    m_server.clear_access_channels(websocketpp::log::alevel::all);
    m_server.set_access_channels(websocketpp::log::alevel::connect);
    m_server.set_access_channels(websocketpp::log::alevel::disconnect);
    m_server.set_access_channels(websocketpp::log::alevel::app);

    m_server.set_ping_handler(bind(&WebSocketServer::on_ping, this, ::_1, ::_2));
    m_server.set_pong_timeout_handler(bind(&WebSocketServer::on_pong_timeout, this, ::_1, ::_2));
}

void WebSocketServer::run(uint16_t port) {
    m_server.listen(port);
    m_server.start_accept();
    std::cout << "Server listening on port " << port << std::endl;
    m_server.run();
}

void WebSocketServer::stop() {
    m_server.stop_listening();
    for (auto& conn : m_connections) {
        try {
            m_server.close(conn.first, websocketpp::close::status::going_away, "Server shutdown");
        } catch (const websocketpp::exception& e) {
            std::cerr << "Error closing connection: " << e.what() << std::endl;
        }
    }
    m_server.stop();
}

void WebSocketServer::on_open(websocketpp::connection_hdl hdl) {
    server::connection_ptr con = m_server.get_con_from_hdl(hdl);
    std::cout << "New connection from " << con->get_remote_endpoint() << std::endl;

    auto new_player = m_game_state.addPlayer();
    m_connections[hdl] = new_player;
    std::cout << "New player connected. ID: " << new_player->id << std::endl;

    broadcast_game_state();
}

void WebSocketServer::on_close(websocketpp::connection_hdl hdl) {
    auto it = m_connections.find(hdl);
    if (it != m_connections.end()) {
        std::cout << "Player " << it->second->id << " disconnected" << std::endl;
        m_game_state.removePlayer(it->second->id);
        m_connections.erase(it);
        broadcast_game_state();
    } else {
        std::cout << "Unknown connection closed" << std::endl;
    }
}

void WebSocketServer::on_fail(websocketpp::connection_hdl hdl) {
    server::connection_ptr con = m_server.get_con_from_hdl(hdl);
    std::cout << "Connection failed. Error: " << con->get_ec().message() << std::endl;
}

void WebSocketServer::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::cout << "Received raw message: " << msg->get_payload() << std::endl;
    
    try {
        auto j = nlohmann::json::parse(msg->get_payload());
        std::cout << "Parsed JSON: " << j.dump(4) << std::endl;
        
        GameAction action = parseGameAction(j);
        auto it = m_connections.find(hdl);
        if (it != m_connections.end()) {
            int playerId = it->second->id;
            if (m_game_state.handleAction(playerId, action)) {
                broadcast_game_state();
            } else {
                std::cerr << "Failed to handle action" << std::endl;
            }
        } else {
            std::cerr << "Unknown connection sent a message" << std::endl;
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        std::cerr << "Failed to parse: " << msg->get_payload() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error processing message: " << e.what() << std::endl;
    }
}

GameAction WebSocketServer::parseGameAction(const nlohmann::json& j) {
    GameAction action;
    action.type = GameAction::Type::Unknown;

    if (j.contains("action")) {
        std::string actionStr = j["action"];
        if (actionStr == "move") {
            action.type = GameAction::Type::Move;
            if (j.contains("dx") && j.contains("dy")) {
                action.dx = j["dx"];
                action.dy = j["dy"];
            }
        }
    }

    return action;
}

void WebSocketServer::broadcast_game_state() {
    nlohmann::json game_state = m_game_state.getState();
    std::string payload = game_state.dump();
    std::cout << "Broadcasting game state: " << payload << std::endl;
    
    for (auto it : m_connections) {
        try {
            m_server.send(it.first, payload, websocketpp::frame::opcode::text);
        } catch (const websocketpp::exception& e) {
            std::cerr << "Error broadcasting game state: " << e.what() << std::endl;
        }
    }
}

bool WebSocketServer::on_ping(websocketpp::connection_hdl hdl, std::string) {
    try {
        m_server.pong(hdl, "");
        return true;
    } catch (const websocketpp::exception& e) {
        std::cerr << "Error sending pong: " << e.what() << std::endl;
        return false;
    }
}

void WebSocketServer::on_pong_timeout(websocketpp::connection_hdl hdl, std::string) {
    std::cout << "Pong timeout. Closing connection." << std::endl;
    try {
        m_server.close(hdl, websocketpp::close::status::policy_violation, "Pong timeout");
    } catch (const websocketpp::exception& e) {
        std::cerr << "Error closing connection: " << e.what() << std::endl;
    }
}
