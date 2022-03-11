// InfinityDebugTools.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <psapi.h>
#include <intsafe.h>
#include <string>
#include <sstream>
#include <tlhelp32.h>
#include <tchar.h>
#include <thread>
#include <chrono>
#include <iomanip>
#include <list>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <regex>
#include <iterator>

#include "TemplateUtils.h"
#include "TcpUtils.h"
#include "StringUtils.h"
#include "FileUtils.h"
#include "InfinityPcre.h"
// #include "StackWalker.h"
#include "InfinityNative.h"
#include "InfinityThread.h"
// #include "InfinityCrashlytics.h"
#include "StringUtils.h"
#include "Object.h"
#include "ArrayObject.h"
#include "RedBlackTree.h"
#include "MemoryTools.h" // must be last!

void funcTest3() {
    #ifdef _WIN64
        //CONTEXT context;
        //context.ContextFlags = CONTEXT_ALL; // Actually we should need only CONTEXT_INTEGER, so let's test that next chance we get.
        //RtlCaptureContext(&context);
        #ifdef _DEBUG
            // printStackTrace4(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\x64\\Debug\\InfinityDebugTools.pdb"));
        #else
            // printStackTrace4(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\x64\\Release\\InfinityDebugTools.pdb"));
        #endif
    #else
        //CONTEXT context;
        //context.ContextFlags = CONTEXT_ALL; // Actually we should need only CONTEXT_INTEGER, so let's test that next chance we get.
        //RtlCaptureContext(&context);
        //

        #ifdef _DEBUG
            // printStackTrace4(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\Debug\\InfinityDebugTools.pdb"), NULL, 1);
        #else
            // printStackTrace4(_T("H:\\Programming\\VSProjects\\InfinityProjectsCpp\\Release\\InfinityDebugTools.pdb"));
        #endif
    #endif

    // printStackTrace3();
}

void funcTest2() {
    // exceptFilter(std::exception("myExcept"));
    //funcTest3();
    // throw "some error happend";
}

void funcTest() {
    //funcTest2();
}

std::string getTimestamp() {
    // get a precise timestamp as a string
    const auto now = std::chrono::system_clock::now();
    const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
    const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    std::stringstream nowSs;
    nowSs
        << std::put_time(std::localtime(&nowAsTimeT), "%a %b %d %Y %T")
        << '.' << std::setfill('0') << std::setw(3) << nowMs.count();
    return nowSs.str();
}

long long getMillis() {
    const auto now = std::chrono::system_clock::now();
    const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
    const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    return nowMs.count();
}

void MyDebugHandler(const char* output) {
    printf("%s", output);
}

const int keys[] = {DMEMORY, DMEMLEAK, 0};

struct MyStruct {
    bool init;
};

int main() {
    // Set the function that will receive all debugging output.
    dbgSetHandler(MyDebugHandler);

    // Set which debug output you want.
    dbgSetKeys(keys);

    testPcre();

    //std::string sp("geeksforgeeks");
    //std::regex re("(geeks)(.*)");

    //// flag type for determining the matching behavior 
    //// && here it is for matches on strings. 
    //std::smatch match;

    //// we can use member function on match 
    //// to extract the matched pattern. 
    //if (std::regex_search(sp, match, re) == true) {

    //    // The size() member function indicates the 
    //    // number of capturing groups plus one for the overall match 
    //    // match size = Number of capturing group + 1 
    //    // (.*) which "forgeeks" ). 
    //    std::cout << "Match size = " << match.size() << std::endl;

    //    // Capturing group is index from 0 to match_size -1  
    //    // .....here 0 to 2 
    //    // pattern at index 0 is the overall match "geeksforgeeks" 
    //    // pattern at index 1 is the first capturing group "geeks" 
    //    // pattern at index 2 is the 2nd capturing group "forgeeks"

    //    std::cout << "Whole match : " << match.str(0) << std::endl;
    //    std::cout << "First capturing group is '" << match.str(1)
    //        << "' which is captured at index " << match.position(1)
    //        << std::endl;
    //    std::cout << "Second capturing group is '" << match.str(2)
    //        << "' which is captured at index " << match.position(2)
    //        << std::endl;
    //} else {
    //    std::cout << "No match is found" << std::endl;
    //}

    //std::string s = "Some people, when confronted with a problem, think "
    //    "\"I know, I'll use regular expressions.\" "
    //    "Now they have two problems.";

    //std::regex self_regex("REGULAR EXPRESSIONS",
    //                      std::regex_constants::ECMAScript | std::regex_constants::icase);
    //if (std::regex_search(s, self_regex)) {
    //    std::cout << "Text contains the phrase 'regular expressions'\n";
    //}

    //std::regex word_regex("(\\w+)");
    //auto words_begin = std::sregex_iterator(s.begin(), s.end(), word_regex);
    //auto words_end = std::sregex_iterator();

    //std::cout << "Found " << std::distance(words_begin, words_end) << " words\n";

    //const int N = 6;
    //std::cout << "Words longer than " << N << " characters:\n";
    //for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
    //    std::smatch match = *i;
    //    std::string match_str = match.str();
    //    if (match_str.size() > N) {
    //        std::cout << "  " << match_str << '\n';
    //    }
    //}

    //std::regex long_word_regex("(\\w{7,})");
    //std::string new_s = std::regex_replace(s, long_word_regex, "[$&]");
    //std::cout << new_s << '\n';

    return _getch();
}