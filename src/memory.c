#include <include/types.h>

struct pageTable {
    struct {
        uint32_t present:1;
        uint32_t rw:1;
        uint32_t user:1;
        uint32_t writeThrough:1;
        uint32_t cacheDisable:1;
        uint32_t accessed:1;
        uint32_t dirty:1;
        uint32_t PAT:1;
        uint32_t ignore:3;
        uint32_t address:20;
    } pageTableContents[1024];
};
uint32_t pageTableInitializer = 0x00000006;

struct pageDirectory {
    struct {
        uint32_t present:1;
        uint32_t rw:1;
        uint32_t user:1;
        uint32_t writeThrough:1;
        uint32_t cacheDisable:1;
        uint32_t accessed:1;
        uint32_t ignore1:1;
        uint32_t size:1;
        uint32_t ignore2:4;
        uint32_t address:20;
    } pageDirectoryContents[1024];
};
uint32_t pageDirectoryInitializer = 0x00000006;



void* init_malloc(uint32_t size) {

}