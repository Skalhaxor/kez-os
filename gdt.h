/* Author: Alexander Anderson
 * Description: Used for setting up and modifying the GDT
 */

#ifndef INCLUDE_GDT_H
#define INCLUDE_GDT_H

/* Only 3 descriptors needed (NULL, code segment, and data segment descriptors) */
#define NUM_DESCRIPTORS 3

/* For specifying the memory details of the GDT */
struct gdt_t
{
    unsigned short size;         /**< Size in bytes of the GDT minus 1 */
    unsigned int address;        /**< Starting address in memory of the GDT */
} __attribute__((packed));

/* Descriptor for the GDT */
struct gdt_descriptor_t
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access_byte;
    unsigned char limit_high_and_flags;
    unsigned char base_high;
} __attribute__((packed));

/**
 * Sets up the memory information of the GDT
 *
 * @param [in] gdt_info - Pointer to the gdt information
 */
void load_gdt(struct gdt_t gdt_ptr);

/**
 * Loads each segment with the proper offset to the new GDT
 */
void load_segments();

#endif /* INCLUDE_GDT_H */
