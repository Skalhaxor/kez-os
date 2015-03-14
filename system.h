/* Author: Alexander Anderson
 * Description: Important functions and data structures for setting up the system
 */

#ifndef INCLUDE_SYSTEM_H
#define INCLUDE_SYSTEM_H

/**
 * Sets up a descriptor within the GDT
 *
 * @param [in] descriptor_num - The number of the descriptor to change. Starting at 0 with the NULL
 *                                descriptor. Changing the NULL descriptor is not allowed
 * @param [in] base_address   - The base address of the address range a segment is allowed to access
 * @param [in] limit_address  - The limit address of the address range a segment is allowed to access
 * @param [in] access_byte    - Byte defining how the segment is accessed
 * @param [in] flags          - Additional settings for the segment
 *
 * @return - 0 if successful, and -1 for failure
 */
int setup_descriptor(int descriptor_num, unsigned int base_address, unsigned int limit_address,
                     unsigned char access_byte, unsigned char flags);

/**
 * Sets up the GDT in order to use segmentation
 *
 * @return - 0 if successful, and -1 for failure
 */
int install_gdt();

#endif /* INCLUDE_SYSTEM_H */
