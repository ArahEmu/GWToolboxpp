#pragma once

namespace easywsclient {
    class WebSocket;
}

class TradeWindow : public ToolboxWindow {
    TradeWindow() {};
    ~TradeWindow();
public:
    static TradeWindow& Instance() {
        static TradeWindow instance;
        return instance;
    }

    const char* Name() const { return "Trade"; }

    void Initialize() override;
    void Terminate() override;

    void Update(float delta) override;
    void Draw(IDirect3DDevice9* pDevice) override;

    void LoadSettings(CSimpleIni* ini) override;
    void SaveSettings(CSimpleIni* ini) override;
    void DrawSettingInternal() override;

private:
    struct Message {
        uint32_t    timestamp;
        std::string name;
        std::string message;
    };

    bool show_alert_window = false;

    // if we need to print in the chat
    bool print_game_chat = false;

    // if enable, we won't print the messages containing word from alert_words
    bool filter_alerts = false;

    #define ALERT_BUF_SIZE 1024 * 16
    char alert_buf[ALERT_BUF_SIZE];
    // set when the alert_buf was modified
    bool alertfile_dirty = false;

    std::vector<std::string> alert_words;

    void DrawAlertsWindowContent(bool ownwindow);

    static bool GetInKamadan();

    // Since we are connecting in an other thread, the following attributes/methods avoid spamming connection requests
    void AsyncChatConnect();
    void AsyncWindowConnect();
    bool ws_chat_connecting = false;
    bool ws_window_connecting = false;

    easywsclient::WebSocket *ws_chat = NULL;
    easywsclient::WebSocket *ws_window = NULL;

    bool search_pending;
    void search(std::string);
    void fetch();

    static Message parse_json_message(nlohmann::json& js);
    CircularBuffer<Message> messages;

    // tasks to be done async by the worker thread
    std::queue<std::function<void()>> thread_jobs;
    bool should_stop = false;
    std::thread worker;

    void ParseBuffer(const char *text, std::vector<std::string> &words);
    void ParseBuffer(std::fstream stream, std::vector<std::string>& words);
};
