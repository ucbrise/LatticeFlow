# Click
Kohler et al. recognized that routers were increasingly expected to do more
than route. However, traditional routers were inflexible, and it was hard to
modify them to meet the increasingly complex functionality demanded of them.
To solve this problem, they introduced the [Click modular router][click]: a
framework which enables the simple and modular construction of complex routers.

Using Click, routers are a directed graph of processing units called
*elements*. Click routers are 100% event driven. There is a single process;
there is a single thread; there are no synchronous I/O calls. The leaves of the
element graph register events and event handlers with an event loop which polls
asynchronous I/O calls and dispatches to the correct event handler which pushes
data through the element graph.

This directory generalizes Click and implements a framework to build
event-driven servers. Much like with Click, we construct directed graphs of
processing elements, and the leaf elements register events and event handlers.
The main difference between this framework and Click is that we include general
elements (e.g. [`Map`](map.h), [`GroupBy`](group_by.h)) that are not specific
to building routers.

An example server written in this framework is given in
[`example_server.cc`](example_server.cc). The corresponding client is
[`example_client.cc`](example_client.cc).

[click]: http://bit.ly/2esyPOk
