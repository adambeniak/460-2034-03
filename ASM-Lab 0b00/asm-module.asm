
    bits 64

    section .data

    global task1_extend_temp
    global task2_extend_batt
    global task3_reverse_timestamp
    global task4_split_device_id

    extern g_temp8
    extern g_temp32
    extern g_batt8
    extern g_batt32
    extern g_timestamp
    extern g_timestamp_r
    extern g_device_id
    extern g_serial_id
    extern g_vendor_id

    section .text

task1_extend_temp:
        movsx eax, byte[g_temp8]
        mov [g_temp32], eax

        movsx eax, byte[g_temp8 + 1]
        mov [g_temp32 + 4], eax

        movsx eax, byte[g_temp8 + 2]
        mov [g_temp32 + 8], eax

        movsx eax, byte[g_temp8 + 3]
        mov [g_temp32 + 12], eax
        ret

task2_extend_batt:
        movzx eax, byte[g_batt8]
        mov [g_batt32], eax

        movzx eax, byte[g_batt8 + 1]
        mov [g_batt32 + 4], eax

        movzx eax, byte[g_batt8 + 2]
        mov [g_batt32 + 8], eax

        movzx eax, byte[g_batt8 + 3]
        mov [g_batt32 + 12], eax
        ret

task3_reverse_timestamp:
        mov al, [g_timestamp]
        mov [g_timestamp_r + 3], al

        mov al, [g_timestamp + 1]
        mov [g_timestamp_r + 2], al

        mov al, [g_timestamp + 2]
        mov [g_timestamp_r + 1], al

        mov al, [g_timestamp + 3]
        mov [g_timestamp_r], al
        ret

task4_split_device_id:
        mov eax, [g_device_id]
        mov [g_serial_id], eax

        mov eax, [g_device_id + 4]
        mov [g_vendor_id], eax
        ret