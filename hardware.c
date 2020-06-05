/*
   Nintendo Switch Fightstick - Proof-of-Concept

   Based on the LUFA library's Low-Level Joystick Demo
   (C) Dean Camera
   Based on the HORI's Pokken Tournament Pro Pad design
   (C) HORI

   This project implements a modified version of HORI's Pokken Tournament Pro Pad
   USB descriptors to allow for the creation of custom controllers for the
   Nintendo Switch. This also works to a limited degree on the PS3.

   Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken
   Tournament Pro Pad as a Pro Controller. Physical design limitations prevent
   the Pokken Controller from functioning at the same level as the Pro
   Controller. However, by default most of the descriptors are there, with the
   exception of Home and Capture. Descriptor modification allows us to unlock
   these buttons for our use.
 */

#include "hardware.h"
#include "macro.h"
#include "command.h"
#include "usbservice.h"
//#define MACROCHOICE

//#define LEDD_ON		(PORTC |= (1<<5))
//#define LEDD_OFF		(PORTC &= ~(1<<5))
//#define LEDD_CONFIG	(DDRC |= (1<<5))

#define DIP_CONFIG \
								DDRC = 0; \
								PORTC = 255;    // PULL-UP

#define READ_DIP (PINC & 3)
#define LED_ON  (PORTD |= (1<<6))
#define LED_OFF  (PORTD &= ~(1<<6))

#define LED2_ON  (PORTD |= (1<<5))
#define LED2_OFF  (PORTD &= ~(1<<5))

#define LED_CONFIG (DDRD = ((1<<5) | (1<<6)))

int readDipSwitch(void);
int getCurrentButton(void);
void signal(int del,int number) ;

//int macroFinished(void);
//int getCurrentDuration(void);


//int restartCommand;
//command *step;
//int numMacroElements;
//int macroChoice = 0 ;


// Main entry point.
int main(void) {
								// We'll start by performing hardware and peripheral setup.
								setupMacroChoices();
								setupHardware();

								// We'll then enable global interrupts for our use.
								GlobalInterruptEnable();
								// Once that's done, we'll enter an infinite loop.
								for (;;)
								{
																// We need to run our task to process and deliver data for our IN and OUT endpoints.
																HIDTask();
																// We also need to run the main USB management task.
																USB_USBTask();
								}
}

void setupMacroChoices() {
								// TODO
								//	DIP_CONFIG ;
								DDRC = 0; // ALL C pins set to input.
								PORTC = 255; // PULLUP MODE for all pins.

								LED_CONFIG;
								LED_OFF;
								LED2_OFF ;

								int macroChoice = readDipSwitch();

								for(int i = 0; i < macroChoice + 1; i++) {
									LED_ON;
									_delay_ms(25);
									LED_OFF;
									_delay_ms(25);
								}
								initMacro(macroChoice) ;
								_delay_ms(100) ;

}
// 4 Dip Switch
int readDipSwitch() {
	return (PINC & 7); // just read in bottom 3 bits of port C
}


void SetupHardware(void) {
	setupHardware() ;

}

// Configures hardware and peripherals, such as the USB peripherals.
void setupHardware(void) {
								// We need to disable watchdog if enabled by bootloader/fuses.
				MCUSR &= ~(1 << WDRF);
				wdt_disable();

				// We need to disable clock division before initializing the USB hardware.
				clock_prescale_set(clock_div_1);
				// We can then initialize our hardware and peripherals, including the USB stack.
				// The USB stack should be initialized last.
				USB_Init();
				signal(100,16) ;
}

void Delay_Ms(uint16_t ms)
{
    while(ms--){
        _delay_ms(1);//built in util/delay.h
    }
}

void signal(int del,int number) {
	for (int i = 0 ; i < number ; i++) {
		LED_ON;
		LED2_OFF ;
		Delay_Ms(del);
		LED_OFF;
		LED2_ON ;
		Delay_Ms(del);
	}

	LED2_OFF ;
	LED_OFF ;

}
// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
		// We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
	// We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
								bool ConfigSuccess = true;

								// We setup the HID report endpoints.
								ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
								ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);

								// We can read ConfigSuccess to indicate a success or failure at this point.
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
								// We can handle two control requests: a GetReport and a SetReport.

								// Not used here, it looks like we don't receive control request from the Switch.
}

// Process and deliver data from IN and OUT endpoints.
void HIDTask(void) {
								// If the device isn't connected and properly configured, we can't do anything here.
								if (USB_DeviceState != DEVICE_STATE_Configured)
																return;

								// We'll start with the OUT endpoint.
								Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
								// We'll check to see if we received something on the OUT endpoint.
								if (Endpoint_IsOUTReceived())
								{
																// If we did, and the packet has data, we'll react to it.
																if (Endpoint_IsReadWriteAllowed())
																{
																								// We'll create a place to store our data received from the host.
																								USB_JoystickReport_Output_t joystickOutputData;
																								// We'll then take in that data, setting it up in our storage.
																								while(Endpoint_Read_Stream_LE(&joystickOutputData, sizeof(joystickOutputData), NULL) != ENDPOINT_RWSTREAM_NoError);
																								// At this point, we can react to this data.

																								// However, since we're not doing anything with this data, we abandon it.
																}
																// Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
																Endpoint_ClearOUT();
								}

								// We'll then move on to the IN endpoint.
								Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);
								// We first check to see if the host is ready to accept data.
								if (Endpoint_IsINReady())
								{
																// We'll create an empty report.
																USB_JoystickReport_Input_t joystickInputData;
																// We'll then populate this report with what we want to send to the host.
																getNextReport(&joystickInputData);
																// Once populated, we can output this data to the host. We do this by first writing the data to the control stream.
																while(Endpoint_Write_Stream_LE(&joystickInputData, sizeof(joystickInputData), NULL) != ENDPOINT_RWSTREAM_NoError);
																// We then send an IN packet on this endpoint.
																Endpoint_ClearIN();
								}
}

typedef enum {
								SYNC_CONTROLLER,
								SYNC_POSITION,
								BREATHE,
								PROCESS,
								CLEANUP,
								DONE
} State_t;
State_t state = SYNC_CONTROLLER;

#define ECHOES 2
int echoes = 0;
USB_JoystickReport_Input_t last_report;


void processJoystick(USB_JoystickReport_Input_t* const joyData) {
	// States and moves management
	switch (getCurrentJoyStick())
	{

			case UP:
											joyData->LY = STICK_MIN;
											break;

			case LEFT:
											joyData->LX = STICK_MIN;
											break;

			case DOWN:
											joyData->LY = STICK_MAX;
											break;

			case RIGHT:
											joyData->LX = STICK_MAX;
											break;

			case CONFIRM:
											joyData->Button |= SWITCH_PLUS;
											break;

			case X:
											joyData->Button |= SWITCH_X;
											break;


			case A:
											joyData->Button |= SWITCH_A;
											break;

			case B:
											joyData->Button |= SWITCH_B;
											break;

			case R:
											joyData->Button |= SWITCH_R;
											break;

			case THROW:
											joyData->LY = STICK_MIN;
											joyData->Button |= SWITCH_R;
											break;

			case TRIGGERS:
											joyData->Button |= SWITCH_L | SWITCH_R;
											break;

			default:
											joyData->LX = STICK_CENTER;
											joyData->LY = STICK_CENTER;
											joyData->RX = STICK_CENTER;
											joyData->RY = STICK_CENTER;
											joyData->HAT = HAT_CENTER;
											break;
	}

}

int led = 0 ;

// Prepare the next report for the host.
void getNextReport(USB_JoystickReport_Input_t* const joyData) {

								// Prepare an empty report
								memset(joyData, 0, sizeof(USB_JoystickReport_Input_t));
								joyData->LX = STICK_CENTER;
								joyData->LY = STICK_CENTER;
								joyData->RX = STICK_CENTER;
								joyData->RY = STICK_CENTER;
								joyData->HAT = HAT_CENTER;

								// Repeat ECHOES times the last report
								if (echoes > 0)
								{
										memcpy(joyData, &last_report, sizeof(USB_JoystickReport_Input_t));
										echoes--;
										return;
								}

								switch (state)
								{

										case SYNC_CONTROLLER:
											state = BREATHE;
											break;

										case SYNC_POSITION:
											state = BREATHE;
											break;

										case BREATHE:
											state = PROCESS;
											break;

										case PROCESS:
											led = !led ;
											if (!isCompleted()) {
												if (led) {
													LED_ON ;
												} else {
													LED_OFF ;
												}
											} else {
												LED_OFF ;
												if (led) {
													LED2_ON ;
												} else {
													LED2_OFF ;
												}
											}
											processJoystick(joyData) ;
											runMacro() ;
											break ;

										default:
											break ;


							}


								// Prepare to echo this report
								memcpy(&last_report, joyData, sizeof(USB_JoystickReport_Input_t));
								echoes = ECHOES;

}
