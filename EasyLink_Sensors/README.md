Example Summary
---------------
Example includes the EasyLink API and uses it to configure the RF driver to
transmit packets. This project should be run in conjunction with the
rfEasyLinkRx project.

For more information on the EasyLink API and usage refer to
http://processors.wiki.ti.com/index.php/SimpleLink-EasyLink

Peripherals Exercised
---------------------
* `Board_PIN_LED1` - Indicates that a packet has been transmitted
* `Board_PIN_LED2` - Indicates an abort which is expected after transmitting 10 packets
 `Board_PIN_LED1` & `Board_PIN_LED2` indicate an error

System Confirguration Tool (SysConfig) Setup
-------------
### Driver Configuration
* `RF` - RF Driver Configuration
    * Hardware Used - `RF Antenna Switch`
* `Power` - Power Manager Configuration
    * Default options used
* `GPIO` - GPIO Configuration
    * Hardware Used: `LaunchPad LED Red`
* `GPIO` - GPIO Configuration
    * Hardware Used: `LaunchPad LED Green`

### EasyLink Stack Configuration
* `Basic Configuration` - Basic EasyLink Stack Configuration
    * Enable Address Filtering - `True`
    * Addresses to Filter - `0xAA`
* `RF Settings` - PHY Configuration
    * Default PHY - `Custom`
    * Additional Supported PHYs - `Custom`

Resources & Jumper Settings
---------------------------
> If you're using an IDE (such as CCS or IAR), please refer to Board.html in your project
directory for resources used and board-specific jumper settings. Otherwise, you can find
Board.html in the directory &lt;SDK_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

Board Specific Settings
-----------------------
1. The default frequency is:
    - 433.92 MHz for the CC1350-LAUNCHXL-433
    - 433.92/490 MHz for the CC1352P-4-LAUNCHXL
    - 2440 MHz on the CC2640R2-LAUNXHL
    - 868.0 MHz for other launchpads
In order to change frequency, modify the smartrf_settings.c file. This can be
done using the code export feature in Smart RF Studio, or directly in the file
2. On the CC1352P1 the high PA is enabled (high output power) for all
Sub-1 GHz modes by default.
3. On the CC1352P-2 the high PA operation for Sub-1 GHz modes is not supported
4. On the CC1352P-4 the high PA is enabled (high output power) for all
Sub-1 GHz modes by default.
    - The center frequency for 2-GFSK is set to 490 MHz
    - **CAUTION:** The center frequency for SimpleLink long range (SLR) is set to 433.92 MHz,
    but the high output power violates the maximum power output requirement
    for this band
5. The CC2640R2 is setup to run all proprietary physical modes at a center
frequency of 2440 MHz, at a data rate of 250 Kbps

Example Usage
-------------
Run the example. Board_PIN_LED1 will toggle every 100 ms indicating a packet has
been transmitted. This will happen 10 times. The 11th transmission will then be
aborted, toggling Board_PIN_LED2. This cycle will continue.

Before running this application you should first start the rfEasyLinkRx on a
second board to see that the transmitted packets are received.

On the CC1352P-4, if the High PA is enabled, i.e. high output power, the user
may choose to set the output power, using the *EasyLink_setRfPower()*, to 20 dBm
instead of the default 12 dBm.

Application Design Details
--------------------------
This example shows how to use the EasyLink API to access the RF driver, set the
frequency and transmit packets. The RFEASYLINKTX_ASYNC define is used to select
between the Blocking or Async TX API.

The rfEasyLinkTx example will transmit a packet every 10 ms for 10 packets, if
RFEASYLINKTX_ASYNC is defined (as it is by default) then the 11th TX will be
scheduled, but will be aborted. This is to show an example of aborting a TX.
LED2 will toggle when a TX abort happens. Board_PIN_LED1 and Board_PIN_LED2 indicates an
error (not expected to happen). The TX/abort cycle will repeat indefinitely.

A single task, "rfEasyLinkTxFnx", configures the RF driver through the EasyLink
API and transmits messages.

EasyLink API
-------------------------
### Overview
The EasyLink API should be used in application code. The EasyLink API is
intended to abstract the RF Driver in order to give a simple API for
customers to use as is or extend to suit their application[Use Cases]
(@ref USE_CASES).

### General Behavior
Before using the EasyLink API:

  - The EasyLink Layer is initialized by calling EasyLink_init(). This
    initializes and opens the RF driver and configures a modulation scheme
    passed to EasyLink_init.
  - The RX and TX can operate independently of each other.

The following is true for receive operation:

  - RX is enabled by calling EasyLink_receive() or EasyLink_receiveAsync()
  - Entering RX can be immediate or scheduled
  - EasyLink_receive() is blocking and EasyLink_receiveAsync() is non-blocking
  - The EasyLink API does not queue messages so calling another API function
    while in EasyLink_receiveAsync() will return EasyLink_Status_Busy_Error
  - An Async operation can be canceled with EasyLink_abort()

The following apply for transmit operation:

  - TX is enabled by calling EasyLink_transmit(), EasyLink_transmitAsync()
    or EasyLink_transmitCcaAsync()
  - TX can be immediate or scheduled
  - EasyLink_transmit() is blocking and EasyLink_transmitAsync(),
    EasyLink_transmitCcaAsync() are non-blocking
  - EasyLink_transmit() for a scheduled command, or if TX cannot start
  - The EasyLink API does not queue messages so calling another API function
    while in either EasyLink_transmitAsync() or EasyLink_transmitCcaAsync()
    will return EasyLink_Status_Busy_Error
  - An Async operation can be canceled with EasyLink_abort()

### Error Handling
The EasyLink API will return EasyLink_Status containing success or error
  code. The EasyLink_Status codes are:

   - EasyLink_Status_Success
   - EasyLink_Status_Config_Error
   - EasyLink_Status_Param_Error
   - EasyLink_Status_Mem_Error
   - EasyLink_Status_Cmd_Error
   - EasyLink_Status_Tx_Error
   - EasyLink_Status_Rx_Error
   - EasyLink_Status_Rx_Timeout
   - EasyLink_Status_Busy_Error
   - EasyLink_Status_Aborted

### Power Management
The power management framework will try to put the device into the most
power efficient mode whenever possible. Please see the technical reference
manual for further details on each power mode.

The EasyLink Layer uses the power management offered by the RF driver Refer to the RF
Driver documentation for more details.

# No-RTOS Implementation #
The No-RTOS implementation uses *usleep()* to implement the timeout
feature for the asynchronous case.

### Supported Functions
    | Generic API function          | Description                                        |
    |-------------------------------|----------------------------------------------------|
    | EasyLink_init()               | Init's and opens the RF driver and configures the  |
    |                               | specified settings based on EasyLink_Params struct |
    | EasyLink_transmit()           | Blocking Transmit                                  |
    | EasyLink_transmitAsync()      | Non-blocking Transmit                              |
    | EasyLink_transmitCcaAsync()   | Non-blocking Transmit with Clear Channel Assessment|
    | EasyLink_receive()            | Blocking Receive                                   |
    | EasyLink_receiveAsync()       | Nonblocking Receive                                |
    | EasyLink_abort()              | Aborts a non blocking call                         |
    | EasyLink_enableRxAddrFilter() | Enables/Disables RX filtering on the Addr          |
    | EasyLink_getIeeeAddr()        | Gets the IEEE Address                              |
    | EasyLink_setFrequency()       | Sets the frequency                                 |
    | EasyLink_getFrequency()       | Gets the frequency                                 |
    | EasyLink_setRfPower()         | Sets the Tx Power                                  |
    | EasyLink_getRfPower()         | Gets the Tx Power                                  |
    | EasyLink_getRssi()            | Gets the RSSI                                      |
    | EasyLink_getAbsTime()         | Gets the absolute time in RAT ticks                |
    | EasyLink_setCtrl()            | Set RF parameters, test modes or EasyLink options  |
    | EasyLink_getCtrl()            | Get RF parameters or EasyLink options              |
    | EasyLink_getIeeeAddr()        | Gets the IEEE address                              |


### Frame Structure
The EasyLink implements a basic header for transmitting and receiving data. This header supports
addressing for a star or point-to-point network with acknowledgments.

Packet structure:

     _________________________________________________________
    |           |                   |                         |
    | 1B Length | 1-64b Dst Address |         Payload         |
    |___________|___________________|_________________________|


Note for IAR users: When using the CC1310DK, the TI XDS110v3 USB Emulator must
be selected. For the CC1310_LAUNCHXL, select TI XDS110 Emulator. In both cases,
select the cJTAG interface.

EasyLink Stack Configuration using the System Configuration Tool (SysConfig)
-------------------------
### Overview
The purpose of SysConfig is to provide an easy to use interface for configuring 
drivers, RF stacks, and more. Many parameters of the EasyLink stack can be 
configured using SysConfig including address filtering, clear channel 
assessment parameters, RF settings, and advanced settings. 

### Basic Configuration
The basic EasyLink stack configuration includes address filtering, address 
size, and max data length. When these paramters are configured via SysConfig, 
they will take affect when the EasyLink_init() API is called.

### Clear Channel Assessment (CCA) Configuration
The CCA configuration parameters are used when checking if a channel is clear 
of traffic before transmitting. The combination of RSSI threshold and channel 
idle time define what is considered a "clear" channel. The max and min backoff 
window, backoff time units, and random number generation function define the 
behavior when the channel is sniffed and found to be busy. The CCA 
configuration parameters take affect when using the EasyLink_transmitCcaAsync() 
API. 

### Advanced Configuration
The advanced configuration parameters require in-depth knowledge of the 
EasyLink APIs. For more information on these configuration parameters please 
see the EasyLink_CtrlOption enumeration for use with the EasyLink_setStrl() API. 
When the advanced configuration parameters are configured via SysConfig, they 
take affect when the EasyLink_init() API is called with the exception of the 
Asynchronous Rx Timeout (ms) and the Test Mode Whitening paramters. These 
require the use of the EasyLink_receiveAsync() and EasyLink_setCtrl() APIs 
respectively.

### RF Configuration
The RF configuration of the EasyLink stack includes setup of both the RF driver 
and any PHYs the application needs to support. The RF Driver provides access to 
the radio core and offers a high-level interface for command execution that is
utilized by the EasyLink APIs. Configuration of the PHY settings includes 
selecting the default PHY that will be used when the EasyLink_init() API is 
called as well as any additional supported PHYs. As each PHY is added to the 
configuraiton a Radio Configuration module will provide the ability to 
customize settings such as frequency, power, etc. All the RF configuration 
parameters take affect when calling the EasyLink_init() API.

### Responsibilities of the Application
Several EasyLink stack parameters configured in SysConfig require the 
application to use specific EasyLink APIs to take affect. The following table 
shows which parameters that require the use of these APIs in the application.

    | Configuration Parameter(s)          | EasyLink API needed to take Affect                 |
    |-------------------------------------|----------------------------------------------------|
    | All parameters                      | EasyLink_init()                                    |
    | Clear Channel Assessment Parameters | EasyLink_transmitCcaAsync()                        |
    | Asynchronous Rx Timeout (ms)        | EasyLink_receiveAsync()                            |
    | Test Mode Whitening                 | EasyLink_setCtrl()                                 |

### Provided SysConfig Files
The .syscfg file provided with an EasyLink stack example project has been 
configured and tested with that project. Changes to the .syscfg file can and 
will alter the behavior of the example. Some stack parameters configured in 
SysConfig will require the use of specific EasyLink APIs to take affect. Please 
refer to the Responsibilities of the Application section of this document for 
further information on which configuration parameters require the use of 
specifc APIs.
