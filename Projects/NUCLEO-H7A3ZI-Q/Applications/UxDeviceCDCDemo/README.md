# __NUCLEO-H7A3ZI UxDeviceCDCDemo__

If you are not already familiar with eLooM-based firmwares, please consider to have a look at HelloWorld
and SensorManagerDemo example and README before running this demo.

**UxDeviceCDCDemo** shows how to integrate a **USB CDC class** based on **usbx** into an eLooM firmware.

Based on the SensorManagerDemo, it rises a bit more the complexity by adding the support for usbx.
In the original SensorManagerDemo, data were streamed via UART through the terminal console (via ST-LINK).
In UxDeviceCDCDemo instead, data are redirected to the CDC class.

The demo shows also how to updated and easily reconfigure a eLooM firmware.
In fact, this examples includes also a further project configuration called **DPUDemo** that changes the App.c
file to add a further eLooM component: **DPU**.
DpuDemo shows the complete processing chain from sensor configuration to DSP calculation, whose results
are showed on the debug terminal console still available via UART through ST-LINK.

DPU stands for **Digital Processing Unit** and provides a set of ready-to-use processing blocks able 
to apply some computation (process) over input data generated by a data source object (i.e.: raw data 
from SensorManager) and generate output data. 
It provides also an abstract class and a set of interfaces to simplify the creation of new DPU.

Each DPU is a class object, providing an implementation of the **IDPU2** interface, that is the abstract 
class **ADPU2**. 
This abstract class implements all the virtual functions of the IDPU2 interface except the 
**IDPU2_Process** function. 
All the concrete DPU provided by the component inherit from ADPU2, so each DPU must only provide the 
initialization function and the concrete implementation of the IDPU2_Process() virtual processing function.

DpuDemo shows the entire processing chain using the **Dummy_DPU2**, a DSP block that simply returns the length
of the input buffer and the first sample data.
User is free to modify or use it as a template to develop any kind of calculation or data manipulation.

DpuDemo handles the Dummy_DPU2 into a AManagedTask at application level called **ProcessTask**.

As for SensorManager demos, DpuDemo state machine is composed by 2 running states.
HelloWorldTask has been properly modified to handle the system state machine through the User Button.

To customize the example, change the clock tree, update peripherals and irq configurations, please have a
look at the **mx** folder, where there is the **.ioc** used for the available configuration and a README descibing
the full procedure to be followed.



## __Hardware and Software environment__

- This example runs on STM32H7 Devices.

- This example has been tested with STMicroelectronics [NUCLEO-H7A3ZI-Q](https://www.st.com/en/evaluation-tools/nucleo-h7a3zi-q.html) + [X-NUCLEO-IKS02A1](https://www.st.com/en/ecosystems/x-nucleo-iks02a1.html)
  and can be easily tailored to any other supported
  device and development board. 


## __How to use it?__

In order to make the program work, you must do the following:

- Open the project in STM32CubeIDE

- Rebuild all files and load your image into target memory

- Run the example

