/*
* Priston Tale EU
* Copyright (C) 2019
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _PristonTale_Listener_h_
#define _PristonTale_Listener_h_
#include "../Common/SharedDefines.h"
#include "NetworkThread.h"

namespace Priston
{
    // Is there a better way of doing this?
    class GlobalConnections
    {
    public:
        static GlobalConnections* instance()
        {
            static GlobalConnections instance;
            return &instance;
        }

    public:
        GlobalConnections() : CurrentConnections(0) {}
        ~GlobalConnections() {}

    public:
        int32 CurrentConnections;
    };

    template <typename SocketType>
    class Listener
    {
    public:
        Listener(std::string const& address, uint32 port, uint8 workerThreads);
        ~Listener();

    private:
        NetworkThread<SocketType> *SelectWorker() const
        {
            int minIndex = 0;
            size_t minSize = mWorkerThreads[minIndex]->Size();

            for (size_t i = 1; i < mWorkerThreads.size(); ++i)
            {
                const size_t size = mWorkerThreads[i]->Size();

                if (size < minSize)
                {
                    minSize = size;
                    minIndex = i;
                }
            }

            return mWorkerThreads[minIndex].get();
        }

        void BeginAccept();
        void OnAccept(NetworkThread<SocketType> *worker, std::shared_ptr<SocketType> const& socket, const boost::system::error_code &ec);

    private:
        std::unique_ptr<boost::asio::io_service> mService;
        std::unique_ptr<boost::asio::ip::tcp::acceptor> mAcceptor;

        std::thread mAcceptorThread;
        std::vector<std::unique_ptr<NetworkThread<SocketType>>> mWorkerThreads;
    };

    template <typename SocketType>
    Listener<SocketType>::Listener(std::string const& address, uint32 port, uint8 workerThreads)
        : mService(new boost::asio::io_service()), mAcceptor(new boost::asio::ip::tcp::acceptor(*mService, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address), port)))
    {
        mWorkerThreads.reserve(workerThreads);
        for (auto i = 0; i < workerThreads; ++i)
            mWorkerThreads.push_back(std::unique_ptr<NetworkThread<SocketType>>(new NetworkThread<SocketType>));

        BeginAccept();

        mAcceptorThread = std::thread([this]() { this->mService->run(); });
    }

    template <typename SocketType>
    Listener<SocketType>::~Listener()
    {
        IF_LOG(plog::debug)
            LOG_DEBUG << "Destructor SocketType called!";

        mAcceptor->close();
        mService->stop();
        mAcceptorThread.join();
        mAcceptor.reset();
        mService.reset();
    }

    template <typename SocketType>
    void Listener<SocketType>::BeginAccept()
    {
        auto worker = SelectWorker();
        auto socket = worker->CreateSocket();

        mAcceptor->async_accept(socket->GetAsioSocket(),
            [this, worker, socket] (const boost::system::error_code &ec)
        {
            this->OnAccept(worker, socket, ec);
        });
    }

    template <typename SocketType>
    void Listener<SocketType>::OnAccept(NetworkThread<SocketType> *worker, std::shared_ptr<SocketType> const& socket, const boost::system::error_code &ec)
    {
        // an error has occurred
        if (ec)
            worker->RemoveSocket(socket.get());
        else
            socket->Open();

        Priston::GlobalConnections::instance()->CurrentConnections++;
        BeginAccept();
    }
}

#endif /* !_PristonTale_Listener_h_ */
