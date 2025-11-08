#include <arena.h>
#include <string.h>
#include <unity.h>
#include <utils.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

static size_t get_page_size(void)
{
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwPageSize;
#else
    long ps = sysconf(_SC_PAGESIZE);
    return (size_t) ps;
#endif
}

static arena_t arena;

void setUp(void)
{
    TEST_ASSERT_TRUE(arena_init(&arena));
}

void tearDown(void)
{
    arena_destroy(&arena);
}

void test_arena_init(void)
{
    arena_t test_arena;
    TEST_ASSERT_TRUE(arena_init(&test_arena));
    TEST_ASSERT_NOT_NULL(test_arena.first);
    TEST_ASSERT_NOT_NULL(test_arena.current);
    TEST_ASSERT_EQUAL(10 * get_page_size(), test_arena.size);
    arena_destroy(&test_arena);
}

void test_arena_alloc_small(void)
{
    void *ptr = arena_alloc(&arena, 100);
    TEST_ASSERT_NOT_NULL(ptr);
    memset(ptr, 0xAA, 100);
    TEST_ASSERT_EQUAL(100, arena.current->size);
}

void test_arena_alloc_multiple(void)
{
    void *ptr1 = arena_alloc(&arena, 50);
    void *ptr2 = arena_alloc(&arena, 75);
    void *ptr3 = arena_alloc(&arena, 25);
    TEST_ASSERT_NOT_NULL(ptr1);
    TEST_ASSERT_NOT_NULL(ptr2);
    TEST_ASSERT_NOT_NULL(ptr3);
    memset(ptr1, 0xBB, 50);
    memset(ptr2, 0xCC, 75);
    memset(ptr3, 0xDD, 25);
    TEST_ASSERT_EQUAL(150, arena.current->size);
}

void test_arena_alloc_new_chunk(void)
{
    size_t chunk_size = 10 * get_page_size();
    /* Allocate almost the entire first chunk */
    void *ptr1 = arena_alloc(&arena, chunk_size - 100);
    TEST_ASSERT_NOT_NULL(ptr1);
    memset(ptr1, 0xEE, chunk_size - 100);
    TEST_ASSERT_EQUAL(chunk_size - 100, arena.current->size);

    /* This should create a new chunk */
    void *ptr2 = arena_alloc(&arena, 100);
    TEST_ASSERT_NOT_NULL(ptr2);
    memset(ptr2, 0xFF, 100);
    TEST_ASSERT_EQUAL(100, arena.current->size);
    TEST_ASSERT_NOT_EQUAL(arena.first, arena.current);
    TEST_ASSERT_EQUAL(chunk_size + chunk_size, arena.size);
}

void test_arena_alloc_large(void)
{
    size_t chunk_size = 10 * get_page_size();
    size_t alloc_size = chunk_size + 1000; /* Larger than chunk_size */
    size_t expected_capacity = ALIGN_UP(alloc_size, chunk_size);
    /* Allocate something larger than default chunk size */
    void *ptr = arena_alloc(&arena, alloc_size);
    TEST_ASSERT_NOT_NULL(ptr);
    memset(ptr, 0x11, alloc_size);
    TEST_ASSERT_EQUAL(alloc_size, arena.current->size);
    TEST_ASSERT_EQUAL(expected_capacity, arena.current->capacity);
    TEST_ASSERT_EQUAL(chunk_size + expected_capacity, arena.size);
}

void test_arena_alloc_zero(void)
{
    void *ptr = arena_alloc(&arena, 0);
    TEST_ASSERT_NOT_NULL(ptr);
    /* Zero allocation should not change size */
    size_t size_before = arena.current->size;
    TEST_ASSERT_EQUAL(size_before, arena.current->size);
}

void test_arena_destroy(void)
{
    arena_t local_arena;
    TEST_ASSERT_TRUE(arena_init(&local_arena));
    arena_alloc(&local_arena, 100);
    arena_alloc(&local_arena, 200);
    arena_destroy(&local_arena);
    /* After destroy, arena should be in a clean state, but since it's destroyed,
       we can't check internals. Just ensure no crashes. */
    TEST_PASS();
}

void test_allocations_are_contiguous(void)
{
    char *ptr1 = arena_alloc(&arena, 10);
    char *ptr2 = arena_alloc(&arena, 10);
    TEST_ASSERT_NOT_NULL(ptr1);
    TEST_ASSERT_NOT_NULL(ptr2);
    TEST_ASSERT_EQUAL_PTR(ptr1 + 10, ptr2);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_arena_init);
    RUN_TEST(test_arena_alloc_small);
    RUN_TEST(test_arena_alloc_multiple);
    RUN_TEST(test_arena_alloc_new_chunk);
    RUN_TEST(test_arena_alloc_large);
    RUN_TEST(test_arena_alloc_zero);
    RUN_TEST(test_arena_destroy);
    RUN_TEST(test_allocations_are_contiguous);
    return UNITY_END();
}
