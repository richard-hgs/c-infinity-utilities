// InfinityPcreTools.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#define PCRE2_CODE_UNIT_WIDTH 16
#define DEBUG_ON false

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <nlohmann/json.hpp>
#include "CliUtils.h"
#include "StringUtils.h"
#include "pcre2.h"



/**************************************************************************
* Here is the program. The API includes the concept of "contexts" for     *
* setting up unusual interface requirements for compiling and matching,   *
* such as custom memory managers and non-standard newline definitions.    *
* This program does not do any of this, so it makes no use of contexts,   *
* always passing NULL where a context could be given.                     *
**************************************************************************/

int main(int argc, char** argv) {
    pcre2_code* re;
    //PCRE2_SPTR pattern = (PCRE2_SPTR) L"((?<spc>[.?#@]+)|(?<w_s>\\w+))\\s*(?<digit>\\d+)";     /* PCRE2_SPTR is a pointer to unsigned code units of */
    //PCRE2_SPTR subject = (PCRE2_SPTR) L"(I am a string with words and digits 45 and specials chars: ?.#@ 443 অ আ ক খ গ ঘ  56)";     /* the appropriate width (8, 16, or 32 bits). */
    //PCRE2_SPTR pattern = (PCRE2_SPTR)L"(?<name>\\\\N)*(?<function>{\\\\+[a-z0-9]+(\\(+[0-9,]+\\)+)*\\}+)(?<text>[a-z\\sA-Z]+)*";     /* PCRE2_SPTR is a pointer to unsigned code units of */
    //PCRE2_SPTR subject = (PCRE2_SPTR)L"{\\fad(2027,1)}{\\fs30}Yowamushi Pedal \\N{\\fs20}The Movie";     /* the appropriate width (8, 16, or 32 bits). */
    PCRE2_SPTR name_table = nullptr;
    nlohmann::json jsonObjResp = { nlohmann::json::object() };

    int crlf_is_newline;
    int errornumber;
    int find_all;
    int i;
    int namecount;
    int name_entry_size;
    int rc;
    int utf8;
    std::string key, value;

    uint32_t option_bits = PCRE2_DUPNAMES | PCRE2_UTF | PCRE2_UCP;
    uint32_t newline;

    PCRE2_SPTR pattern;
    PCRE2_SPTR subject;

    PCRE2_SIZE erroroffset;
    PCRE2_SIZE* ovector;

    size_t subject_length;
    pcre2_match_data* match_data;

    int unamed_match_pos = 0;
    int named_match_pos = 0;



    /**************************************************************************
    * First, sort out the command line. There is only one possible option at  *
    * the moment, "-g" to request repeated matching to find all occurrences,  *
    * like Perl's /g option. We set the variable find_all to a non-zero value *
    * if the -g option is present. Apart from that, there must be exactly two *
    * arguments.                                                              *
    **************************************************************************/

    find_all = 1;
    //for (i = 1; i < argc; i++)
    //  {
    //  if (strcmp(argv[i], "-g") == 0) find_all = 1;
    //    else break;
    //  }

    ///* After the options, we require exactly two arguments, which are the pattern,
    //and the subject string. */

    //if (argc - i != 2)
    //  {
    //  printf("Two arguments required: a regex and a subject string\n");
    //  return 1;
    //  }

    ///* As pattern and subject are char arguments, they can be straightforwardly
    //cast to PCRE2_SPTR as we are working in 8-bit code units. */

    //pattern = (PCRE2_SPTR)argv[i];
    //subject = (PCRE2_SPTR)argv[i+1];

    char* mPattern = nullptr;
    char* mText = nullptr;
    if (CliUtils::cmdOptionExists(argv, argv + argc, "--help")) {
        return 0;
    }

    if (CliUtils::cmdOptionExists(argv, argv + argc, "--regex")) {
        mPattern = CliUtils::getCmdOption(argv, argv + argc, "--regex");
    }
    if (CliUtils::cmdOptionExists(argv, argv + argc, "--text")) {
        mText = CliUtils::getCmdOption(argv, argv + argc, "--text");
    }

    if (mText == nullptr || mPattern == nullptr) {
        printf("Invalid command, please verify the --help to find more information about command usages.");
        return 0;
    }

    wchar_t* mWpattern;
    wchar_t* mWText;

    StringUtils::toWchart(mPattern, mWpattern);
    StringUtils::toWchart(mText, mWText);

    pattern = (PCRE2_SPTR) mWpattern;
    subject = (PCRE2_SPTR) mWText;

    // Uncoment to see the pattern received
    /*std::wstring message;
    message += L"pattern: " + std::wstring(mWpattern) + std::wstring(L"\n") + std::wstring(mWText);

    int msgboxID = MessageBox(
        NULL,
        (LPCWSTR) message.c_str(),
        (LPCWSTR)L"AVISO",
        MB_ICONWARNING | MB_OK | MB_DEFBUTTON2
    );*/

    //wprintf(L"pattern: %s\n", mWpattern);
    //wprintf(L"text: %s\n", mWText);


#if PCRE2_CODE_UNIT_WIDTH == 8
    subject_length = strlen((char*)subject);
#elif PCRE2_CODE_UNIT_WIDTH == 16 || PCRE2_CODE_UNIT_WIDTH == 32
    subject_length = wcslen((wchar_t*)subject);
#endif

    /*************************************************************************
    * Now we are going to compile the regular expression pattern, and handle *
    * any errors that are detected.                                          *
    *************************************************************************/

    re = pcre2_compile(
        pattern,               /* the pattern */
        PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
        option_bits,                     /* default options */
        &errornumber,          /* for error number */
        &erroroffset,          /* for error offset */
        NULL);                 /* use default compile context */

      /* Compilation failed: print the error message and exit. */

    if (re == NULL)
    {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
#if PCRE2_CODE_UNIT_WIDTH == 8
        printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);
#elif PCRE2_CODE_UNIT_WIDTH == 16 || PCRE2_CODE_UNIT_WIDTH == 32
        wprintf(L"PCRE2 compilation failed at offset %d: %ls\n", (int)erroroffset, (wchar_t*) buffer);
#endif
        return 1;
    }


    /*************************************************************************
    * If the compilation succeeded, we call PCRE again, in order to do a     *
    * pattern match against the subject string. This does just ONE match. If *
    * further matching is needed, it will be done below. Before running the  *
    * match we must set up a match_data block for holding the result.        *
    *************************************************************************/

    /* Using this function ensures that the block is exactly the right size for
    the number of capturing parentheses in the pattern. */

    match_data = pcre2_match_data_create_from_pattern(re, NULL);

    rc = pcre2_match(
        re,                   /* the compiled pattern */
        subject,              /* the subject string */
        subject_length,       /* the length of the subject */
        0,                    /* start at offset 0 in the subject */
        0,                    /* default options */
        match_data,           /* block for storing the result */
        NULL);                /* use default match context */

      /* Matching failed: handle error cases */

    if (rc < 0)
    {
        switch (rc)
        {
        case PCRE2_ERROR_NOMATCH: printf("No match\n"); break;
            /*
            Handle other special cases if you like
            */
        default: printf("Matching error %d\n", rc); break;
        }
        pcre2_match_data_free(match_data);   /* Release memory used for the match */
        pcre2_code_free(re);                 /* data and the compiled pattern. */
        return 1;
    }

    /* Match succeded. Get a pointer to the output vector, where string offsets are
    stored. */

    ovector = pcre2_get_ovector_pointer(match_data);
    if (DEBUG_ON) {
        printf("\nMatch succeeded at offset %d\n", (int)ovector[0]);
    }


    /*************************************************************************
    * We have found the first match within the subject string. If the output *
    * vector wasn't big enough, say so. Then output any substrings that were *
    * captured.                                                              *
    *************************************************************************/

    /* The output vector wasn't big enough. This should not happen, because we used
    pcre2_match_data_create_from_pattern() above. */

    if (rc == 0) {
        printf("ovector was not big enough for all the captured substrings\n");
    }

    /* Show substrings stored in the output vector by number. Obviously, in a real
    application you might want to do things other than print them. */
    for (i = 0; i < rc; i++)
    {
        PCRE2_SPTR substring_start = subject + ovector[2 * i];
        size_t substring_length = ovector[2 * i + 1] - ovector[2 * i];
#if PCRE2_CODE_UNIT_WIDTH == 8
        printf("%2d: %.*s\n", i, (int)substring_length, (char*)substring_start);
        std::string tmps1 = std::string((char*)substring_start);
        value = tmps1.substr(0, substring_length);
        std::cout << "value: " << value << std::endl;
#elif PCRE2_CODE_UNIT_WIDTH == 16 || PCRE2_CODE_UNIT_WIDTH == 32
        if (DEBUG_ON) {
            printf("%2d: %.*ls\n", i, (int)substring_length, (wchar_t*)substring_start);
        }

        char* buffer = (char*)malloc(sizeof(char) * (substring_length + 2));
        sprintf_s(buffer, substring_length + 1, "%.*ls", (int)substring_length, (wchar_t*)substring_start);

        if (i == 0) {
            jsonObjResp[std::to_string(unamed_match_pos)] = {
                {"match", buffer},
                {"offset_start", (substring_length > 0 ? ovector[2 * i] : 0)},
                {"offset_end", (substring_length > 0 ? ovector[2 * i + 1] : 0)},
                {"offset_length", substring_length}
            };
            jsonObjResp[std::to_string(unamed_match_pos)]["groups"] = { nlohmann::json::object() };
            jsonObjResp[std::to_string(unamed_match_pos)]["named_groups"] = { nlohmann::json::object() };
        } else {
            jsonObjResp[std::to_string(unamed_match_pos)]["groups"][std::to_string(i)] = {
                {"text", buffer},
                {"offset_start", (substring_length > 0 ? ovector[2 * i] : 0)},
                {"offset_end", (substring_length > 0 ? ovector[2 * i + 1] : 0)},
                {"offset_length", substring_length}
            };
        }

        free(buffer);
#endif
    }

    if (rc > 0) {
        unamed_match_pos++;
    }


    /**************************************************************************
    * That concludes the basic part of this demonstration program. We have    *
    * compiled a pattern, and performed a single match. The code that follows *
    * shows first how to access named substrings, and then how to code for    *
    * repeated matches on the same subject.                                   *
    **************************************************************************/

    /* See if there are any named substrings, and if so, show them by name. First
    we have to extract the count of named parentheses from the pattern. */

    (void) pcre2_pattern_info(
        re,                   /* the compiled pattern */
        PCRE2_INFO_NAMECOUNT, /* get the number of named substrings */
        &namecount);          /* where to put the answer */

    if (namecount <= 0) { 
        if (DEBUG_ON) {
            printf("No named substrings\n");
        }
    } else {
        PCRE2_SPTR tabptr;
        if (DEBUG_ON) {
            printf("Named substrings\n");
        }

        /* Before we can access the substrings, we must extract the table for
        translating names to numbers, and the size of each entry in the table. */

        (void)pcre2_pattern_info(
            re,                       /* the compiled pattern */
            PCRE2_INFO_NAMETABLE,     /* address of the table */
            &name_table);             /* where to put the answer */

        (void)pcre2_pattern_info(
            re,                       /* the compiled pattern */
            PCRE2_INFO_NAMEENTRYSIZE, /* size of each entry in the table */
            &name_entry_size);        /* where to put the answer */

          /* Now we can scan the table and, for each entry, print the number, the name,
          and the substring itself. In the 8-bit library the number is held in two
          bytes, most significant first. */

        tabptr = name_table;
        for (i = 0; i < namecount; i++)
        {
#if PCRE2_CODE_UNIT_WIDTH == 8
            int n = (tabptr[0] << 8) | tabptr[1];
            printf("(%d) %*s: %.*s\n", n, name_entry_size - 3, tabptr + 2,
                (int)(ovector[2 * n + 1] - ovector[2 * n]), subject + ovector[2 * n]);
            key = std::string((char*)tabptr + 2);
            std::string tmps2 = std::string((char*)(subject + ovector[2 * n]));
            value = tmps2.substr(0, ovector[2 * n + 1] - ovector[2 * n]);
            std::cout << "key: " << key << "\tvalue: " << value << std::endl;
#elif PCRE2_CODE_UNIT_WIDTH == 16 || PCRE2_CODE_UNIT_WIDTH == 32
            int n = tabptr[0];
            PCRE2_SPTR groupname_start = tabptr + 1;
            size_t groupname_length = name_entry_size - 3;
            PCRE2_SPTR substring_start = subject + ovector[2 * n];
            size_t substring_length = ovector[2 * n + 1] - ovector[2 * n];

            if (DEBUG_ON) {
                printf("(%d) %*ls: %.*ls\n", n, groupname_length, (wchar_t*)groupname_start,
                    (int)substring_length, (wchar_t*)substring_start);
            }

            char* buffer = (char*)malloc(sizeof(char) * (substring_length + 2));
            sprintf_s(buffer, substring_length + 1, "%.*ls", (int)substring_length, (wchar_t*)substring_start);

            char* bufferGroupName = (char*)malloc(sizeof(char) * (groupname_length + 3));
            sprintf_s(bufferGroupName, groupname_length + 2, "%.*ls", (int)groupname_length + 1, (wchar_t*)groupname_start);
                
            jsonObjResp[std::to_string(named_match_pos)]["named_groups"][std::to_string(n)] = {
                {"name", bufferGroupName},
                {"text", buffer},
                {"offset_start", (substring_length > 0 ? ovector[2 * i] : 0)},
                {"offset_end", (substring_length > 0 ? ovector[2 * i + 1] : 0)},
                {"offset_length", substring_length}
            };

            free(buffer);
            free(bufferGroupName);
            
#endif
            tabptr += name_entry_size;

            //jsonObjResp["matches"].push_back({
            //});
        }

        named_match_pos++;
    }


    /*************************************************************************
    * If the "-g" option was given on the command line, we want to continue  *
    * to search for additional matches in the subject string, in a similar   *
    * way to the /g option in Perl. This turns out to be trickier than you   *
    * might think because of the possibility of matching an empty string.    *
    * What happens is as follows:                                            *
    *                                                                        *
    * If the previous match was NOT for an empty string, we can just start   *
    * the next match at the end of the previous one.                         *
    *                                                                        *
    * If the previous match WAS for an empty string, we can't do that, as it *
    * would lead to an infinite loop. Instead, a call of pcre2_match() is    *
    * made with the PCRE2_NOTEMPTY_ATSTART and PCRE2_ANCHORED flags set. The *
    * first of these tells PCRE2 that an empty string at the start of the    *
    * subject is not a valid match; other possibilities must be tried. The   *
    * second flag restricts PCRE2 to one match attempt at the initial string *
    * position. If this match succeeds, an alternative to the empty string   *
    * match has been found, and we can print it and proceed round the loop,  *
    * advancing by the length of whatever was found. If this match does not  *
    * succeed, we still stay in the loop, advancing by just one character.   *
    * In UTF-8 mode, which can be set by (*UTF) in the pattern, this may be  *
    * more than one byte.                                                    *
    *                                                                        *
    * However, there is a complication concerned with newlines. When the     *
    * newline convention is such that CRLF is a valid newline, we must       *
    * advance by two characters rather than one. The newline convention can  *
    * be set in the regex by (*CR), etc.; if not, we must find the default.  *
    *************************************************************************/

    if (!find_all)     /* Check for -g */
    {
        pcre2_match_data_free(match_data);  /* Release the memory that was used */
        pcre2_code_free(re);                /* for the match data and the pattern. */
        return 0;                           /* Exit the program. */
    }

    /* Before running the loop, check for UTF-8 and whether CRLF is a valid newline
    sequence. First, find the options with which the regex was compiled and extract
    the UTF state. */

    (void)pcre2_pattern_info(re, PCRE2_INFO_ALLOPTIONS, &option_bits);
    utf8 = (option_bits & PCRE2_UTF) != 0;

    /* Now find the newline convention and see whether CRLF is a valid newline
    sequence. */

    (void)pcre2_pattern_info(re, PCRE2_INFO_NEWLINE, &newline);
    crlf_is_newline = newline == PCRE2_NEWLINE_ANY ||
        newline == PCRE2_NEWLINE_CRLF ||
        newline == PCRE2_NEWLINE_ANYCRLF;

    /* Loop for second and subsequent matches */
    for (;;)
    {
        uint32_t options = 0;                    /* Normally no options */
        PCRE2_SIZE start_offset = ovector[1];  /* Start at end of previous match */

        /* If the previous match was for an empty string, we are finished if we are
        at the end of the subject. Otherwise, arrange to run another match at the
        same point to see if a non-empty match can be found. */

        if (ovector[0] == ovector[1])
        {
            if (ovector[0] == subject_length) break;
            options = PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
        }

        /* Run the next matching operation */

        rc = pcre2_match(
            re,                   /* the compiled pattern */
            subject,              /* the subject string */
            subject_length,       /* the length of the subject */
            start_offset,         /* starting offset in the subject */
            options,              /* options */
            match_data,           /* block for storing the result */
            NULL);                /* use default match context */

          /* This time, a result of NOMATCH isn't an error. If the value in "options"
          is zero, it just means we have found all possible matches, so the loop ends.
          Otherwise, it means we have failed to find a non-empty-string match at a
          point where there was a previous empty-string match. In this case, we do what
          Perl does: advance the matching position by one character, and continue. We
          do this by setting the "end of previous match" offset, because that is picked
          up at the top of the loop as the point at which to start again.
          There are two complications: (a) When CRLF is a valid newline sequence, and
          the current position is just before it, advance by an extra byte. (b)
          Otherwise we must ensure that we skip an entire UTF character if we are in
          UTF mode. */

        if (rc == PCRE2_ERROR_NOMATCH)
        {
            if (options == 0) { /* All matches found */
                break; 
            }                    
            ovector[1] = start_offset + 1;              /* Advance one code unit */
            if (crlf_is_newline &&                      /* If CRLF is newline & */
                start_offset < subject_length - 1 &&    /* we are at CRLF, */
                subject[start_offset] == '\r' &&
                subject[start_offset + 1] == '\n')
                ovector[1] += 1;                          /* Advance by one more. */
            else if (utf8)                              /* Otherwise, ensure we */
            {                                         /* advance a whole UTF-8 */
                while (ovector[1] < subject_length)       /* character. */
                {
#if PCRE2_CODE_UNIT_WIDTH == 8
                    if ((subject[ovector[1]] & 0xc0) != 0x80)
#elif PCRE2_CODE_UNIT_WIDTH == 16
                    if ((subject[ovector[1]] & 0xfc00) != 0xdc00)
#endif
                        break;
                    ovector[1] += 1;
                }
            }
            continue;    /* Go round the loop again */
        }

        /* Other matching errors are not recoverable. */

        if (rc < 0)
        {
            printf("Matching error %d\n", rc);
            pcre2_match_data_free(match_data);
            pcre2_code_free(re);
            return 1;
        }

        /* Match succeded */
        if (DEBUG_ON) {
            printf("\nMatch succeeded again at offset %d\n", (int)ovector[0]);
        }

        /* The match succeeded, but the output vector wasn't big enough. This
        should not happen. */

        if (rc == 0)
            printf("ovector was not big enough for all the captured substrings\n");

        /* As before, show substrings stored in the output vector by number, and then
        also any named substrings. */

        for (i = 0; i < rc; i++)
        {
            PCRE2_SPTR substring_start = subject + ovector[2 * i];
            size_t substring_length = ovector[2 * i + 1] - ovector[2 * i];
#if PCRE2_CODE_UNIT_WIDTH == 8
            printf("%2d: %.*s\n", i, (int)substring_length, (char*)substring_start);
            std::string tmps1 = std::string((char*)substring_start);
            value = tmps1.substr(0, substring_length);
            std::cout << "value: " << value << std::endl;
#elif PCRE2_CODE_UNIT_WIDTH == 16 || PCRE2_CODE_UNIT_WIDTH == 32
            if (DEBUG_ON) {
                printf("%2d: %.*ls\n", i, (int)substring_length, (wchar_t*)substring_start);
            }

            char* buffer = (char*)malloc(sizeof(char) * (substring_length + 2));
            sprintf_s(buffer, substring_length + 1, "%.*ls", (int)substring_length, (wchar_t*)substring_start);

            if (i == 0) {
                jsonObjResp[std::to_string(unamed_match_pos)] = {
                    {"match", buffer},
                    {"offset_start", (substring_length > 0 ? ovector[2 * i] : 0)},
                    {"offset_end", (substring_length > 0 ? ovector[2 * i + 1] : 0)},
                    {"offset_length", substring_length}
                };
                jsonObjResp[std::to_string(unamed_match_pos)]["groups"] = { nlohmann::json::object() };
                jsonObjResp[std::to_string(unamed_match_pos)]["named_groups"] = { nlohmann::json::object() };
            }
            else {
                jsonObjResp[std::to_string(unamed_match_pos)]["groups"][std::to_string(i)] = {
                    {"text", buffer},
                    {"offset_start", (substring_length > 0 ? ovector[2 * i] : 0)},
                    {"offset_end", (substring_length > 0 ? ovector[2 * i + 1] : 0)},
                    {"offset_length", substring_length}
                };
            }

            free(buffer);
#endif
        }

        if (rc > 0) {
            unamed_match_pos++;
        }

        if (namecount <= 0) { 
            if (DEBUG_ON) {
                printf("No named substrings\n");
            }
        } else
        {
            PCRE2_SPTR tabptr = name_table;
            if (DEBUG_ON) {
                printf("Named substrings\n");
            }
            for (i = 0; i < namecount; i++)
            {
#if PCRE2_CODE_UNIT_WIDTH == 8
                int n = (tabptr[0] << 8) | tabptr[1];
                printf("(%d) %*s: %.*s\n", n, name_entry_size - 3, tabptr + 2,
                    (int)(ovector[2 * n + 1] - ovector[2 * n]), subject + ovector[2 * n]);
                key = std::string((char*)tabptr + 2);
                //~ std::string tmps2 = std::string((char*)(subject + ovector[2*n]));
                //~ value = tmps2.substr(0, ovector[2*n+1] - ovector[2*n]);
                value = std::string((char*)(subject + ovector[2 * n]), ovector[2 * n + 1] - ovector[2 * n]);
                std::cout << "key: " << key << "\tvalue: " << value << std::endl;
#elif PCRE2_CODE_UNIT_WIDTH == 16 || PCRE2_CODE_UNIT_WIDTH == 32
                int n = tabptr[0];
                PCRE2_SPTR groupname_start = tabptr + 1;
                size_t groupname_length = name_entry_size - 3;
                PCRE2_SPTR substring_start = subject + ovector[2 * n];
                size_t substring_length = ovector[2 * n + 1] - ovector[2 * n];

                if (DEBUG_ON) {
                    printf("(%d) %*ls: %.*ls\n", n, groupname_length, (wchar_t*)groupname_start,
                        (int)substring_length, (wchar_t*)substring_start);
                }

                char* buffer = (char*)malloc(sizeof(char) * (substring_length + 2));
                sprintf_s(buffer, substring_length + 1, "%.*ls", (int)substring_length, (wchar_t*)substring_start);

                char* bufferGroupName = (char*) malloc(sizeof(char) * (groupname_length + 3));
                sprintf_s(bufferGroupName, groupname_length + 2, "%.*ls", (int)groupname_length + 1, (wchar_t*)groupname_start);

                jsonObjResp[std::to_string(named_match_pos)]["named_groups"][std::to_string(n)] = {
                    {"name", bufferGroupName},
                    {"text", buffer},
                    {"offset_start", (substring_length > 0 ? ovector[2 * i] : 0)},
                    {"offset_end", (substring_length > 0 ? ovector[2 * i + 1] : 0)},
                    {"offset_length", substring_length}
                };

                free(buffer);
                free(bufferGroupName);

#endif
                tabptr += name_entry_size;
            }
            named_match_pos++;
        }
        //jsonObjResp["matches"].push_back({
        //});
    }      /* End of loop to find second and subsequent matches */

    if (DEBUG_ON) {
        printf("\n");
    }
    printf("%s", jsonObjResp.dump(2).c_str());
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
    return 0;
}
