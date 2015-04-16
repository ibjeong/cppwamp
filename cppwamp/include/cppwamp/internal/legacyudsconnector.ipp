/*------------------------------------------------------------------------------
                Copyright Butterfly Energy Systems 2014-2015.
           Distributed under the Boost Software License, Version 1.0.
              (See accompanying file LICENSE_1_0.txt or copy at
                    http://www.boost.org/LICENSE_1_0.txt)
------------------------------------------------------------------------------*/

#include <utility>
#include "client.hpp"
#include "config.hpp"
#include "legacyasioendpoint.hpp"
#include "udsopener.hpp"

namespace wamp
{

namespace legacy
{

//------------------------------------------------------------------------------
CPPWAMP_INLINE UdsConnector::Ptr UdsConnector::create(AsioService& iosvc,
        const std::string& path, CodecId codecId, RawsockMaxLength maxLength)
{
    return Ptr( new UdsConnector({iosvc, path, codecId, maxLength}) );
}

//------------------------------------------------------------------------------
CPPWAMP_INLINE Connector::Ptr UdsConnector::clone() const
{
    return Connector::Ptr(new UdsConnector(info_));
}

//------------------------------------------------------------------------------
CPPWAMP_INLINE void UdsConnector::establish(Handler handler)
{
    CPPWAMP_LOGIC_CHECK(!impl_, "Connection already in progress");
    const Info& i = info_;
    using internal::UdsOpener;
    impl_.reset(new Impl(UdsOpener(i.iosvc, i.path), i.codecId, i.maxRxLength));

    auto self = shared_from_this();
    impl_->establish([this, self, handler](std::error_code ec, CodecId codecId,
                                           Impl::Transport::Ptr trnsp)
    {
        internal::ClientInterface::Ptr client;
        if (!ec)
            client = internal::createClient(codecId, std::move(trnsp));
        info_.iosvc.post(std::bind(handler, ec, client));
        impl_.reset();
    });
}

//------------------------------------------------------------------------------
CPPWAMP_INLINE void UdsConnector::cancel()
{
    if (impl_)
        impl_->cancel();
}

//------------------------------------------------------------------------------
CPPWAMP_INLINE UdsConnector::UdsConnector(Info info)
    : info_(std::move(info))
{}

//------------------------------------------------------------------------------
CPPWAMP_INLINE UdsConnector::Ptr UdsConnector::shared_from_this()
{
    return std::static_pointer_cast<UdsConnector>(
                Connector::shared_from_this() );
}

} // namespace legacy

} // namespace wamp