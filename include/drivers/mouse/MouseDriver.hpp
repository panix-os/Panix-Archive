
#ifndef MOUSE_DRIVER_HPP
#define MOUSE_DRIVER_HPP

#include <types.hpp>
#include <cpu/Port/Port.hpp>
#include <cpu/interrupts/InterruptHandler.hpp>
#include <drivers/mouse/MouseEventHandler.hpp>
#include <drivers/Driver.hpp>

class MouseEventHandler;

class MouseDriver : public InterruptHandler, public Driver {
    private:
        Port8Bit dataPort;
        Port8Bit commandPort;
        uint8_t buffer[3];
        uint8_t offset;
        uint8_t buttons;

        MouseEventHandler* mouseEventHandler;
    public:
        /**
         * @brief Construct a new Mouse Driver object
         * 
         * @param interruptManager 
         * @param mouseEventHandler 
         */
        MouseDriver(InterruptManager* interruptManager, MouseEventHandler* mouseEventHandler);
        /**
         * @brief Destroy the Mouse Driver object
         * 
         */
        ~MouseDriver();
        /**
         * @brief 
         * 
         * @param esp 
         * @return uint32_t 
         */
        virtual uint32_t handleInterrupt(uint32_t esp);
        /**
         * @brief 
         * 
         */
        virtual void activate();
};

#endif /* MOUSE_DRIVER_HPP */