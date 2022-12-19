#include <stdio.h>
#include <usb.h>

// https://unix.stackexchange.com/questions/61484/
// needs libusb-dev :
// sudo apt install libusb-dev
// dependency('libusb')

int main()
{
    setbuf(stdout, 0);

    usb_init();
    usb_find_busses();
    usb_find_devices();

    struct usb_bus *bus;
    struct usb_device *dev;

    for (bus = usb_busses; bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next)
        {
            printf("Trying device %s/%s\n", bus->dirname, dev->filename);
            printf("\tID_VENDOR = 0x%04x\n", dev->descriptor.idVendor);
            printf("\tID_PRODUCT = 0x%04x\n", dev->descriptor.idProduct);
        }
    }

    return 0;
}


