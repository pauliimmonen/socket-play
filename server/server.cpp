#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <set>
#include <string>
#include <map>
#include <random>
#include <sstream>
#include <nlohmann/json.hpp>
#include <memory>
#include <chrono>

typedef websocketpp::server<websocketpp::config::asio> server;
using json = nlohmann::json;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

struct Player {
    int id;
    int x;
    int y;
    int score;
};

struct GameAction {
    enum class Type {
        Move,
        Unknown
    };

    Type type;
    int dx = 0;
    int dy = 0;
};

class GameState {
public:
    GameState() : m_next_id(1) {}

    std::shared_ptr<Player> addPlayer() {
        auto new_player = std::make_shared<Player>(Player{m_next_id++, 0, 0, 0});
        m_players[new_player->id] = new_player;
        return new_player;
    }

    void removePlayer(int id) {
        m_players.erase(id);
    }

    bool handleAction(int playerId, const GameAction& action) {
        switch (action.type) {
            case GameAction::Type::Move:
                return movePlayer(playerId, action.dx, action.dy);
            default:
                return false;
        }
    }

    json getState() const {
        json state;
        state["players"] = json::array();
        for (const auto& pair : m_players) {
            const auto& player = pair.second;
            state["players"].push_back({
                {"id", player->id},
                {"x", player->x},
                {"y", player->y},
                {"score", player->score}
            });
        }
        return state;
    }

private:
    std::map<int, std::shared_ptr<Player>> m_players;
    int m_next_id;

    bool movePlayer(int id, int dx, int dy) {
        auto it = m_players.find(id);
        if (it != m_players.end()) {
            it->second->x += dx;
            it->second->y += dy;
            return true;
        }
        return false;
    }
};

class WebSocketServer {
public:
    WebSocketServer() {
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

    void run(uint16_t port) {
        m_server.listen(port);
        m_server.start_accept();
        std::cout << "Server listening on port " << port << std::endl;
        m_server.run();
    }

    void stop() {
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

private:
    typedef std::map<connection_hdl, std::shared_ptr<Player>, std::owner_less<connection_hdl>> con_list;

    server m_server;
    con_list m_connections;
    GameState m_game_state;

    void on_open(connection_hdl hdl) {
        server::connection_ptr con = m_server.get_con_from_hdl(hdl);
        std::cout << "New connection from " << con->get_remote_endpoint() << std::endl;

        auto new_player = m_game_state.addPlayer();
        m_connections[hdl] = new_player;
        std::cout << "New player connected. ID: " << new_player->id << std::endl;

        broadcast_game_state();
    }

    void on_close(connection_hdl hdl) {
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

    void on_fail(connection_hdl hdl) {
        server::connection_ptr con = m_server.get_con_from_hdl(hdl);
        std::cout << "Connection failed. Error: " << con->get_ec().message() << std::endl;
    }

    void on_message(connection_hdl hdl, server::message_ptr msg) {
        std::cout << "Received raw message: " << msg->get_payload() << std::endl;
        
        try {
            auto j = json::parse(msg->get_payload());
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
        } catch (const json::exception& e) {
            std::cerr << "JSON parsing error: " << e.what() << std::endl;
            std::cerr << "Failed to parse: " << msg->get_payload() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error processing message: " << e.what() << std::endl;
        }
    }

    GameAction parseGameAction(const json& j) {
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

    void broadcast_game_state() {
        json game_state = m_game_state.getState();
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

    bool on_ping(connection_hdl hdl, std::string) {
        try {
            m_server.pong(hdl, "");
            return true;
        } catch (const websocketpp::exception& e) {
            std::cerr << "Error sending pong: " << e.what() << std::endl;
            return false;
        }
    }

    void on_pong_timeout(connection_hdl hdl, std::string) {
        std::cout << "Pong timeout. Closing connection." << std::endl;
        try {
            m_server.close(hdl, websocketpp::close::status::policy_violation, "Pong timeout");
        } catch (const websocketpp::exception& e) {
            std::cerr << "Error closing connection: " << e.what() << std::endl;
        }
    }
};

int main() {
    try {
        WebSocketServer server;
        std::thread server_thread([&server]() {
            server.run(9002);
        });

        std::cout << "Server running. Press 'q' to quit." << std::endl;
        char input;
        do {
            std::cin >> input;
        } while (input != 'q');

        std::cout << "Shutting down server..." << std::endl;
        server.stop();
        server_thread.join();
        std::cout << "Server stopped." << std::endl;
    } catch (websocketpp::exception const & e) {
        std::cout << "WebSocket++ exception: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Standard exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception" << std::endl;
    }
    return 0;
}