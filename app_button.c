#include "app_button.h"

#include <wiced_bt_event.h>
#include <wiced_platform.h>
#include <hal/wiced_hal_gpio.h>

#include "hci_control_hfp_hf.h"

static const uint8_t call_action;

static int app_button_execute(void *data)
{
    (void)data;

    WICED_BT_TRACE("SW3 execute call action\n");
    hci_control_hf_button_call_action();
    return 0;
}

static void app_button_gpio_callback(void *user_data, uint8_t pin)
{
    uint32_t pin_state;

    (void)user_data;

    pin_state = wiced_hal_gpio_get_pin_input_status(WICED_BUTTON3);
    WICED_BT_TRACE("SW3 edge pin=%u state=%u\n", pin, pin_state);

    /* Execute only on button release. SW3 is active-low. */
    if (pin_state == GPIO_PIN_OUTPUT_HIGH)
    {
        if (!wiced_app_event_serialize(
                app_button_execute,
                (void *)&call_action))
        {
            WICED_BT_TRACE("Failed to queue SW3 action\n");
        }
    }
}

void app_button_init(void)
{
    wiced_hal_gpio_register_pin_for_interrupt(
    		WICED_BUTTON3,
        app_button_gpio_callback,
        NULL);

    wiced_hal_gpio_configure_pin(
        WICED_BUTTON3,
        WICED_GPIO_BUTTON_SETTINGS(GPIO_EN_INT_BOTH_EDGE),
        GPIO_PIN_OUTPUT_HIGH);

        WICED_BT_TRACE("SW3 initialized gpio=%u state=%u\n",
               WICED_BUTTON3,
               wiced_hal_gpio_get_pin_input_status(WICED_BUTTON3));
}
