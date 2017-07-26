/*
 * Source :
 * https://stackoverflow.com/questions/44093964/c-blocking-queue-with-boost
 */

#include <boost/thread.hpp>
#include <deque>
#include <fstream>

#include <iostream>
static boost::mutex s_iomutex;

template <typename T> class BlockingQueue {
  public:
    explicit BlockingQueue(size_t capacity) : _buffer(), _capacity(capacity) {
        assert(capacity>0);
    }

    void push(const T &elem) {
        boost::unique_lock<boost::mutex> lock(_mutex);
        _pop_event.wait(lock, [&] { return _buffer.size() < _capacity; });
        _buffer.push_back(elem);
        _push_event.notify_one(); // notifies one of the waiting threads which are blocked on the queue
        // assert(!_buffer.empty());
    }

    T pop() {
        boost::unique_lock<boost::mutex> lock(_mutex);
        _push_event.wait(lock, [&] { return _buffer.size() > 0; });

        T elem = _buffer.front();
        _buffer.pop_front();
        _pop_event.notify_one();
        return elem;
    }

  private:
    boost::mutex _mutex;
    boost::condition_variable _push_event, _pop_event;
    std::deque<T> _buffer;
    size_t _capacity;
};

class FileProcessor {
  public:
    explicit FileProcessor(size_t capacity = 10) : bqueue(capacity), inFile("random.txt") {}

    /* Read strings from a file, populate them in the blocking-queue */
    void read() {
        try {
            std::ifstream file(inFile, std::ios_base::in | std::ios_base::binary);
            for (std::string inputStr; std::getline(file, inputStr);) {
                bqueue.push(inputStr);

                boost::lock_guard<boost::mutex> lock(s_iomutex);
                std::cout << "inserted " << inputStr << "\n";
            }
        } catch (std::exception &e) {
            std::cerr << "exception occurred while reading file\n" << e.what() << "\n";
        }
    }

    /* Process the elements (dequeue and print) */
    void process() {
        while (true) {
            std::string rstr = bqueue.pop();
            boost::lock_guard<boost::mutex> lock(s_iomutex);
            std::cout << "consumed " << rstr << "\n";
        }
    }

  private:
    BlockingQueue<std::string> bqueue;
    std::string inFile; // input file name from where the strings are read
};

int main() {
    FileProcessor fp(10);
    boost::thread rt(boost::bind(&FileProcessor::read, &fp));
    boost::thread pt1(boost::bind(&FileProcessor::process, &fp));
    rt.join();

    pt1.interrupt();
    pt1.join();
}
