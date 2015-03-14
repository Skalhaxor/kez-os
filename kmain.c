#include "framebuffer_driver.h"
#include "serial_driver.h"
#include "system.h"

void kmain()
{
    /* Setup serial first, so logging can occur */
    serial_init();
    char starting[] = "Kernel is starting up\n";
    serial_write(starting, sizeof(starting));

    /* Install a new GDT to allow segmentation */
    if (install_gdt() < 0)
    {
        char error[] = "ERROR: Failed to setup the new GDT\n";
        serial_write(error, sizeof(error));
    }

    /* for some reason I wasn't able to pass the string literal into write.
     * I had to pass the array...Why is that?
     **/
    char hello[] = "kez-os Bitches!!!!";
    clear_screen();
    fb_write(hello, sizeof(hello));

    char log[] = "WARN: Serial logging like a badass!\n";
    serial_write(log, sizeof(log));
}
