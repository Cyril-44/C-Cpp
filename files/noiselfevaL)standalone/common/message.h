// ArbiterPlus 独立服务器工程 —— c-s/common/message.h
#ifndef AP_MESSAGE_H
#define AP_MESSAGE_H

#include <string>
#include <list>
#include <semaphore.h>

enum Role { Begin = 0, End = 1, Progress = 2, Query = 3, Result = 4 };
enum RunType { NotRun = 0, Sync = 1, Async = 2 };

class MessageQueue;
class CommandRes;

// ---- 消息基类 ----
class Message {
public:
    Message() : m_sync(true), m_type(NotRun) {}
    virtual ~Message() {}

    virtual void handle() = 0;
    virtual std::string query(int what) const { (void)what; return ""; }
    virtual void delRef() {}

    bool sync() const { return m_sync; }
    void setSync(bool s) { m_sync = s; }
    RunType runType() const { return m_type; }
    void setRunType(RunType t) { m_type = t; }

protected:
    bool    m_sync;
    RunType m_type;
};

class CancelMessage : public Message {
public:
    CancelMessage(MessageQueue* q) : m_queue(q) {}
    void handle();
private:
    MessageQueue* m_queue;
};

class NotifyMessage : public Message {
public:
    NotifyMessage(Message* m, MessageQueue* q) : m_message(m), m_queue(q) {}
    void handle();
    void delRef() { delete this; }
private:
    Message*      m_message;
    MessageQueue* m_queue;
};

// ---- 通知者 ----
class Notifier {
public:
    struct NResult {
        int         res;
        std::string info;
        NResult(int r, const std::string& i) : res(r), info(i) {}
    };
    Notifier() : m_canceled(false) {}
    virtual ~Notifier() {}

    virtual void notify(Role role, const std::string& name, void* param = 0) = 0;
    virtual void delRef() {}

    bool canceled() const { return m_canceled; }
    void setCanceled(bool c) { m_canceled = c; }

protected:
    volatile bool m_canceled;
};

class AsyncVisitor {
public:
    virtual ~AsyncVisitor() {}
    virtual bool visit(Message* m) = 0;
};

// ---- 消息队列 ----
class MessageQueue {
public:
    MessageQueue();
    ~MessageQueue();

    void run();
    void push(Message* m);
    Message* pop();
    void quit() { push(NULL); }

    void handleAsync(Message* m);
    static void* messageThread(void* param);
    Message* popAsync();
    void addAsyncMsg(Message* m);
    bool removeAsyncMsg(Message* m);
    void visitAsyncMessages(AsyncVisitor* v);
    int  asyncCount();

    friend class NotifyMessage;
    friend class CancelMessage;

private:
    static void _semwait(sem_t* s);
    sem_t                 m_empty;
    sem_t                 m_mutex;
    sem_t                 m_asyncMutex;
    std::list<Message*>   m_msgs;
    std::list<Message*>   m_asyncMsgs;
    std::list<Message*>   m_asyncWaitMsgs;
};

#endif
