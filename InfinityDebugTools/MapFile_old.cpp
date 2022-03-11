#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>
#include <time.h>
#include "debug.h"

#define CRASH_API __declspec(dllexport)

#include "MapFile.h"

DWORD WINAPI Emulate_UnDecorateSymbolName(
  const TCHAR* DecoratedName,
  PTSTR UnDecoratedName,
  DWORD UndecoratedLength,
  DWORD Flags
)
{
    _tcscpy( UnDecoratedName, DecoratedName );
    return _tcslen( UnDecoratedName );
}

DWORD WINAPI Probe_UnDecorateSymbolName(
  const TCHAR* DecoratedName,
  PTSTR UnDecoratedName,
  DWORD UndecoratedLength,
  DWORD Flags
);

static DWORD (WINAPI *UnDecorateSymbolNamePtr)(
  const TCHAR* DecoratedName,
  PTSTR UnDecoratedName,
  DWORD UndecoratedLength,
  DWORD Flags
) = Probe_UnDecorateSymbolName;

DWORD WINAPI Probe_UnDecorateSymbolName(
  const TCHAR* DecoratedName,
  PTSTR UnDecoratedName,
  DWORD UndecoratedLength,
  DWORD Flags
)
{
    HMODULE hDbghelp = LoadLibrary( TEXT("dbghelp.dll") );
    if ( hDbghelp == 0 ) {
        goto emulate;
    }

    *(FARPROC*)&UnDecorateSymbolNamePtr = 
        (FARPROC)GetProcAddress( hDbghelp, "UnDecorateSymbolName" );

    if ( UnDecorateSymbolNamePtr ) {
        return UnDecorateSymbolNamePtr( DecoratedName,
                UnDecoratedName,
                UndecoratedLength,
                Flags );
    }

emulate:
    UnDecorateSymbolNamePtr = Emulate_UnDecorateSymbolName;
    return UnDecorateSymbolNamePtr( DecoratedName,
            UnDecoratedName,
            UndecoratedLength,
            Flags );
}


const char*
CallStack_Unmangle( const char* str )
{
    static char Buffer[1024];
    static TCHAR TcBuffer[1024];
    TCHAR tcStr[1024];

    mbstowcs(tcStr, str, 1024);

    Buffer[0] = 0;
    TcBuffer[0] = 0;
    UnDecorateSymbolNamePtr( tcStr, TcBuffer, sizeof(TcBuffer) / sizeof(*TcBuffer), 0 );

    wcstombs(Buffer, TcBuffer, 1024);

    return Buffer;
}

MapFile::MapFile()
{
    map_array = 0;
    map_alloc = 0;
    map_used = 0;

    file_array = 0;
    file_alloc = 0;
    file_used = 0;

    line_array = 0;
    line_alloc = 0;
    line_used = 0;
    _imageName = _strdup("");
    _timeStamp = 0;

    map_array_init();
    file_array_init();
    line_array_init();

    _preferred_load_address = 0x400000;

}

MapFile::~MapFile() 
{
    map_array_destroy();
    file_array_destroy();
    line_array_destroy();
    free( _imageName );
}

const char*
MapFile::imageName()
{
    return _imageName;
}

long
MapFile::timeStamp()
{
    return _timeStamp;
}



// ---------------------------------------------------------------------------
// map array

void
MapFile::map_array_init( void )
{
    map_array = (map_entry_t**)malloc( sizeof(map_entry_t*) );
    map_alloc = 1;
    map_used = 0;
}

void
MapFile::map_array_add( map_entry_t* item )
{
    if ( map_used == map_alloc ) {
        map_alloc <<= 1;
        map_array = (map_entry_t**)realloc( map_array,
                map_alloc * sizeof(map_entry_t*) );
    }

    map_array[map_used++] = item;
    //printf( "0x%x: %s\n", item->address, CallStack_Unmangle(item->name) );
}

void
MapFile::map_array_destroy( void )
{
    int i;
    for ( i = 0; i < (int)map_used; i++ ) {
        free( map_array[i]->name );
        free( map_array[i] );
    }

    free( map_array );
    map_array = 0;
}

// ---------------------------------------------------------------------------
// file array

void
MapFile::file_array_init( void )
{
    file_array = (char**)malloc( sizeof(char*) );
    file_alloc = 1;
    file_used = 0;
}

void
MapFile::file_array_add( char* item )
{
    if ( file_used == file_alloc ) {
        file_alloc <<= 1;
        file_array = (char**)realloc( file_array,
                file_alloc * sizeof(char*) );
    }

    file_array[file_used++] = item;
}

void
MapFile::file_array_destroy( void )
{
    int i;
    for ( i = 0; i < (int)file_used; i++ ) {
        free( file_array[i] );
    }

    free( file_array );
    file_array = 0;
}

// ---------------------------------------------------------------------------
// line array
void
MapFile::line_array_init( void )
{
    line_array = (line_entry_t*)malloc( sizeof(line_entry_t) );
    line_alloc = 1;
    line_used = 0;
}

void
MapFile::line_array_add( char* file, unsigned line, unsigned address )
{
    if ( line_used == line_alloc ) {
        line_alloc <<= 1;
        line_array = (line_entry_t*)realloc( line_array,
                line_alloc * sizeof(line_entry_t) );
    }

    line_array[line_used].address = address;
    line_array[line_used].file = file;
    line_array[line_used].line = line;

    line_used++;
}

void
MapFile::line_array_destroy( void )
{
    free( line_array );
    line_array = 0;
}


// ---------------------------------------------------------------------------
// array sort

int
MapFile::compare_map_entry( map_entry_t** first, map_entry_t** second )
{
    if ( (*first)->address < (*second)->address ) {
        return -1;
    } else if ( (*first)->address > (*second)->address ) {
        return 1;
    } else {
        return 0;
    }
}

int
MapFile::compare_line_entry( line_entry_t* first, line_entry_t* second )
{
    if ( first->address < second->address ) {
        return -1;
    } else if ( first->address > second->address ) {
        return 1;
    } else {
        return 0;
    }
}

void
MapFile::sort_arrays(void)
{
    typedef int (*fncompare)(const void*, const void*);

    qsort( map_array, map_used, sizeof(*map_array), 
            (fncompare)compare_map_entry );

    qsort( line_array, line_used, sizeof(*line_array), 
            (fncompare)compare_line_entry );
}

// ---------------------------------------------------------------------------
// map parsing code

bool
MapFile::handle_symbol_entry( char* line, int n, char* token[] )
{
    unsigned address;

    // must be at least 3 tokens.
    if ( n < 3 ) {
        return false;
    }

    // first token must be of the form 0000:00000000
    if ( token[0][4] != ':' ) {
        return false;
    }

    // 3rd token must be a number.
    if ( 1 != sscanf( token[2], "%x", &address ) ) {
        return false;
    }

    // okay, create a map entry name is token 1, addr is token 2.
    map_entry_t* entry = (map_entry_t*)malloc(sizeof(map_entry_t));

    char buffer[128];
    TCHAR tcBuffer[128];
    TCHAR tcToken1[128];

    mbstowcs(tcToken1, token[1], 128);

    UnDecorateSymbolNamePtr( tcToken1, tcBuffer,
            sizeof(tcBuffer)/sizeof(*tcBuffer), 0 );

    wcstombs(buffer, tcBuffer, 128);

    buffer[sizeof(buffer)-1] = 0;
    entry->name = _strdup( buffer );
    entry->address = address;
    map_array_add( entry );

    return true;
}


bool
MapFile::handle_preferred( char* line, int n, char* token[] )
{
    return 1 == sscanf( line, " Preferred load address is %x", 
                &_preferred_load_address );
}


bool
MapFile::handle_line_header( char* line, int n, char* token[] )
{
    if ( memcmp( line, "Line numbers for ", 17 ) != 0 ) {
        return false;
    }
    
    char* ob = strchr( line, '(' );
    char* cb = strchr( line, ')' );
    if ( cb ) {
        *cb = 0;
    }

    if ( ob ) {
        ob++;
    } else {
        ob = &line[17];
    }

    file_array_add( _strdup( ob ) );

    return true;
}

bool
MapFile::handle_line_numbers( char* line, int n, char* token[] )
{
    int i;

    // number of tokens must be 8.
    if ( n != 8 ) {
        return false;
    }

    if ( file_used == 0 ) {
        return false;
    }

    for ( i = 0; i < 4; i++ ) {
        int lineno;
        int address;
        if ( 1 != sscanf( token[i*2], "%d", &lineno ) ) {
            return false;
        }

        if ( 1 != sscanf( token[i*2+1], "0001:%x", &address ) ) {
            return false;
        }

        address += _preferred_load_address + 0x1000;

        line_array_add( file_array[file_used-1], lineno, address );
    }

    return true;
}

bool
MapFile::handle_timestamp( char* line, int n, char* token[] )
{
    if ( n < 3 ) {
        return false;
    }

    if ( 0 != strcmp( token[0], "Timestamp" ) ) {
        return false;
    }


    return 1 == sscanf( token[2], "%lx", &_timeStamp );
}

void
MapFile::handle_line( char* line )
{
    char buffer[1024];
    char* tokens[10];
    int num = 0;
    unsigned addr = 0;

    strcpy( buffer, line );
    buffer[strlen(buffer)-1] = 0;

    tokens[num] = strtok( line, " \n\r" );
    while( tokens[num] && num + 1 < sizeof(tokens)/sizeof(*tokens) ) {
        num++;
        tokens[num] = strtok( 0, " \n\r" );
    }

    handle_symbol_entry( buffer, num, tokens ) ||
    handle_preferred( buffer, num, tokens ) ||
    handle_line_header( buffer, num, tokens ) || 
    handle_line_numbers( buffer, num, tokens ) ||
    handle_timestamp( buffer, num, tokens );
}

// ---------------------------------------------------------------------------
// In-place removal of leading whitespace
// ---------------------------------------------------------------------------
void 
trim_whitespace(char* buffer)
{
    char* ch = buffer;
    char* pos = buffer;
    int found = 0;

    // for each character in the string,
    while( *ch ) {

        // if we have not found a whitespace character, and the current
        // character is not a whitespace character, 
        if ( found == 0 && *ch != ' ' ) {
            // if this is the first character there is nothing to be done.
            if ( ch == buffer ) {
                return;
            }

            // set a flag indicating that we should begin shifting.
            found = 1;
        }

        // if we are now shifting, 
        if ( found == 1 ) {
            // copy the character to the correct position and increment pos.
            *pos++ = *ch;
        }

        ch++;
    }

    // end the string at pos.
    *pos = 0;
}

int
MapFile::read( const char* name, int maxLines )
{
    char buffer[1024];
    int lines_read = 0;

    // load the map file.
    FILE* file = fopen( name, "rt" );
    if ( file == 0 ) {
        return 0;
    }

    map_array_init();
    file_array_init();
    line_array_init();

    // read the image name from the file.
    char* dot = strrchr( _imageName, '.' );
    if ( dot ) {
        *dot = 0;
    }
    if ( 0 == fgets( buffer, sizeof(buffer), file ) ) {
        fclose( file );
        return 0;
    }
    lines_read++;

    trim_whitespace(buffer);
    free( _imageName );
    _imageName = _strdup( buffer );

    int len = strlen(_imageName);
    while( len && _imageName[len-1] == '\r' || _imageName[len-1] == '\n' ||
            _imageName[len-1] == ' ' ) 
    {
        len--;
        _imageName[len] = 0;
    }

    // now process the rest of the file.

    for( ;; ) {
        if ( lines_read == maxLines ) {
            break;
        }

        if ( 0 == fgets( buffer, sizeof(buffer), file ) ) {
            break;
        }

        handle_line( buffer );
        lines_read++;
    }

    fclose( file );

    sort_arrays();

    return map_used;
}

MapFile::map_entry_t*
MapFile::map_lookup( unsigned target )
{
    int high = map_used, low = -1, probe;

	if ( map_used == 0 ) {
		return 0;
	}

    while (high - low > 1)
    {
        probe = (high + low) / 2;
        if (map_array[probe]->address < target)
            low = probe;
        else
            high = probe;
    }
    if ( low == -1 ) return 0;
    if ( (int)high >= (int)map_used ) return 0;

    return map_array[low];
}

MapFile::line_entry_t*
MapFile::line_lookup( unsigned target )
{
    int high = line_used, low = -1, probe;

	if ( line_used == 0 ) {
		return 0;
	}

    while (high - low > 1)
    {
        probe = (high + low) / 2;
        if (line_array[probe].address < target)
            low = probe;
        else
            high = probe;
    }
    if ( low == -1 ) return 0;
    if ( (int)low >= (int)line_used ) return 0;

    return &line_array[low];
}

void
MapFile::print_stack( unsigned* stack, int max )
{
    int i;

    for( i = 0; i < max; i++ ) {
        map_entry_t* entry = map_lookup( stack[i] );
        printf("0x%08x -> 0x%08x: %s\n", stack[i], entry->address, entry->name );
    }
}

const char* 
MapFile::lookup( unsigned address )
{
    map_entry_t* entry = map_lookup( address );
    if ( entry ) {
        return CallStack_Unmangle( entry->name );
    }

    return 0;
}

bool
MapFile::getPosition( unsigned address, CrashPosition_t* pos )
{
    line_entry_t* line_entry = line_lookup( address );
    map_entry_t* map_entry = map_lookup( address );

    memset( pos, 0, sizeof(*pos));

    if ( line_entry ) {
        pos->file = line_entry->file;
        pos->line = line_entry->line;
    }

    if ( map_entry ) {
//        UnDecorateSymbolNamePtr( map_entry->name, pos->function,
//                sizeof(pos->function)/sizeof(*pos->function), 0 );
//        pos->function[sizeof(pos->function)-1] = 0;
        pos->function = map_entry->name;
    }

    return true;
}

