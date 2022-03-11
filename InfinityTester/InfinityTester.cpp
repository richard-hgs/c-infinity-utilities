// InfinityTester.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>

#define TYPE_CHARPTR 0       // n bytes
#define TYPE_UINT8 1         // 1 bytes
#define TYPE_UINT16 2        // 2 bytes
#define TYPE_UINT32 3        // 4 bytes

unsigned long ultoa(unsigned long value, char* str, unsigned char radix) {
    unsigned long written = 0;
    const unsigned int NUMBER_OF_DIGITS = 32;
    unsigned int index;
    unsigned char ch;
    unsigned char buffer[NUMBER_OF_DIGITS];  /* space for NUMBER_OF_DIGITS + '\0' */

    index = NUMBER_OF_DIGITS;

    do {
        ch = '0' + (value % radix);
        if (ch > (unsigned char)'9') {
            ch += 'a' - '9' - 1;
        }
        buffer[--index] = ch;
        value /= radix;
    } while (value != 0);

    do {
        *str++ = buffer[index++];
        written++;
    } while (index < NUMBER_OF_DIGITS);

    *str = 0;  /* string terminator */

    return written;
}

long ltoa(long value, char* str, unsigned char radix) {
    if (value < 0 && radix == 10) {
        *str++ = '-';
        value = -value;
    }

    return (long)ultoa((unsigned long)value, str, radix);
}

int itoa_s(int value, unsigned char radix, char* str, int size) {
    return (int) ltoa(value, str, radix);
}

int	atoi(char* str) {
    int neg;
    int num;
    int i;

    i = 0;
    neg = 1;
    num = 0;
    while (str[i] <= ' ')
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
        {
            neg *= -1;
        }
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        num = num * 10 + (str[i] - 48);
        i++;
    }
    return (num * neg);
}

unsigned int uitoa_s(unsigned int value, unsigned char radix, char* str, int size) {
    return ultoa(value, str, radix);
}

void u64toa(uint64_t value, char* str, unsigned char radix) {
    const unsigned int NUMBER_OF_DIGITS = 64;
    unsigned int index;
    unsigned char ch;
    unsigned char buffer[NUMBER_OF_DIGITS];  /* space for NUMBER_OF_DIGITS + '\0' */

    index = NUMBER_OF_DIGITS;

    do {
        ch = '0' + (value % radix);
        if (ch > (unsigned char)'9') {
            ch += 'a' - '9' - 1;
        }
        buffer[--index] = ch;
        value /= radix;
    } while (value != 0);

    do {
        *str++ = buffer[index++];
    } while (index < NUMBER_OF_DIGITS);

    *str = 0;  /* string terminator */
}

void* memcpy_r(void* dst, const void* src, uint32_t size) {
    uint32_t* wdst = (uint32_t*) dst;
    const uint32_t* wsrc = (uint32_t*) src;

    // word per word copy if both addresses aligned
    if (!((uint32_t)wdst & 3) && !((uint32_t)wsrc & 3)) {
        while (size > 3) {
            *wdst-- = *wsrc--;
            size -= 4;
        }
    }

    unsigned char* cdst = (unsigned char*)wdst;
    unsigned char* csrc = (unsigned char*)wsrc;

    // byte per byte for last bytes (or not aligned)
    while (size--) {
        *cdst-- = *csrc--;
    }
    return dst;
}

//void muitoa_s(unsigned int value, unsigned char radix, char* str, int size, char leadChar, int leadCount) {
//    // uint32_t strOffset = (uint32_t) str;
//    int written = uitoa_s(value, radix, str, size);
//    if (written < leadCount && written < size - 1) {
//        // leadCount = 2; written = 1; sizeToCpy = 1;
//        int leadSize = leadCount - written;
//        memcpy_r(str + (written - 1) + leadSize, str + (written - 1), written);
//        memset(str, leadChar, leadSize);
//        str[written + leadSize] = '\0';
//    }
//}

void mitoa_s(int value, unsigned char radix, char* str, int size, char leadChar, int leadCount) {
    uint32_t strOffset = (uint32_t) str;
    int written = itoa_s(value, radix, str, size);
    if (written < leadCount && written < size - 1) {
        // leadCount = 2; written = 1; sizeToCpy = 1;
        int leadSize = leadCount - written;
        memcpy_r(str + (written - 1) + leadSize, str + (written - 1), written);
        memset(str, leadChar, leadSize);
        str[written + leadSize] = '\0';
    }
    /*muitoa_s(value, radix, str, size, leadChar, leadCount);*/
}

int snprintf_cpy(char* buffer, size_t bufferSize, size_t startOffset, const char* instr, size_t inlen) {
    const unsigned char* bytes = (const unsigned char*) instr;
    for (size_t i = 0; i < inlen; i++) {
        unsigned char byteAt = bytes[i];
        if (byteAt == EOF || (startOffset + i) >= bufferSize - 1) {
            return 0;
        } else {
            buffer[startOffset + i] = instr[i];
        }
    }
    return 1;
}

int m_snprintf(char* buffer, size_t startOffset, size_t size, const char* format, ...) {
    va_list ap;
    size_t written = 0;
    char leadingNumberChar = 0;
    int leadingNumberCount = 0;

    buffer = buffer + startOffset;
    size -= startOffset;

    va_start(ap, format);
    while (*format != 0) {
        unsigned maxrem = INT_MAX - written;

        int addToWritten = 0;

        if (leadingNumberCount == 0 && (format[0] != '%' || format[1] == '%')) {
            if (format[0] == '%') {
                format++;
            }
            unsigned amount = 1;
            while (format[amount] && format[amount] != '%') {
                amount++;
            }

            printf("%i - %c\n", format[amount], amount);

            // If custom format passed fix the format
            if (format[amount] && format[amount] == '%') {
                if (format[amount + 1] && format[amount + 1] == '0') {
                    leadingNumberChar = '0';
                    addToWritten++;

                    printf("%%02 found: ");

                    char strNum[2] = {'\0'};

                    while (format[amount + 1 + addToWritten] && format[amount + 1 + addToWritten] >= '0' && format[amount + 1 + addToWritten] <= '9') {
                        strNum[addToWritten - 1] = format[amount + 1 + addToWritten];
                        leadingNumberCount = atoi(strNum);
                        addToWritten++;

                        if (addToWritten > 6) {
                            // Block infinite loop
                            break;
                        }
                    }
                }
            }

            if (maxrem < amount) {
                /* TODO: Set errno to EOVERFLOW. */
                return written;
            }
            if (!snprintf_cpy(buffer, size, written, format, amount)) {
                return written;
            }
            format += (amount + addToWritten);
            written += (amount + addToWritten);
            continue;
        }

        const char* format_begun_at = format++;

        printf("format: %s\n", format_begun_at);
        printf("leadingNumberChar: %c\n", leadingNumberChar);
        printf("leadingNumberCount: %i\n", leadingNumberCount);

        if (*format == 'c') { /* Character */
            format++;
            char c = (char)va_arg(ap, int);
            if (!maxrem) {
                /* TODO: Set errno to EOVERFLOW. */
                return -1;
            }
            if (written >= size - 1) {
                return -1;
            } else {
                buffer[written] = c;
            }
            // kputc(c);
            written++;
        } else if (*format == 's') { /* String */
            format++;
            const char* str = va_arg(ap, const char*);
            unsigned len = strlen(str);
            if (maxrem < len) {
                /* TODO: Set errno to EOVERFLOW. */
                return -1;
            }
            if (!snprintf_cpy(buffer, size, written, str, len)) {
                return -1;
            }
            written += len;
        } else if (*format == 'd' || *format == 'i') { /* Signed Integer */
            format++;
            int c = va_arg(ap, int);
            char str[32] = { 0 };
            unsigned len;

            mitoa_s(c, 10, str, 32, leadingNumberChar, leadingNumberCount);
            // itoa_s(c, 10, str, 32);
            len = strlen(str);
            if (maxrem < len) {
                /* TODO: Set errno to EOVERFLOW. */
                return -1;
            }
            if (!snprintf_cpy(buffer, size, written, str, len)) {
                return -1;
            }
            written += len;
        } else if (*format == 'x' || *format == 'X') { /* Hexadecimal */
            format++;
            int c = va_arg(ap, int);
            char str[32] = { 0 };
            unsigned len;

            mitoa_s(c, 16, str, 32, leadingNumberChar, leadingNumberCount);
            // itoa_s(c, 16, str, 32);
            len = strlen(str);
            if (maxrem < len) {
                /* TODO: Set errno to EOVERFLOW. */
                return -1;
            }
            if (!snprintf_cpy(buffer, size, written, str, len)) {
                return -1;
            }
            written += len;
        } else if (*format == 'p') { /* Pointer address Hexadecimal */
            format++;
            void* c = va_arg(ap, void*);
            // int **pp = &c;
            char str[32] = { 0 };
            unsigned len;

            uitoa_s((unsigned long)c, 16, str, 32);
            len = strlen(str);
            // int address = (int) &pp;
            if (maxrem < len) {
                /* TODO: Set errno to EOVERFLOW. */
                return -1;
            }
            if (!snprintf_cpy(buffer, size, written, str, len)) {
                return -1;
            }
            written += len;
        } else if (*format == 'u') {
            format++;
            unsigned int c = va_arg(ap, unsigned int);
            // int **pp = &c;
            char str[32] = { 0 };
            unsigned len;

            uitoa_s(c, 10, str, 32);
            len = strlen(str);
            // int address = (int) &pp;
            if (maxrem < len) {
                /* TODO: Set errno to EOVERFLOW. */
                return -1;
            }
            if (!snprintf_cpy(buffer, size, written, str, len)) {
                return -1;
            }
            written += len;
        } else if (*format == 'o') {
            format++;
            unsigned int c = va_arg(ap, unsigned int);
            // int **pp = &c;
            char str[32] = { 0 };
            unsigned len;

            uitoa_s(c, 8, str, 32);
            len = strlen(str);
            // int address = (int) &pp;
            if (maxrem < len) {
                /* TODO: Set errno to EOVERFLOW. */
                return -1;
            }
            if (!snprintf_cpy(buffer, size, written, str, len)) {
                return -1;
            }
            written += len;
        } else { /* Normal character */
            format = format_begun_at;
            unsigned len = strlen(format);
            if (maxrem < len) {
                /* TODO: Set errno to EOVERFLOW. */
                return -1;
            }
            if (!snprintf_cpy(buffer, size, written, format, len)) {
                return -1;
            }
            written += len;
            format += len;
        }
        leadingNumberChar = 0;
        leadingNumberCount = 0;
    }
    va_end(ap);

    if (written < size - 1) {
        buffer[written] = '\0';
        // kprintf("\nzeroWritten: %i\n", written);
    }

    written++;

    return written;
}

void skipBuffer(size_t* inBuffOffset, size_t lenToSkip) {
    (*inBuffOffset) += lenToSkip;
}

void writeBufferInBuffer(char** buffer, size_t* bufferSize, size_t* offset, const void* inBuffer, size_t* inBuffOffset, size_t lenToWrite, int dataType, const char* format) {
    int written = -1;
    if (dataType == TYPE_UINT32) {
        uint32_t intValue = 0;
        memcpy(&intValue, (void*)(((size_t)inBuffer) + (*inBuffOffset)), lenToWrite);
        if (format == NULL) {
            written = uitoa_s(intValue, 10, (*buffer) + (*offset), *bufferSize);
        } else {
            int myVar = 2;
            written = m_snprintf(*buffer, *offset, *bufferSize, format, 2);
        }
    } else if (dataType == TYPE_UINT8) {
        uint8_t intValue = 0;
        memcpy(&intValue, (void*)(((size_t)inBuffer) + (*inBuffOffset)), lenToWrite);
        if (format == NULL) {
            written = itoa_s(intValue, 10, (*buffer) + (*offset), *bufferSize);
        } else {
            written = m_snprintf(*buffer, *offset, *bufferSize, format, intValue);
        }
    } else {
        memcpy((*buffer) + (*offset), (void*)(((size_t) inBuffer) + (*inBuffOffset)), lenToWrite);
    }

    if (written > -1) {
        (*offset) += written;
        (*inBuffOffset) += lenToWrite;
    } else {
        (*offset) += lenToWrite;
        (*inBuffOffset) += lenToWrite;
    }
}

void writeBufferInBuffer(char** buffer, size_t* bufferSize, size_t* offset, const void* inBuffer, size_t* inBuffOffset, size_t lenToWrite, int dataType) {
    writeBufferInBuffer(buffer, bufferSize, offset, inBuffer, inBuffOffset, lenToWrite, dataType, NULL);
}

void writeStrInBuffer(char** buffer, size_t* bufferSize, size_t* offset, const char* strToWrite) {
    size_t lenToWrite = strlen(strToWrite);
    memcpy((*buffer) + (*offset), strToWrite, lenToWrite);
    (*offset) += lenToWrite;
}

void acpiDecompileDsdt() {
    LPCSTR lpFileName = "D:\\Programming\\VSProjects\\OperatingSystems\\InfinityOS\\docs\\iasl-win-20210930\\dsdt.dat";
    LARGE_INTEGER liFileSize;

    HANDLE hFile = CreateFileA(
        lpFileName,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("invalid handle file\n");
        CloseHandle(hFile);
        return;
    }


    DWORD fSizeResult = GetFileSizeEx(
        hFile,
        &liFileSize
    );

    if (fSizeResult == INVALID_FILE_SIZE) {
        printf("invalid file size\n");
        CloseHandle(hFile);
        return;
    }

    int intFSize = (int)liFileSize.QuadPart;

    unsigned char* lpBuffer = (unsigned char*)malloc(sizeof(unsigned char) * intFSize + 1);

    size_t acpiDumpSize = intFSize;
    char* acpiDump = (char*)malloc(sizeof(unsigned char) * acpiDumpSize + 1);
    size_t acpiDumpOffset = 0;
    size_t acpiInBufferOffset = 0;

    BOOL readFileResult = ReadFile(
        hFile,
        lpBuffer,
        intFSize,
        NULL,
        NULL
    );

    if (!readFileResult) {
        printf("could not read file\n");
        CloseHandle(hFile);
        return;
    }



    // Definition Block
    // -------------------- COMMENT ---------------------
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, "/*\n");
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, " * InfinityOS - Acpi Dump\n");
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, " */\n");
    // -------------------- COMMENT ---------------------

    // ---------------- DEFINITION BLOCK ----------------
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, "Definition Block (\"\", ");
    // Signature
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, "\"");
    writeBufferInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, lpBuffer, &acpiInBufferOffset, 4, TYPE_CHARPTR);
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, "\"");
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, ", ");
    // Skip Length
    skipBuffer(&acpiInBufferOffset, 4);
    // Revision
    writeBufferInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, lpBuffer, &acpiInBufferOffset, 1, TYPE_UINT8);
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, ", ");
    // Skip Checksum
    skipBuffer(&acpiInBufferOffset, 1);
    // OEM ID
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, "\"");
    writeBufferInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, lpBuffer, &acpiInBufferOffset, 6, TYPE_CHARPTR);
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, "\", ");
    // OEM TABLE ID
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, "\"");
    writeBufferInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, lpBuffer, &acpiInBufferOffset, 8, TYPE_CHARPTR);
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, "\", ");
    // OEM REVISION
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, "0x");
    writeBufferInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, lpBuffer, &acpiInBufferOffset, 4, TYPE_UINT32, "%02d");
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, ") {\n");
    // Skip Creator ID
    skipBuffer(&acpiInBufferOffset, 4);
    // Skip Creator Revision
    skipBuffer(&acpiInBufferOffset, 4);
    // BEGIN DEFINITION BLOCK

    // DEFINITION BLOCK END
    writeStrInBuffer(&acpiDump, &acpiDumpSize, &acpiDumpOffset, "}\n\n\n");
    // ---------------- DEFINITION BLOCK ----------------

    //while (acpiInBufferOffset < 100) {

    //}

    acpiDump[acpiDumpOffset] = '\0';

    printf("fSize: %i\n", intFSize);
    printf("acpiDumpOffset: %i\n", acpiDumpOffset);
    printf("acpiInBufferOffset: %i\n", acpiInBufferOffset);
    printf(acpiDump);

    //char* myText = (char*) malloc(101);
    ////mitoa_s(2, 10, myText, 100, '0', 6);

    //snprintf_cpy(myText, 100, 0, "0x0000002", 9);
    //printf(myText);

    free(lpBuffer);
    CloseHandle(hFile);
}

int main() {
    // const char* myText = (char*) malloc(20 * sizeof(char));

    // std::cout << "currentProcessId: " << std::to_string(GetCurrentProcessId()) << std::endl;

    acpiDecompileDsdt();

    return _getch();
}