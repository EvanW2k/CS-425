
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <future>

// These are custom classes that encode the web transactions.  They're
//   actually quite simple (mostly because we're solving a very limited)
//   problem.
#include "Connection.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"

using namespace std;

// This is the unique networking "port" that your web server is communicating
//   with the web browser with.  Make sure to use you're unique port, otherwise
//   you'll stomp on other people trying to do the same thing.
//
// Common ports, particularly for this web and related stuff are:
//   - 22 : ssh port
//   - 25 : email port
//   - 80 : http port (unencrypted web connection)
//   - 8080 : https port (encrypted web connection)
//
//  (Don't use any of them.  Generally, above 9000 is usually pretty clear)
//
const uint16_t DefaultPort = 8104; // Update this variable with your assigned port value
const int bufferSize = 15;
const int numProducers = 10;
const int numConsumers = 30;


// ring buffer class
template <int N>
class RingBuffer {


    public:
        RingBuffer() {
            buffer.resize(N);
        }

        void request(const int& t) {
            requests.acquire();
            {
                lock_guard lock{ requestMutex };
                buffer[nextRequest] = t;
                nextRequest = ++nextRequest % N;
            }

            responses.release();
        }

        int respond() {
            int t;
            responses.acquire();
            {
                lock_guard lock{ responseMutex };
                t = buffer[nextResponse];
                nextResponse = ++nextResponse % N;
            }
            requests.release();
            return t;
        }

    private:
        vector<int> buffer;

        int nextRequest = 0;
        int nextResponse = 0;

        counting_semaphore<N> requests{ N };
        counting_semaphore<N> responses{ 0 };

        mutex requestMutex;
        mutex responseMutex;

};





int main(int argc, char* argv[]) {

    uint16_t port = argc > 1 ? std::stol(argv[1]) : DefaultPort;

    Connection connection(port);
    RingBuffer<bufferSize> buffer;
   
   

    // producer threads
    for (int i = 0; i < numProducers; ++i) {
        thread producerThread{ [&]() {
            int client;
            while (connection) {
                client = connection.accept();
                cout << "Producing\n";
                buffer.request(client);
            }
        } };
        producerThread.detach();
    }


    // consumer threads
    for (int i = 0; i < numConsumers; ++i) {
        thread consumerThread{ [&]() {
                int client;
                while (connection) {
                    client = buffer.respond();
                    cout << "Consuming\n";
                    Session session(client);
                    string msg;
                    session >> msg;
                    HTTPRequest request(msg);
                    const char* root = "/home/faculty/shreiner/public_html/03";
                    HTTPResponse response(request, root);
                    session << response;
                }
        } };
        (i < numConsumers - 1) ? consumerThread.detach() : consumerThread.join();
    }

    return 0;
}
