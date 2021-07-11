# KafkaUI simple desktop application for Apache Kafka

KafkaUI simple GUI desktop application for Apache Kafka based on [librdkafka](https://github.com/edenhill/librdkafka) and
[Qt](https://www.qt.io/) and written on C++.

## Build from source

### Requirements

* Qt >= 6;
* librdkafka >= 1.7;
* spdlog

#### GNU/Linux

    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
    $ make
    $ ./kafkaui
    