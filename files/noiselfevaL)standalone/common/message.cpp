// ArbiterPlus 独立服务器工程 —— c-s/common/message.cpp
#include "message.h"
#include "arch.h"
#include "log.h"
#include <errno.h>
#include <pthread.h>

typedef std::pair<Message*, MessageQueue*> AsyncData;

MessageQueue::MessageQueue()
{
    sem_init(&m_empty, 0, 0);
    sem_init(&m_mutex, 0, 1);
    sem_init(&m_asyncMutex, 0, 1);
}

MessageQueue::~MessageQueue()
{
    sem_destroy(&m_empty);
    sem_destroy(&m_mutex);
    sem_destroy(&m_asyncMutex);
}

void MessageQueue::_semwait(sem_t* s)
{
    while (sem_wait(s) == -1 && errno == EINTR) {}
}

void MessageQueue::run()
{
    bool quit = false;
    while (!quit) {
        Message* m = pop();
        if (m == NULL) {
            quit = true;
        } else if (m->sync()) {
            m->setRunType(Sync);
            m->handle();
            m->delRef();
        } else if (asyncCount() < GlobalConfig::m_asyncMax) {
            handleAsync(m);
        } else {
            m_asyncWaitMsgs.push_back(m);
        }
    }
}

void MessageQueue::push(Message* m)
{
    _semwait(&m_mutex);
    m_msgs.push_back(m);
    sem_post(&m_empty);
    sem_post(&m_mutex);
}

Message* MessageQueue::pop()
{
    _semwait(&m_empty);
    _semwait(&m_mutex);
    Message* m = m_msgs.front();
    m_msgs.pop_front();
    sem_post(&m_mutex);
    return m;
}

void MessageQueue::handleAsync(Message* m)
{
    AsyncData* data = new AsyncData();
    data->first  = m;
    data->second = this;

    pthread_t thread;
    if (pthread_create(&thread, NULL, &MessageQueue::messageThread, data) == 0) {
        pthread_detach(thread);
        addAsyncMsg(m);
        APLog::printf(Normal, "create thread to handle message [%s]", m->query(0).c_str());
    } else {
        delete data;
        APLog::printf(Critical, "failed to create thread to handle message [%s]",
                      m->query(0).c_str());
        m->setRunType(Sync);
        m->handle();
        m->delRef();
    }
}

void* MessageQueue::messageThread(void* param)
{
    Message* m     = ((AsyncData*)param)->first;
    MessageQueue* q = ((AsyncData*)param)->second;
    delete (AsyncData*)param;

    m->setRunType(Async);
    m->handle();

    NotifyMessage* n = new NotifyMessage(m, q);
    q->push(n);
    pthread_exit(NULL);
}

void NotifyMessage::handle()
{
    m_queue->removeAsyncMsg(m_message);        // 内部执行 m_message->delRef()
    while (!m_queue->m_asyncWaitMsgs.empty()) {
        Message* w = m_queue->popAsync();
        if (w != NULL) {
            m_queue->handleAsync(w);
            break;
        }
    }
}

void CancelMessage::handle()
{
    // (还原语义: 置消队列中滞留的异步消息; 具体命令经 Notifier::setCanceled 生效)
}

Message* MessageQueue::popAsync()
{
    Message* m = NULL;
    if (!m_asyncWaitMsgs.empty()) {
        m = m_asyncWaitMsgs.front();
        m_asyncWaitMsgs.pop_front();
    }
    return m;
}

void MessageQueue::addAsyncMsg(Message* m)
{
    _semwait(&m_asyncMutex);
    m_asyncMsgs.push_back(m);
    sem_post(&m_asyncMutex);
}

bool MessageQueue::removeAsyncMsg(Message* m)
{
    _semwait(&m_asyncMutex);
    for (std::list<Message*>::iterator it = m_asyncMsgs.begin();
         it != m_asyncMsgs.end(); ++it) {
        if (*it == m) {
            m_asyncMsgs.erase(it);
            m->delRef();
            sem_post(&m_asyncMutex);
            return true;
        }
    }
    sem_post(&m_asyncMutex);
    return false;
}

void MessageQueue::visitAsyncMessages(AsyncVisitor* v)
{
    _semwait(&m_asyncMutex);
    for (std::list<Message*>::iterator it = m_asyncMsgs.begin();
         it != m_asyncMsgs.end(); ++it) {
        if (!v->visit(*it))
            break;
    }
    sem_post(&m_asyncMutex);
}

int MessageQueue::asyncCount()
{
    _semwait(&m_asyncMutex);
    int n = (int)m_asyncMsgs.size();
    sem_post(&m_asyncMutex);
    return n;
}
