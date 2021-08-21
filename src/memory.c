#include "memory.h"


static uint32_t* kstart = (void*) KHEAP_START;
static uint32_t* ustart = (void*) UHEAP_START;

pageDirectoryCR3_t pageDirectoryCR3;
alignas(4096) pageDirectory_t pageDirectory[1024];
alignas(4096) pageTable_t pageTable[1024][1024];

// bare bones malloc before virtual memory is set up virtual memory
// All memory allocations are 4 byte aligned for performance
void* bcmalloc(uint32_t size) {
    uint32_t* index = kstart;
    while(index < ustart) {                                             // Within kernel heap boundaries
        if ((*index) == 0) {                                            // If block is free
            uint32_t* blockPointer = index + sizeof(uint32_t);
            uint32_t blockSize = sizeof(uint32_t);
            while (blockPointer < ustart) {                             // Not to read out of kernel heap boundaries
                if ((*blockPointer) == 0) {                             // If block 4byte is empty
                    if (blockSize >= size) {                            // If block is big enough
                        (*index) == blockSize;
                        return index++;
                    }
                    else {                                              // Increment to next 4byte
                        blockPointer++;
                        blockSize += sizeof(uint32_t);
                    }
                }
                else {
                    index = blockPointer;                               // Pointer to next block;
                    break;
                }
            }
        }
        else {                                                          // Move to next block
            index += (*index) + sizeof(uint32_t);                                                                    
        }
    }

    // If out of bounds;
    println("Out of kernel memory, aborting bbmalloc");
    return nullptr;
}

void bbfree(void* address) {
    uint32_t* size = (uint32_t*)(address - sizeof(uint32_t));
    uint32_t* data = (void*)address;
    // clear size data
    address -= sizeof(uint32_t);
    address = 0;
    // clear contents of block
    for (uint32_t blockSize = 0; blockSize < (*size);  blockSize++) {
        
    }
}


// malloc with virtual memory for kernel
// kernel is always physically mapped for performance
void* kmalloc(uint32_t size) {

}


bool memory_init(multiboot_info_t* mbd, uint32_t magic) {
    // temp string of size 64
    char tempString[64];
    // Variables to initialize memory;
    void* usableFirstPageAddress;
    void* usableLastPageAddress;
    void* finalPageAddress;


/*-----------------------------------------------------------------------------------------------*/
/*                                          Memory Type                                          */
/*-----------------------------------------------------------------------------------------------*/
    /* Make sure the magic number matches for memory mapping*/
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        println("Fatal Error: Invalid magic number!");
        panic();
    }
 
    /* Check bit 6 to see if we have a valid memory map */
    if(!(mbd->flags >> 6 & 0x1)) {
        println("Fatal Error: Invalid memory map given by GRUB bootloader");
        panic();
    }
 
    /* Loop through the memory map and display the values */
    for(uint32_t i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) 
    {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
        
        print("Start Addr : ");
        println(itoa(mmmt->addr, tempString, 16));
        print("Length: ");
        println(itoa(mmmt->len, tempString, 16));
        print("Size: ");
        println(itoa(mmmt->size, tempString, 16));
        print("type: ");
        println(itoa(mmmt->type, tempString, 16));
        // printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
        //     mmmt->addr, mmmt->len, mmmt->size, mmmt->type);
 
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            print("Memory block: ");
            println(itoa(i, tempString, 10));
            // get first and last page addresses 
            usableFirstPageAddress = (void*)mmmt -> addr;
            uint32_t tempLong = mmmt -> addr + mmmt -> len - 1;
            tempLong &= 0xFFFFF000;
            usableLastPageAddress = (void*)tempLong;
        }
        println("");
        // Iterate through sections and get final page address
        uint32_t tempLong = mmmt -> addr + mmmt -> len - 1;
        tempLong &= 0xFFFFF000;
        finalPageAddress = (void*)tempLong;
    }
    

/*-----------------------------------------------------------------------------------------------*/
/*                                          Paging Init                                          */
/*-----------------------------------------------------------------------------------------------*/

    // Initialization of paging structures
    // Map all of the paging structures to memory
    // Only enable kernel space memory for now, enable MMIO and user space later in init process
    print("Initializing paging... ");

    // Init pageDirectoryCR3
    pageDirectoryCR3.address = (uint32_t)&pageDirectory >> 12;
    pageDirectoryCR3.pageWriteThrough = 1;
    pageDirectoryCR3.pageCacheDisable = 1;

    // Init pageDirectory
    void* currentPageIndex;
    for (int i = 0; i < 1024; i++) {
        // Separate User space and kernel space tables
        if ((uint32_t)currentPageIndex < (uint32_t)ustart) {        // Kernel space
            currentPageIndex = (void*)(i * 1024 * 4096);
            pageDirectory[i].present = 1;
            pageDirectory[i].RW = 1;
            pageDirectory[i].US = 0;
            pageDirectory[i].pageWriteThrough = 1;
            pageDirectory[i].pageCacheDisable = 1;
            pageDirectory[i].size = 1;
            pageDirectory[i].address = (uint32_t)&pageTable[i] >> 12;

            // Init kernel space page tables
            for (int j = 0; j < 1024; j++) {
                currentPageIndex = (void*)(i * 1024 * 4096 + j * 4096);
                pageTable[i][j].present = 1;
                pageTable[i][j].RW = 1;
                pageTable[i][j].US = 0;
                pageTable[i][j].pageWriteThrough = 1;
                pageTable[i][j].pageCacheDisable = 1;
                pageTable[i][j].global = 1;                         // Will set to global cuz lazy to implement TLBs
                pageTable[i][j].address = ((uint32_t)currentPageIndex) >> 12;
            }
        }

        else if (currentPageIndex <= usableLastPageAddress) {       // User space
            currentPageIndex = (void*)(i * 1024 * 4096);
            pageDirectory[i].present = 0;
            pageDirectory[i].RW = 1;
            pageDirectory[i].US = 1;
            pageDirectory[i].pageWriteThrough = 1;
            pageDirectory[i].pageCacheDisable = 1;
            pageDirectory[i].size = 1;
            pageDirectory[i].address = (uint32_t)&pageTable[i] >> 12;

            // Init user space page tables
            for (int j = 0; j < 1024; j++) {
                currentPageIndex = (void*)(i * 1024 * 4096 + j * 4096);
                pageTable[i][j].present = 0;
                pageTable[i][j].RW = 1;
                pageTable[i][j].US = 1;
                pageTable[i][j].pageWriteThrough = 1;
                pageTable[i][j].pageCacheDisable = 1;
                pageTable[i][j].global = 1;                         // Will set to global cuz lazy to implement TLBs
                pageTable[i][j].address = ((uint32_t)currentPageIndex) >> 12;
            }
        }

        else if (currentPageIndex <= finalPageAddress) {            // MMIO address magic
            currentPageIndex = (void*)(i * 1024 * 4096);
            pageDirectory[i].present = 0;
            pageDirectory[i].RW = 1;
            pageDirectory[i].US = 0;
            pageDirectory[i].pageWriteThrough = 1;
            pageDirectory[i].pageCacheDisable = 1;
            pageDirectory[i].size = 1;
            pageDirectory[i].address = (uint32_t)&pageTable[i] >> 12;

            // Init MMIO space page tables
            for (int j = 0; j < 1024; j++) {
                currentPageIndex = (void*)(i * 1024 * 4096 + j * 4096);
                pageTable[i][j].present = 0;
                pageTable[i][j].RW = 1;
                pageTable[i][j].US = 0;
                pageTable[i][j].pageWriteThrough = 1;
                pageTable[i][j].pageCacheDisable = 1;
                pageTable[i][j].global = 1;                         // Will set to global cuz lazy to implement TLBs
                pageTable[i][j].address = ((uint32_t)currentPageIndex) >> 12;
            }
        }
    }
    return false;
}