/*
ELEKTRON © 2023 - now
Written by melektron
www.elektron.work
11.11.23, 23:00
All rights reserved.

This source code is licensed under the Apache-2.0 license found in the
LICENSE file in the root directory of this source tree.

msglink event class used to define custom events
*/

#pragma once

#include <concepts>

#include "../codable.hpp"
#include "../cxxversions.h"


#ifdef __EL_ENABLE_CXX20
namespace el::msglink
{


    /**
     * @brief base class for all incoming msglink event
     * definition classes. To create an incoming event define a class inheriting from this one.
     * Then use the EL_MSGLINK_DEFINE_INCOMING_EVENT macro to generate the required boilerplate.
     * Incoming events are el::decodable[s],
     * meaning they must be decodable from json. If a decoder for a member cannot
     * be generated automatically or needs to be altered, the EL_DECODER macro can be
     * used like with codables to manually define the decoder.
     */
    struct incoming_event : public el::decodable
    {
        virtual ~incoming_event() = default;
        
        // dummy method which must be overriden to ensure the correct
        // generate macro is used.
        virtual void _el_msglink_is_incoming_dummy() const noexcept = 0;
    };

// (public) generates the necessary boilerplate code for an incoming event class.
// The members listed in the arguments will be made decodable using el::decodable
// and are part of the event's data.
#define EL_MSGLINK_DEFINE_INCOMING_EVENT(TypeName, ...)                             \
    static inline const char *_event_name = #TypeName;                              \
    virtual void _el_msglink_is_incoming_dummy() const noexcept override {}         \
    EL_DEFINE_DECODABLE(TypeName, __VA_ARGS__)


    /**
     * @brief base class for all outgoing msglink event 
     * definition classes. To create an outgoing event define a class inheriting from this one.
     * Then use the EL_MSGLINK_DEFINE_OUTGOING_EVENT macro to generate the required boilerplate.
     * Outgoing events are el::encodable[s],
     * meaning they must be encodable to json. If a encoder for a member cannot 
     * be generated automatically or needs to be altered, the EL_ENCODER macro can be
     * used like with codables to manually define the encoder.
     */
    struct outgoing_event : public el::encodable
    {
        virtual ~outgoing_event() = default;

        // dummy method which must be overriden to ensure the correct
        // generate macro is used.
        virtual void _el_msglink_is_outgoing_dummy() const noexcept = 0;
    };

// (public) generates the necessary boilerplate code for an outgoing event class.
// The members listed in the arguments will be made encodable using el::encodable
// and are part of the event's data.
#define EL_MSGLINK_DEFINE_OUTGOING_EVENT(TypeName, ...)                             \
    static inline const char *_event_name = #TypeName;                              \
    virtual void _el_msglink_is_outgoing_dummy() const noexcept override {}         \
    EL_DEFINE_ENCODABLE(TypeName, __VA_ARGS__)


    /**
     * @brief shortcut base class for all bidirectional (incoming and outgoing) msglink event 
     * definition classes. It is simply a composite class inheriting form outgoing_event
     * and incoming_event to save you the hassle of having to do that manually. 
     * Derived classes must satisfy all the requirements of incoming and outgoing events.
     * To create a bidirectional event define a class inheriting from this one.
     * Then use the EL_MSGLINK_DEFINE_BIDIRECTIONAL_EVENT macro to generate the required boilerplate.
     */
    struct bidirectional_event : public incoming_event, public outgoing_event
    {
        virtual ~bidirectional_event() = default;
    };

// (public) generates the necessary boilerplate code for an event class.
// The members listed in the arguments will be made codable using el::codable
// and are part of the event's data.
#define EL_MSGLINK_DEFINE_BIDIRECTIONAL_EVENT(TypeName, ...)                        \
    static inline const char *_event_name = #TypeName;                              \
    virtual void _el_msglink_is_incoming_dummy() const noexcept override {}         \
    virtual void _el_msglink_is_outgoing_dummy() const noexcept override {}         \
    EL_DEFINE_CODABLE(TypeName, __VA_ARGS__)


    /**
     * The following concepts define constraints making sure
     * an event class is either ONLY an incoming event or ONLY
     * an outgoing event or a bidirectional event (BOTH incoming
     * and outgoing)
     */

    /**
     * @brief Constrains _ET to be ONLY derived from incoming_event
     * and NOT from outgoing_event
     */
    template<class _ET>
    concept IncomingOnlyEvent = std::derived_from<_ET, incoming_event> && !std::derived_from<_ET, outgoing_event>;

    /**
     * @brief Constrains _ET to be ONLY derived from outgoing_event
     * and NOT from incoming_event
     */
    template<class _ET>
    concept OutgoingOnlyEvent = std::derived_from<_ET, outgoing_event> && !std::derived_from<_ET, incoming_event>;

    /**
     * @brief Constrains _ET to be derived BOTH from incoming_event
     * and from outgoing_event, making it a bidirectional event
     */
    template<class _ET>
    concept BidirectionalEvent = std::derived_from<_ET, incoming_event> && std::derived_from<_ET, outgoing_event>;


} // namespace el::msglink
#endif  // __EL_ENABLE_CXX20