#ifdef _WIN32
#pragma warning(disable:4786)
#endif
#include "pch.h"
#include <assert.h>
#include <map>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>
#ifdef DEBUG_MEM
#include <windows.h>
// #include "MapFile.h"
#endif

#include "debug.h"

#ifndef _WIN32
#define _vsnprintf_s vsnprintf_s
#endif

static const int DEBUG_KEYS[] = { DMEMLEAK, 0 };
static const int* Keys = DEBUG_KEYS;
FILE* _file = 0;

static const char* fname_delete;
static int lineno_delete;

typedef struct _CrashPosition_t {
    const char* file;
    const char* function;
    int line;
} CrashPosition_t;

#ifdef DEBUG
void odsHandler( const char* str )
{
    const size_t tcSize = 0;

    const_cast<size_t&>(tcSize) = strlen(str);

    TCHAR tcStr[tcSize + 1];
    size_t tcConverted;
    mbstowcs_s(&tcConverted, tcStr, str, tcSize);
    tcStr[tcSize] = '\0';

    #ifdef _WIN32
        OutputDebugString(tcStr);
    #endif
}

static DebugHandler handler = odsHandler;

INFINITY_CRASHLYTICS_EXTERN INFINITY_CRASHLYTICS_API void dbgSetHandler(DebugHandler handlers)
{
    if ( handler == 0 ) {
        handler = odsHandler;
    } else {
        handler = handlers;
    }

    //SharedMemInfo sharedMemInfo = SharedMemInfo();
    //sharedMemInfo.debugHandler = handler;

    //SetSharedMem((char*)&sharedMemInfo, sizeof(SharedMemInfo));

    //SharedMemInfo sharedMemInfo2 = SharedMemInfo();

    //GetSharedMem((char*)&sharedMemInfo2, sizeof(SharedMemInfo*));

    // std::cout << "shaedMemInfo: " << sharedMemInfo2._init << std::endl;
}

INFINITY_CRASHLYTICS_EXTERN INFINITY_CRASHLYTICS_API void dbgSetKeys( const int* keys )
{
    Keys = keys;
}


void dbgInitOutput()
{
    char env[13];
    size_t envSizeConverted;
    getenv_s(&envSizeConverted, env, 13, "DEBUG_OUTPUT");
    if ( env == 0 ) {
        return;
    }

    fopen_s(&_file, env, "wt+");
    if ( _file == 0 ) {
        return;
    }

 //   handler = fileHandler;
}

/******************************************************************************
 *****************************************************************************/
void _dbgprint( unsigned short level, const char* msg, ... )
{
    const int* key = Keys;
    int found = 0;
    char buffer[1024];
    int offset = 0;
    va_list args;

    while( *key ) {
        if ( *key == level ) {
            found = 1;
            break;
        }
        key++;
    }

    if ( ! found ) {
        return;
    }

    va_start( args, msg );
    _vsnprintf_s( &buffer[offset], sizeof(buffer)/sizeof(*buffer)-1, sizeof(buffer) / sizeof(*buffer) - 1, msg, args );
	buffer[sizeof(buffer)/sizeof(*buffer)-1-1] = 0;
    va_end( args );
    strcat_s( buffer, "\n" );

    //SharedMemInfo sharedMemInfo = {};
    //GetSharedMem((char*) &sharedMemInfo, sizeof(SharedMemInfo));

    //sharedMemInfo.debugHandler(buffer);

    handler(buffer);
}
#endif

#ifdef DEBUG_MEM

#undef malloc
#undef calloc
#undef realloc
#undef free
#undef strdup
#undef _strdup
//#undef InvalidateRect

typedef struct _list_entry_t {
    struct _list_entry_t* next;
    struct _list_entry_t* prev;
} list_entry_t;
#define list_init( entry ) \
    (entry)->prev = (entry)->next = entry
#define container_of( ptr, type, member ) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
#define list_entry( ptr, type, member )\
    container_of(ptr, type, member)
#define list_empty( list ) \
    ((list)->next == (list))
#define list_head( list ) \
    (list)->next
#define list_add_tail( list, entry ) \
    (entry)->next = (list); \
    (entry)->prev = (list)->prev; \
    (list)->prev->next = (entry); \
    (list)->prev = (entry) 
#define list_remove( entry ) \
    assert( (entry)->next != 0 ); \
    (entry)->prev->next = (entry)->next; \
    (entry)->next->prev = (entry)->prev;

static const unsigned SENTRY = 0x12345678;
#define STACK_SIZE 5

struct MemBlock {
    unsigned sentry;
    list_entry_t list;
    size_t size;
    char* file;
    int line;
    unsigned stack[STACK_SIZE];
};

list_entry_t _blockList;

static bool _init = false;
static CRITICAL_SECTION _cs;

// static MapFile* _mapFile = 0;


/******************************************************************************
 *****************************************************************************/
static void
dump_blocks()
{
    list_entry_t* entry = list_head( &_blockList );
    while( entry != &_blockList ) {
        MemBlock* block = list_entry( entry, MemBlock, list );
        dbgprint(( DMEMLEAK, "Leaked %d bytes from %s:%d [%p]",
                    block->size, block->file, block->line, block + 1
                 ));
        
        entry = entry->next;
    }

    if ( list_empty(&_blockList ) ) {
        dbgprint(( DMEMLEAK, "No memory leaks detected." ));
    }
}

/******************************************************************************
 *****************************************************************************/
static class Heap {
public:
    Heap() {
        TCHAR tcBuffer[1024];
        char buffer[1024];

        GetModuleFileName( GetModuleHandle(NULL), tcBuffer, 1024);

        size_t tcBuffConverted;
        wcstombs_s(&tcBuffConverted, buffer, tcBuffer, 1024);

        buffer[strlen(buffer) + 1] = '\0';

        char* slash = strrchr( buffer, '\\' );
        if ( slash ) {
            slash++;
        } else {
            slash = buffer;
        }

        char* dot = strrchr(buffer, '.');
        if ( dot ) {
            *dot = 0;
        }
        // std::cout << "tcBuffer:" << (sizeof(slash) / sizeof(slash[0])) << std::endl;
        strcat_s(slash, strlen(slash) + 5, ".map");

        //_mapFile = new MapFile();
        //_mapFile->read( slash );
        list_init(&_blockList);

        InitializeCriticalSection(&_cs);
        
        _init = true;

        const int* key = Keys;
        int found = 0;

        while (*key) {
            if (*key == DMEMLEAK) {
                found = 1;
                break;
            }
            key++;
        }

        if (found) {
            // std::cout << "Debug heap starting up." << std::endl;
        }
        
        // dbgprint(( DMEMLEAK, "Debug heap starting up."));
    }

    ~Heap() {
        dbgprint(( DMEMLEAK, "Debug heap shutting down."));
        _init = false;
        DeleteCriticalSection(&_cs);
        // if this assert goes off, we are dumping unfreed memory blocks to
        // the debug window.
        dump_blocks();
        //assert( list_empty(&_blockList ) );
    }
} heap;

/******************************************************************************
 *****************************************************************************/
void del_record( const char* file, int line, void* ptr )
{
    assert(_init);
    if ( ptr == 0 ) {
        return;
    }
    
    MemBlock* block = (MemBlock*)ptr - 1;
    if ( block->sentry != SENTRY ) {
        dbgprint((DMEMORY, "Invalid free of [%08x] in %s:%d\n", 
                    ptr, file, line));
        assert(0);
        return;
    }

    if ( 0 != memcmp( (char*)ptr + block->size, &SENTRY, sizeof( SENTRY ) ) )
    {
        dbgprint(( DMEMORY, "%s:%d: free(%p): Sentry corrupted.", file, line, ptr));
        assert(0);
    }

    free( block->file );

    EnterCriticalSection(&_cs);
    list_remove( &block->list );
    LeaveCriticalSection(&_cs);
    memset( block, 0xdd, sizeof( block + block->size + sizeof(SENTRY) ) );
    free( block );
}

/******************************************************************************
 *****************************************************************************/
INFINITY_CRASHLYTICS_EXTERN INFINITY_CRASHLYTICS_API void _dbgfree( const char* file, int line, void* ptr )
{
    if ( ptr == 0 ) {
        return;
    }

    if ( !_init ) {
        free( ptr );
        return;
    }

    MemBlock* block = (MemBlock*)ptr - 1;
    int size = block->size;

    char key = DMEMORY;
    if (block->sentry != SENTRY) {
        key = DMEMUNHANDLED;
    }

    dbgprint((key, "%s:%d: free( [%p], %d )", file, line, ptr, size));

    if (block->sentry != SENTRY) {
        free(ptr);
    } else {
        del_record(file, line, ptr);
    }
}



/******************************************************************************
 *****************************************************************************/
void* add_record( const char* file, int line, size_t size )
{
    MemBlock* block;
    assert(_init);

    block = (MemBlock*)malloc( sizeof( MemBlock ) + size + 4 );

    if ( block == 0 ) {
        dbgprint(( DMEMORY, "Out of memory." ));
        return 0;
    }

    block->sentry = SENTRY;
    block->size = size;
    block->line = line;
    block->file = _strdup( file );
    if ( 0 == block->file && file ) {
        free( block );
        dbgprint(( DMEMORY, "Out of memory." ));
        return 0;
    }

    memcpy( (char*)block + sizeof(*block) + size, &SENTRY, 
        sizeof( SENTRY ) );

    EnterCriticalSection(&_cs);
    list_add_tail( &_blockList, &block->list );
    LeaveCriticalSection(&_cs);

    return block + 1;
}


/******************************************************************************
 *****************************************************************************/
INFINITY_CRASHLYTICS_EXTERN INFINITY_CRASHLYTICS_API void* _dbgmalloc( const char* file, int line, size_t size )
{
    void* ptr;

    if ( !_init ) {
        return malloc( size );
    }

    ptr = add_record( file, line, size );
    if ( ptr == 0 ) {
        dbgprint(( DMEMORY, "Out of memory." ));
        return 0;
    }

    dbgprint(( DMEMORY, "%s:%d: malloc( %d ) [%p]", file, line, size, ptr ));

    return ptr;
}

/******************************************************************************
 *****************************************************************************/
INFINITY_CRASHLYTICS_EXTERN INFINITY_CRASHLYTICS_API void* _dbgrealloc( const char* file, int line, void* ptr, size_t size )
{
    if ( !_init ) {
        return realloc( ptr, size );
    }

    if ( ptr == 0 ) {
        return _dbgmalloc( file, line, size );
    }

    if ( size == 0 ) {
        del_record( file, line, ptr );
        dbgprint(( DMEMORY, "%s:%d: realloc/free( [%08x], %d )", file, line,
                    ptr, size ));
        return 0;
    }

    MemBlock* block = (MemBlock*)ptr - 1;
    if ( block->sentry != SENTRY ) {
        dbgprint(( DMEMLEAK, "%s:%d: realloc( [%08x], %d ): ptr is bad.",
                    file, line, ptr, size));
        assert( 0 );
        return ptr;
    }

    EnterCriticalSection(&_cs);
    list_remove( &block->list );
    LeaveCriticalSection(&_cs);

    block = (MemBlock*)realloc( block, size + sizeof(MemBlock) + sizeof(SENTRY));
    if ( block == 0 ) {
        block = (MemBlock*)ptr - 1;
        dbgprint(( DMEMORY, "%s:%d: realloc( %08x, %d ): failed.",
                    file, line, ptr, size ));
        list_add_tail( &_blockList, &block->list );
        return 0;
    }

	ptr = block + 1;
    block->size = size;

    memcpy( (char*)block + sizeof(*block) + size, &SENTRY, 
        sizeof( SENTRY ) );

    EnterCriticalSection(&_cs);
    list_add_tail( &_blockList, &block->list );
    LeaveCriticalSection(&_cs);

    dbgprint(( DMEMORY, "%s:%d: malloc( %d ) [%p]", file, line, size, ptr ));

    return ptr;
}

/******************************************************************************
 *****************************************************************************/
INFINITY_CRASHLYTICS_EXTERN INFINITY_CRASHLYTICS_API char* _dbgstrdup( const char* file, int line, const char* str )
{
    if ( !_init ) {
        return _strdup( str );
    }
    int size;
    char* ptr;
    assert( str );

    size = strlen( str ) + 1;
    ptr = (char*)add_record(file, line, size);
    if ( ptr == 0 ) {
        dbgprint(( DMEMORY, "Out of memory." ));
        return 0;
    }

    dbgprint(( DMEMORY, "%s:%d: strdup( \"%s\" ) [%p]", file, line, str, 
            ptr ));

    strcpy_s( ptr, size, str );

    return ptr;
}

/******************************************************************************
 *****************************************************************************/
//static int 
//GetCallStack( unsigned* stack, int max )
//{
//    unsigned* my_ebp = 0;
//    int i;
//
//    __asm {
//        mov eax, ebp
//        mov dword ptr [my_ebp], eax;
//    }
//
//    if ( IsBadReadPtr( my_ebp + 1, 4 ) ) {
//        return 0;
//    }
//
//    stack[0] = *(my_ebp + 1);
//    for ( i = 1; i < max; i++ ) {
//        unsigned addr;
//        if ( IsBadReadPtr( my_ebp, 4 ) ) {
//            break;
//        }
//        my_ebp = (unsigned*)(*my_ebp);
//
//        if ( IsBadReadPtr( my_ebp + 1, 4 ) ) {
//            break;
//        }
//
//        addr = *(my_ebp + 1);
//        if ( addr ) {
//            stack[i] = addr;
//        } else {
//			break;
//		}
//    }
//
//    return i;
//}

/******************************************************************************
  numUp = 0 returns the caller of getFileLine.
 *****************************************************************************/
//static CrashPosition_t getFileLine2(int numUp) {
//    unsigned stack[5];
//    CrashPosition_t pos;
//    int n = GetCallStack(stack, 5);
//    numUp++;
//    pos.function = "unknown function";
//    pos.file = "unknown file";
//    pos.line = 0;
//
//    if ( n >= numUp ) {
//        // _mapFile->getPosition(stack[numUp], &pos );
//    }
//
//    return pos;
//}

/******************************************************************************
 *****************************************************************************/
/*
throwing (1)
void* operator new (std::size_t size) throw (std::bad_alloc);
nothrow (2)
void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) throw();
placement (3)
void* operator new (std::size_t size, void* ptr) throw();
*/
#pragma warning(push)
#pragma warning(disable: 4290)
INFINITY_CRASHLYTICS_EXTERN INFINITY_CRASHLYTICS_API void* _dbgnew(size_t size, const char* filename, int line) throw(std::bad_alloc) {
    static bool recurse = false;
    void* ret;
    CrashPosition_t pos;

    if (recurse || !_init) {
        return malloc(size);
    }

    EnterCriticalSection(&_cs);
    //pos = getFileLine2(1);
    //if (pos.file == 0) {
    //    pos.file = pos.function;
    //}

    pos.file = filename;
    pos.line = line;

    ret = add_record(pos.file, pos.line, size);
    if (ret == 0) {
        dbgprint((DMEMORY, "Out of memory."));
        LeaveCriticalSection(&_cs);
        return 0;
    }

    dbgprint((DMEMORY, "%s:%d: new( %d ) [%p]", pos.file, pos.line, size, ret));
    LeaveCriticalSection(&_cs);
    return ret;
}
#pragma warning(pop)

INFINITY_CRASHLYTICS_EXTERN INFINITY_CRASHLYTICS_API void _dbgdeletep(const char* filename, int line) {
    fname_delete = filename;
    lineno_delete = line;
    return;
}

INFINITY_CRASHLYTICS_EXTERN INFINITY_CRASHLYTICS_API void _dbgdelete(void* ptr) {
    CrashPosition_t pos;

    if (!_init) {
        free(ptr);
        return;
    }

    if (ptr == 0) {
        return;
    }
    EnterCriticalSection(&_cs);

    // pos = getFileLine2(2);
    LeaveCriticalSection(&_cs);

    pos.file = fname_delete;
    pos.line = lineno_delete;

    MemBlock* block = (MemBlock*)ptr - 1;
    int size = block->size;

    char key = DMEMORY;
    if (block->sentry != SENTRY) {
        key = DMEMUNHANDLED;
    }

    dbgprint((key, "%s:%d: delete [%p]", pos.file, pos.line, ptr));

    if (block->sentry != SENTRY) {
        free(ptr);
    } else {
        del_record(pos.file, pos.line, ptr);
    }

    // del_record(pos.file, pos.line, ptr);
    return;
}

//void* _new(size_t size, const char* filename, int line) throw(std::bad_alloc) {
//    std::cout << "new called" << std::endl;
//    return 0;
//}

//void* operator new(size_t size, const char* filename, int line) throw(std::bad_alloc)
//{
//    static bool recurse = false;
//    void* ret;
//    CrashPosition_t pos;
//    if ( recurse || !_init) {
//        return malloc( size );
//    }
//
//    EnterCriticalSection(&_cs);
//    pos = getFileLine(1);
//    if ( pos.file == 0 ) {
//        pos.file = pos.function;
//    }
//
//    ret = add_record( pos.file, pos.line, size );
//    if ( ret == 0 ) {
//        dbgprint(( DMEMORY, "Out of memory." ));
//	    LeaveCriticalSection(&_cs);
//        return 0;
//    }
//
//    dbgprint(( DMEMORY, "%s:%d: new( %d ) [%p]", pos.file, pos.line, size, ret ));
//	    LeaveCriticalSection(&_cs);
//    return ret;
//}


/******************************************************************************
 *****************************************************************************/
//void operator delete( void* ptr ) throw ()
//{
//    CrashPosition_t pos;
//    
//    if ( !_init ) {
//        free( ptr );
//        return;
//    }
//
//    if ( ptr == 0 ) {
//        return;
//    }
//    EnterCriticalSection(&_cs);
//
//    pos = getFileLine(2);
//	    LeaveCriticalSection(&_cs);
//
//    dbgprint(( DMEMORY, "%s:%d: delete [%p]", pos.file, pos.line, ptr ));
//    del_record( pos.file, pos.line, ptr );
//}

/******************************************************************************
 *****************************************************************************/
INFINITY_CRASHLYTICS_EXTERN INFINITY_CRASHLYTICS_API void* _dbgcalloc( const char* file, int line, size_t nmemb, size_t sizem )
{

    if ( !_init ) {
        return calloc( nmemb, sizem );
    }
    void* ptr;

    int size = sizem * nmemb;

    ptr = add_record( file, line, size );

    if ( ptr == 0 ) {
        dbgprint(( DMEMORY, "%s:%d: calloc( %d, %d ) failed.", file, line, 
                    nmemb, sizem ));
        return 0;
    }

    memset( ptr, 0, size );

    dbgprint(( DMEMORY, "%s:%d: calloc( %d, %d ) [%p]", file, line, nmemb,
        sizem, ptr ));

    return ptr;
}

//BOOL _dbgInvalidateRect( HWND hWnd, const RECT* lpRect, BOOL bErase )
//{
//    assert(hWnd != NULL);
//    return InvalidateRect(hWnd,lpRect,bErase);
//}

#endif // DEBUG_MEM