#include "heap.h"
#include "pmm.h"
#include "stdio.h"
#include "vmm.h"

static header_t* heap_head;

static uint32_t heap_max = HEAP_START;

static void allocChunk(uint32_t start, uint32_t len);
static void freeChunk(header_t* chunk);
// split the chunk into two chunks(chunk(origLen) --> chunk1(len) + chunk2(origLen-len))
static void splitChunk(header_t* chunk, uint32_t len);
// merge chunk with chunks before and after
static void glueChunk(header_t* chunk);

void initHeap() {
	heap_head = 0;
}

void* malloc(uint32_t len) {
	// add the size of header_t to length
	len += sizeof(header_t);

	header_t* cur = heap_head;
	header_t* prev = 0;

	while(cur) {
		if(cur->allocated == 0 && cur->len >= len) {
			// exist a unallocated chunk with enough size 
			splitChunk(cur, len);
			cur->allocated = 1;
			return (void*)((uint32_t)cur + sizeof(header_t));
		}
		prev = cur;
		cur = cur->next;
	}
	// find no suitable chunk, alloc a new one
	uint32_t chunk_start;
	if(prev) chunk_start = (uint32_t)prev + prev->len;
	else {
		chunk_start = HEAP_START;
		heap_head = (header_t*)chunk_start;
	}
	// allocate memory at chunk_start
	allocChunk(chunk_start, len);
	cur = (header_t*) chunk_start;
	cur->prev = prev;
	cur->next = 0;
	cur->allocated = 1;
	cur->len = len;
	if(prev) prev->next = cur;
	return (void*) (chunk_start + sizeof(header_t));
}

void free(void* ptr) {
	header_t *h = (header_t*)((uint32_t)ptr - sizeof(header_t));
	h->allocated = 0;
	glueChunk(h);
}

void testHeap() {
	printf("Test malloc and free\n");

	void* addr1 = malloc(50);
	printf("malloc 50 bytes in 0x%X\n", addr1);
	void* addr2 = malloc(500);
	printf("malloc 500 bytes in 0x%X\n", addr2);
	void* addr3 = malloc(5000);
	printf("malloc 5000 bytes in 0x%X\n", addr3);

	printf("free in 0x%X\n", addr1);
	free(addr1);
	printf("free in 0x%X\n", addr2);
	free(addr2);
	printf("free in 0x%X\n", addr3);
	free(addr3);
}

// static function
void allocChunk(uint32_t start, uint32_t len) {
	// alloc memory page to expand heap 
	while(start + len > heap_max) {
		uint32_t page = pmmAllocPage();
		map(pgd_kern, heap_max, page, PAGE_PRESENT | PAGE_WRITE);
		heap_max += PAGE_SIZE;
	}
}

void freeChunk(header_t* chunk) {
	if(!chunk->prev) heap_head = 0;
	else chunk->prev->next = 0;

	while((heap_max - PAGE_SIZE) >= (uint32_t)chunk) {
		heap_max -= PAGE_SIZE;
		uint32_t page;
		getMapping(pgd_kern, heap_max, &page);
		unmap(pgd_kern, heap_max);
		pmmFreePage(page);
	}
}

void splitChunk(header_t* chunk, uint32_t len) {
	// the left memory should be larger than the size of header_t
	if(chunk->len - len > sizeof(header_t)) {
		// the addr of new chunk
		header_t* newchunk = (header_t*)((uint32_t)chunk + chunk->len);
		newchunk->prev = chunk;
		newchunk->next = chunk->next;
		newchunk->allocated = 0;
		newchunk->len = chunk->len - len;

		chunk->next = newchunk;
		chunk->len = len;
	}
}

void glueChunk(header_t* chunk) {
	// merge the unused chunk after into chunk
	if(chunk->next && chunk->next->allocated == 0) {
		chunk->len = chunk->len + chunk->next->len;
		if(chunk->next->next) chunk->next->next->prev = chunk;
		chunk->next = chunk->next->next;
	}
	// merge chunk into the unused chunk before
	if(chunk->prev && chunk->prev->allocated == 0) {
		chunk->prev->len = chunk->prev->len + chunk->len;
		if(chunk->next) chunk->next->prev = chunk->prev;
		chunk = chunk->prev;
	}
	if(chunk->next == 0) freeChunk(chunk);
}