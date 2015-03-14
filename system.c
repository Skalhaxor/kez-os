/* Author: Alexander Anderson
 * Description: Important functions and data structures for setting up the system
 */

#include "system.h"
#include "gdt.h"
#include "serial_driver.h"

/* The GDT to be loaded. The NULL descriptor will actually contain the pointer to this GDT, because it
 *   is known to be safe from access (pointer to itself) */
static struct gdt_descriptor_t gdt[NUM_DESCRIPTORS];

int setup_descriptor(int descriptor_num, unsigned int base_address, unsigned int limit_address,
                     unsigned char access_byte, unsigned char flags)
{
    if ((descriptor_num < 1) || (descriptor_num >= NUM_DESCRIPTORS))
    {
        return -1;
    }

    /* Setup the base address */
    gdt[descriptor_num].base_low = base_address & 0xFFFF;
    gdt[descriptor_num].base_middle = (base_address >> 16) & 0xFF;
    gdt[descriptor_num].base_high = (base_address >> 24) & 0xFF;

    /* Setup the limit address */
    gdt[descriptor_num].limit_low = limit_address & 0xFFFF;
    gdt[descriptor_num].limit_high_and_flags = (limit_address >> 16) & 0xF;

    /* Setup the access byte */
    gdt[descriptor_num].access_byte = access_byte;

    /* Setup the flags */
    gdt[descriptor_num].limit_high_and_flags |= (flags << 4) & 0xF0;

    return 0;
}

int install_gdt()
{
    /* Zero-out the NULL descriptor */
    gdt[0].limit_low = 0;
    gdt[0].base_low = 0;
    gdt[0].base_middle = 0;
    gdt[0].access_byte = 0;
    gdt[0].limit_high_and_flags = 0;
    gdt[0].base_high = 0;

    /* Setup the pointer to the GDT, which will be stored in the NULL descriptor (trust me, it's safe) */
    struct gdt_t* gdt_ptr = (struct gdt_t*)gdt;
    gdt_ptr->address = (unsigned int)gdt;
    gdt_ptr->size = (sizeof(struct gdt_descriptor_t) * NUM_DESCRIPTORS) - 1;

    /* Descriptor for code segment:
     *
     * Index in table                  = 1
     * Base address valid for segment  = 0x00000000
     * Limit address valid for segment = 0xFFFFFFFF (Because of granularity bit)
     *
     * Access byte:
     *   Readable bit   = 1 (Readable)
     *   Conforming bit = 0 (Code can only be executed within ring specified in privilege bits)
     *   Executable bit = 1 (Allows execution)
     *   Privilege bits = 0 (All access, kernel level)
     *   Present bit    = 1 (Descriptor is valid)
     *
     * Flags:
     *   Protection mode      = 1 (32 bit protected)
     *   Granularity of limit = 1 (4 KB blocks)
     */
    if (setup_descriptor(1, 0, 0xFFFFF, 0x9A, 0x0C) < 0)
    {
        char log[] = "ERROR: Failed to setup the GDT descriptor for the code segment\n";
        serial_write(log, sizeof(log));
        return -1;
    }

    /* Descriptor for data segments:
     *
     * Index in table                  = 2
     * Base address valid for segment  = 0x00000000
     * Limit address valid for segment = 0xFFFFFFFF (Because of granularity bit)
     *
     * Access byte:
     *   Writable bit   = 1 (Writable)
     *   Direction bit  = 0 (Segment grows up)
     *   Executable bit = 0 (Execution not allowed)
     *   Privilege bits = 0 (All access, kernel level)
     *   Present bit    = 1 (Descriptor is valid)
     *
     * Flags:
     *   Protection mode      = 1 (32 bit protected)
     *   Granularity of limit = 1 (4 KB blocks)
     */
    if (setup_descriptor(2, 0, 0xFFFFF, 0x92, 0x0C) < 0)
    {
        char log[] = "ERROR: Failed to setup the GDT descriptor for the data segments\n";
        serial_write(log, sizeof(log));
        return -1;
    }

    /* Load the table into the system */
    load_gdt(*gdt_ptr);

    /* Set up the segments to point to the proper descriptors */
    load_segments();

    return 0;
}
