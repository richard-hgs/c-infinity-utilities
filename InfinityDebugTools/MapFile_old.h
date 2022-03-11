#ifndef MAPREADER_H
#define MAPREADER_H

typedef struct _CrashPosition_t {
    const char* file;
    const char* function;
    int line;
} CrashPosition_t;

class MapFile
{
public:
    MapFile();
    ~MapFile();

    int read( const char* name, int maxLines = 0 );
    bool getPosition( unsigned address, CrashPosition_t* pos );
    long timeStamp();
    const char* imageName();

private:
    typedef struct _map_entry_t {
        char* name;
        unsigned address;
    } map_entry_t;

    map_entry_t** map_array;
    unsigned map_alloc;
    unsigned map_used;

    void map_array_init();
    void map_array_add( map_entry_t* item );
    void map_array_destroy();

    char** file_array;
    unsigned file_alloc;
    unsigned file_used;

    void file_array_init();
    void file_array_add( char* item );
    void file_array_destroy();

    typedef struct _line_entry_t {
        unsigned address;
        char* file;
        unsigned line;
    } line_entry_t;

    line_entry_t* line_array;
    unsigned line_alloc;
    unsigned line_used;

    void line_array_init();
    void line_array_add( char* file, unsigned line, unsigned address );
    void line_array_destroy();

    void sort_arrays();

    unsigned _preferred_load_address;

    bool handle_symbol_entry( char* line, int n, char* token[] );
    bool handle_preferred( char* line, int n, char* token[] );
    bool handle_line_header( char* line, int n, char* token[] );
    bool handle_line_numbers( char* line, int n, char* token[] );
    bool handle_timestamp( char* line, int n, char* token[] );
    void handle_line( char* line );

    static int compare_map_entry( map_entry_t** first, map_entry_t** second );
    static int compare_line_entry( line_entry_t* first, line_entry_t* second );

    map_entry_t* map_lookup( unsigned target );
    line_entry_t* line_lookup( unsigned target );
    void print_stack( unsigned* stack, int max );
    const char* lookup( unsigned address );
    char* _imageName;
    long _timeStamp;
};

#endif // MAPREADER_H
