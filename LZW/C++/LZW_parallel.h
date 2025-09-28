#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cmath>
#include <sys/stat.h>
#include <pthread.h>

#define PRINT 0
#define DEBUG 0

using std::cout;
using std::cerr;
using std::endl;

class LZW {
    int num_thread;
    pthread_attr_t* thread_attr;
    pthread_t* threads;
    
    struct encode_thread_args_t {
        int id;
        std::string filename;
        long long start;
        long long block_size;
        std::unordered_map<std::string, long long>* table;
        long long code_begin;
        std::vector<long long> output_code;
    };

    struct decode_thread_args_t {
        int id;
        long long start;
        long long end;
        std::unordered_map<long long, std::string>* table;
        long long code_begin;
        std::vector<std::string> output;
    };

    encode_thread_args_t* encode_args;
    decode_thread_args_t* decode_args;
    std::vector<long long> codes;

    static long long get_file_size(const std::string& file_name) {
        struct stat stat_buf;
        const int rc = stat(file_name.c_str(), &stat_buf);
        return rc == 0 ? stat_buf.st_size : -1;
    }

    static void* encode_wrapper(void* args) {
        const auto lzw_instance = static_cast<LZW*>(static_cast<encode_thread_args_t*>(args)->table->begin()->second.c_str() - 1000000);
        return lzw_instance->encode_thread(args);
    }

    static void* decode_wrapper(void* args) {
        auto lzw_instance = static_cast<LZW*>(static_cast<decode_thread_args_t*>(args)->table->begin()->second.c_str() - 1000000);
        return lzw_instance->decode_thread(args);
    }

    void* encode_thread(void* args) {
        auto curr_arg = static_cast<encode_thread_args_t *>(args);
        std::ifstream ifile(curr_arg->filename);
        ifile.seekg(curr_arg->start);

        #if DEBUG
        printf("Thread %d starts running with the following params\nstart = %lld\nblock_size = %lld\ncode_begin = %lld\n", 
               curr_arg->id, curr_arg->start, curr_arg->block_size, curr_arg->code_begin);
        #endif

        std::string p = "", c = "";
        p += static_cast<char>(ifile.get());
        #if DEBUG
            printf("Thread %d: Initial Read: %s\n", curr_arg->id, p.c_str());
        #endif
        
        int code = curr_arg->code_begin;
        unsigned long long cnt = 1;
        
        while (!ifile.fail()) {
            const char ch = ifile.get();
            if (!ifile.eof()) {
                c += ch;
            }
            
            if (curr_arg->table->find(p + c) != curr_arg->table->end()) {
                p = p + c;
            } else {
                #if DEBUG
                std::cout << p << "\t" << curr_arg->table->at(p) << "\t\t"
                     << p + c << "\t" << code << std::endl;
                #endif
                curr_arg->output_code.push_back(curr_arg->table->at(p));
                
                curr_arg->table->insert(std::make_pair(p+c, code));
                code++;
                p = c;
            }
            c = "";
            cnt++;
            if (curr_arg->block_size > 0 && cnt >= (unsigned long long)curr_arg->block_size) {
                break;
            }
        }
        
        #if DEBUG
        std::cout << p << "\t" << curr_arg->table->at(p) << std::endl;
        #endif
        curr_arg->output_code.push_back(curr_arg->table->at(p));

        if (curr_arg->id * 2 <= num_thread) {
            int next_id = curr_arg->id * 2 - 1;
            #if DEBUG
                printf("Creating thread %d\n", next_id);
            #endif
            encode_args[next_id].code_begin = code;
            encode_args[next_id].table = curr_arg->table;
        }

        if (curr_arg->id * 2 + 1 <= num_thread) {
            int next_id = curr_arg->id * 2;
            #if DEBUG
                printf("Creating thread %d\n", next_id);
            #endif
            encode_args[next_id].code_begin = code;
            encode_args[next_id].table = new std::unordered_map(*(curr_arg->table));
            pthread_create(&threads[next_id], thread_attr, encode_wrapper, &encode_args[next_id]);
        }

        if (curr_arg->id * 2 <= num_thread) {
            encode_thread(&encode_args[curr_arg->id * 2 - 1]);
        }

        if (curr_arg->id * 2 + 1 <= num_thread) {
            pthread_join(threads[curr_arg->id*2], nullptr);
            delete encode_args[curr_arg->id*2].table;
        }

        return nullptr;
    }

    void* decode_thread(void* args) {
        auto&[id, start, end, table, code_begin, output] = *static_cast<decode_thread_args_t *>(args);
        long long old = codes[start];
        std::string s = table->at(old);
        std::string c;
        c += s[0];
        output.push_back(s);
        int count = code_begin;

        for (int i = start+1; i <= end; i++) {
            long long n = codes[i];
            if (table->find(n) == table->end()) {
                s = table->at(old);
                s = s + c;
            } else {
                s = table->at(n);
            }

            output.push_back(s);
            c = "";
            c += s[0];
            table->insert(std::make_pair(count++, table->at(old)+c));
            old = n;
        }

        if (id*2 <= num_thread) {
            const int next_id = id*2 - 1;
            decode_args[next_id].code_begin = count;
            decode_args[next_id].table = table;
        }

        if (id*2+1 <= num_thread) {
            const int next_id = id*2;
            decode_args[next_id].code_begin = count;
            decode_args[next_id].table = new std::unordered_map(*(table));
            pthread_create(&threads[next_id], thread_attr, decode_wrapper, &decode_args[next_id]);
        }

        if (id*2 <= num_thread) {
            decode_thread(&decode_args[id*2-1]);
        }

        if (id*2+1 <= num_thread) {
            pthread_join(threads[id*2], nullptr);
            delete decode_args[id*2].table;
        }

        return nullptr;
    }

public:
    explicit LZW(int threads = 1) : num_thread(threads), threads()
    {
        if (num_thread < 1 || num_thread > 63)
        {
            throw std::invalid_argument("Unsupported number of threads");
        }

        threads = reinterpret_cast<int>(new pthread_t[num_thread]);
        encode_args = new encode_thread_args_t[num_thread];
        decode_args = new decode_thread_args_t[num_thread];

        thread_attr = new pthread_attr_t;
        pthread_attr_init(thread_attr);
        pthread_attr_setdetachstate(thread_attr, PTHREAD_CREATE_JOINABLE);
    }

    ~LZW() {
        delete[] threads;
        delete[] encode_args;
        delete[] decode_args;
        pthread_attr_destroy(thread_attr);
        delete thread_attr;
    }

    bool encode(const std::string& input_file, const std::string& output_file) const
    {
        const long long input_file_size = get_file_size(input_file);
        if (input_file_size == -1) {
            cerr << "Error reading input file" << endl;
            return false;
        }

        const long long block_size = input_file_size / num_thread;
        for (int i = 0; i < num_thread; i++) {
            encode_args[i].id = i+1;
            encode_args[i].filename = input_file;
            encode_args[i].start = i*block_size;
            encode_args[i].block_size = i == num_thread-1 ? -1 : block_size;
        }

        timespec start_time, stop_time;
        double time = 0;
        if(clock_gettime(CLOCK_REALTIME, &start_time) == -1) { 
            perror("clock gettime");
            return false;
        }

        encode_args[0].table = new std::unordered_map<std::string, long long>;
        for (int i = 0; i <= 255; i++) {
            std::string ch = "";
            ch += static_cast<char>(i);
            encode_args[0].table->insert(std::make_pair(ch, i));
        }

        encode_args[0].code_begin = 256;

        pthread_create(&threads[0], thread_attr, encode_wrapper, (void*)&encode_args[0]);
        pthread_join(threads[0], NULL);

        if(clock_gettime(CLOCK_REALTIME, &stop_time) == -1) { 
            perror("clock gettime");
            return false;
        }
        time = (stop_time.tv_sec - start_time.tv_sec) + static_cast<double>(stop_time.tv_nsec - start_time.tv_nsec)/1e9;
        std::cout << "Encoding time = " << time << " sec " << std::endl;

        std::ofstream ofile(output_file);
        if (!ofile.is_open()) {
            cerr << "Error opening output file" << endl;
            delete encode_args[0].table;
            return false;
        }

        long long max_code = 0;
        unsigned long long total_size = 0;
        ofile << -2 << " " << num_thread << endl;
        
        for (int i = 0; i < num_thread; i++) {
            total_size += encode_args[i].output_code.size();
            ofile << -1 << " " << encode_args[i].output_code.size() << endl;
            for (size_t j = 0; j < encode_args[i].output_code.size(); j++) {
                ofile << encode_args[i].output_code[j] << endl;
                max_code = encode_args[i].output_code[j] > max_code ? encode_args[i].output_code[j] : max_code;
            }
        }

        int num_bits = static_cast<int>(ceil(log2(max_code)));
        unsigned long long compressed_file_size = total_size * num_bits / 8;
        
        #if PRINT
        std::cout << "Largest code assigned: " << max_code << endl;
        std::cout << "Number of bits to store each code: " << num_bits << endl;
        std::cout << "Number of output codes: " << total_size << endl;
        std::cout << "Estimated best-case compressed size: " << compressed_file_size << " bytes" << endl;
        #endif
        
        std::cout << "Estimated Compression Rate = " << static_cast<double>(input_file_size) / compressed_file_size << endl;

        delete encode_args[0].table;
        return true;
    }

    bool decode(const std::string& input_file, const std::string& output_file) {
        if (long long input_file_size = get_file_size(input_file); input_file_size < 0) {
            cerr << "Error: cannot read input file or input file is corrupted" << endl;
            return false;
        }

        std::ifstream ifile(input_file);
        if (!ifile.is_open()) {
            cerr << "Error opening input file" << endl;
            return false;
        }

        long long buf;
        ifile >> buf;
        if (ifile.fail() || buf != -2) {
            cerr << "Error: input file is corrupted" << endl;
            ifile.close();
            return false;
        }

        ifile >> num_thread;
        codes.clear();

        int curr_block = 0;
        int code_cnt = 0;
        int block_size = -1;

        while (ifile >> buf) {
            if (buf == -1) {
                decode_args[curr_block].start = code_cnt;
                ifile >> block_size;
                for (int i = 0; i < block_size; i++) {
                    ifile >> buf;
                    codes.push_back(buf);
                    code_cnt++;
                }
                decode_args[curr_block].end = code_cnt-1;
                curr_block++;
            } else {
                cerr << "Warning: corruption" << endl;
                return false;
            }
        }

        timespec start_time, stop_time;
        double time = 0;
        
        if(clock_gettime(CLOCK_REALTIME, &start_time) == -1) { 
            perror("clock gettime");
            return false;
        }

        for (int i = 0; i < num_thread; i++) {
            decode_args[i].id = i+1;
        }

        decode_args[0].table = new std::unordered_map<long long, std::string>;
        for (int i = 0; i <= 255; i++) {
            std::string ch;
            ch += static_cast<char>(i);
            decode_args[0].table->insert(std::make_pair(i, ch));
        }
        decode_args[0].code_begin = 256;

        pthread_create(&threads[0], thread_attr, decode_wrapper, &decode_args[0]);
        pthread_join(threads[0], NULL);

        if(clock_gettime(CLOCK_REALTIME, &stop_time) == -1) { 
            perror("clock gettime");
            return false;
        }
        time = (stop_time.tv_sec - start_time.tv_sec) + static_cast<double>(stop_time.tv_nsec - start_time.tv_nsec)/1e9;
        std::cout << "Decoding time = " << time << " sec " << std::endl;

        std::ofstream ofile(output_file);
        if (!ofile.is_open()) {
            cerr << "Error opening output file" << endl;
            delete decode_args[0].table;
            return false;
        }

        for (int i = 0; i < num_thread; i++) {
            for (size_t j = 0; j < decode_args[i].output.size(); j++) {
                ofile << decode_args[i].output[j];
            }
        }

        delete decode_args[0].table;
        return true;
    }
};


// int main(int argc, char** argv) {
//     if (argc < 4) {
//         cerr << "Usage: ./lzw <encode|decode> <input_file> <output_file> [num_threads]" << endl;
//         return 1;
//     }
//
//     std::string operation = argv[1];
//     std::string input_file = argv[2];
//     std::string output_file = argv[3];
//     int num_threads = argc > 4 ? atoi(argv[4]) : 1;
//
//     try {
//         LZW lzw(num_threads);
//
//         if (operation == "encode") {
//             if (!lzw.encode(input_file, output_file)) {
//                 cerr << "Encoding failed" << endl;
//                 return 1;
//             }
//         } else if (operation == "decode") {
//             if (!lzw.decode(input_file, output_file)) {
//                 cerr << "Decoding failed" << endl;
//                 return 1;
//             }
//         } else {
//             cerr << "Unknown operation: " << operation << endl;
//             return 1;
//         }
//     } catch (const std::exception& e) {
//         cerr << "Error: " << e.what() << endl;
//         return 1;
//     }
//
//     return 0;
// }
