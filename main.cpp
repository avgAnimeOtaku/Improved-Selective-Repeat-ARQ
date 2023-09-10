#include <bits/stdc++.h>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

// Packet class representing a packet
class Packet {
public:
    int seq_num;
    string data;
    Packet(int seq) : seq_num(seq), data("Packet " + to_string(seq)) {}
};

class Sender {
private:
    int window_size;
    int max_seq_num;
    int send_base;
    int next_seq_num;
    vector<Packet> window;

    void start_timer() {
        double timeout = (double)rand() / RAND_MAX + 0.5;
        cout << "Started timer with timeout " << timeout << "s" << endl;
        this_thread::sleep_for(chrono::milliseconds((int)(timeout * 1000)));
    }

    bool wait_for_ack() {
        double timeout = (double)rand() / RAND_MAX + 0.5;
        cout << "Waiting for acknowledgment with timeout " << timeout << "s" << endl;
        this_thread::sleep_for(chrono::milliseconds((int)(timeout * 1000)));
        return (rand() % 2) == 1;
    }

    void slide_window() {
        cout << "Sliding window" << endl;
        window.erase(window.begin());
        send_base++;
    }

    void resend_packets() {
        cout << "Resending packets" << endl;
        start_timer();
    }

public:
    Sender(int window, int max_seq) : window_size(window), max_seq_num(max_seq), send_base(0), next_seq_num(0) {}

    void send_packets() {
        while (send_base < max_seq_num) {
            while (window.size() < window_size && next_seq_num < max_seq_num) {
                Packet packet(next_seq_num++);
                window.push_back(packet);
                cout << "Sending packet with sequence number " << packet.seq_num << endl;
            }
            start_timer();
            bool ack_received = wait_for_ack();
            if (ack_received) {
                slide_window();
            } else {
                resend_packets();
            }
        }
    }
};

// Receiver class representing the receiver
class Receiver {
private:
    int max_seq_num;
    int expected_seq_num;
    double error_probability;

    bool is_packet_corrupted() {
        return (double)rand() / RAND_MAX < error_probability;
    }

    void send_ack() {
        cout << "Sending acknowledgment for sequence number " << expected_seq_num << endl;
    }

public:
    Receiver(int max_seq, double error_prob) : max_seq_num(max_seq), expected_seq_num(0), error_probability(error_prob) {}

    void receive_packets() {
        while (expected_seq_num < max_seq_num) {
            int seq_num = rand() % max_seq_num;
            Packet packet(seq_num);
            cout << "Received packet with sequence number " << packet.seq_num << endl;
            if (is_packet_corrupted()) {
                cout << "Packet with sequence number " << packet.seq_num << " is corrupted" << endl;
                continue;
            }
            if (packet.seq_num == expected_seq_num) {
                cout << "Sending acknowledgment for sequence number " << expected_seq_num << endl;
                send_ack();
                expected_seq_num++;
            } else {
                cout << "Discarding packet with sequence number " << packet.seq_num << endl;
            }
        }
    }
};

int main() {
    srand(time(NULL));

    Sender sender(4, 10);
    Receiver receiver(10, 0.2); // 0.1 is the error probability for packet corruption

    thread sender_thread(&Sender::send_packets, &sender);
    thread receiver_thread(&Receiver::receive_packets, &receiver);

    sender_thread.join();
    receiver_thread.join();

    return 0;
}
