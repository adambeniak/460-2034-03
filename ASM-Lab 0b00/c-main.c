#include <stdio.h>
#include <stdint.h>


extern void task1_extend_temp(void);

int8_t  g_temp8[4]  = { -12, 7, -1, -128 };
int32_t g_temp32[4] = { 111, 222, 333, 444 };

static void print_task1(void) {
    printf("Temp (int8):  %d %d %d %d\n",
           g_temp8[0], g_temp8[1], g_temp8[2], g_temp8[3]);
    printf("Temp (int32): %d %d %d %d\n",
           g_temp32[0], g_temp32[1], g_temp32[2], g_temp32[3]);
}

void test_task1(void) {
    puts("\n1) Teplota: rozšíření int8 -> int32 se znaménkem (MOVSX)");
    puts("Před:");
    print_task1();

    task1_extend_temp();

    puts("Po:");
    print_task1();
}


extern void task2_extend_batt(void);

uint8_t  g_batt8[4]   = { 0, 1, 200, 255 };
uint32_t g_batt32[4]  = { 9, 9, 9, 9 };

static void print_task2(void) {
    printf("Batt (u8):   %u %u %u %u\n",
           g_batt8[0], g_batt8[1], g_batt8[2], g_batt8[3]);
    printf("Batt (u32):  %u %u %u %u\n",
           g_batt32[0], g_batt32[1], g_batt32[2], g_batt32[3]);
}

void test_task2(void) {
    puts("\n2) Baterie: rozšíření uint8 -> uint32 bez znaménka (MOVZX)");
    puts("Před:");
    print_task2();

    task2_extend_batt();

    puts("Po:");
    print_task2();
}


extern void task3_reverse_timestamp(void);

uint32_t g_timestamp   = 0x0A141E28;
uint32_t g_timestamp_r = 0xDEADBEEF;

static void print_task3(void) {
    printf("Timestamp původní:  0x%08X\n", g_timestamp);
    printf("Timestamp reversed: 0x%08X\n", g_timestamp_r);

    uint8_t *a = (uint8_t*)&g_timestamp;
    uint8_t  *b = (uint8_t*)&g_timestamp_r;
    printf("Bajty src: %02X %02X %02X %02X\n", a[0], a[1], a[2], a[3]);
    printf("Bajty dst: %02X %02X %02X %02X\n", b[0], b[1], b[2], b[3]);
}

void test_task3(void) {
    puts("\n3) Timestamp: prohození bajtů (byte-reverse) jen pomocí MOV");
    puts("Před:");
    print_task3();

    task3_reverse_timestamp();

    puts("Po:");
    print_task3();
}

extern void task4_split_device_id(void);

uint64_t g_device_id = 0x11223344AABBCCDDULL;
uint32_t g_vendor_id = 0;
uint32_t g_serial_id = 0;

static void print_task4(void) {
    printf("Device ID:  0x%016llX\n", (unsigned long long)g_device_id);
    printf("Vendor ID:  0x%08X\n", g_vendor_id);
    printf("Serial ID:  0x%08X\n", g_serial_id);
}

void test_task4(void) {
    puts("\n4) Device ID: rozdělení uint64 -> 2x uint32 (jen MOV)");
    puts("Před:");
    print_task4();

    task4_split_device_id();

    puts("Po:");
    print_task4();
}


int main(void) {
    puts("APPS ASM – test (MOV/MOVZX/MOVSX)");

    test_task1();
    test_task2();
    test_task3();
    test_task4();

    return 0;
}

