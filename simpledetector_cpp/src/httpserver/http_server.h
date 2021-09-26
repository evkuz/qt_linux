#ifndef __IQR_HTTP_SERVER_H__
#define __IQR_HTTP_SERVER_H__

#include "non_copyable.h"
#include "exceptions.h"
#include "tools.h"
#include "http_request.h"

#include <stdexcept>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <cstdint>
#include <vector>

#include <evhttp.h>
#include <functional>
#include <string>


namespace IQRNetwork {
  class HttpServer final : private Common::NonCopyable
  {
    DECLARE_RUNTIME_EXCEPTION(HttpServer)

    public:
      typedef std::vector<IHttpRequest::Type> MethodPool;
      typedef std::function<void (IHttpRequestPtr)> OnRequestFunc;
      enum { MaxHeaderSize = static_cast<std::size_t>(-1), MaxBodySize = MaxHeaderSize };
      HttpServer(std::string const &address, std::uint16_t port,
                std::uint16_t threadCount, OnRequestFunc const &onRequest,
                MethodPool const &allowedMethods = {IHttpRequest::Type::GET },
                std::size_t maxHeadersSize = MaxHeaderSize,
                std::size_t maxBodySize = MaxBodySize);
    private:
      volatile bool IsRun = true;
      void (*ThreadDeleter)(std::thread *t) = [] (std::thread *t) { t->join(); delete t; };
      
      typedef std::unique_ptr<std::thread, decltype(ThreadDeleter)> ThreadPtr;
      typedef std::vector<ThreadPtr> ThreadPool;
      ThreadPool Threads;
      Common::BoolFlagInvertor RunFlag;
  };

} // namespace IQRHttp

#endif // !__IQR_HTTP_SERVER_H__
