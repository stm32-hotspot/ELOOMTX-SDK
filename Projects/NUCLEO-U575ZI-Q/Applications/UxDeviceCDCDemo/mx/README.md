# __mx configuration for NUCLEO-U575ZI-Q board + Generic eLooM project__

This code support firmware on application-level modules (**Sensor Manager**, **EMData**, …) that can be reused and extended easily to design custom applications.
To enable this solution, this firmware has been built on top of **eLooM**, an Embedded Light Object Oriented fraMework for STM32 applications properly designed for embedded low power applications powered by STM32.

**STM32CubeMX** is used here to generate automatically the application clock tree and the peripheral configuration for the [NUCLEO-U575ZI-Q](https://www.st.com/en/evaluation-tools/nucleo-u575zi-q.html) board.

Here you can find the already generated files and the **.ioc** that you can use to modify and update the desired configuration for your custom development.

You can use the files contained into the **mx** folder as they are, or you are free to update peripherals configurations in STM32CubeMX as you need, by just following the instructions in the **How to use** paragraph.

Be aware that 'gpio.c/.h', 'mx.c' and 'main.h' maybe provided with some custom code properly added between the `/* USER_CODE */` macros.
In this way, once STM32CubeMX will re-generate the files, it will not overwrite those lines of code.

Be aware that the provided .ioc is not generating the complete embedded application. It is used only to setup the hardware configuration: peripherals and clock tree. The project generated by STM32CubeMX using this .ioc is not meant to work alone. We are only interested in peripherals source/header files, that will be used by the eLooM-based main application.

There is a known limitation in MDF peripheral configuration in STM32CubeMX which generates incomplete code. That's why we strongly suggest not to update the 'mdf.c/.h' files but to use the one already provided. Overwriting 'mdf.c/.h' files can result in a not working code.


## __Keywords__

AI, Datalog, Predictive Maintenance, Condition Monitoring, Signal Processing, Industry 4.0, Sensor, Ultrasound


## __Directory contents__

The **mx** folder contains:
 - **Inc** folder with the header files (one for each peripheral) generated by STM32CubeMX
 - **Src** folder with the **mx.c** source file used by the eLooM-based application to link resources from STM32CubeMX and source files (one for each peripheral) generated by STM32CubeMX
 - **NUCLEO-U575ZI-Q_eLooM_prj.ioc**, the STM32CubeMX project used to generate the above files.


## __Hardware and Software environment__

- This configuration runs on STM32U575 Devices.

- This configuration has been tested with STMicroelectronics NUCLEO-U575ZI-Q evaluation boards and can be easily tailored to any other supported device and development board through CubeMX.


## __How to use__

The mx folder provided here was tested in different project including NUCLEO-U575ZI-Q/HelloWorld_prj. If you don't need to change the default configuration, the files can be used as they are without any modification.

If you need to update the board configuration for your custom development, you can start from **NUCLEO-U575ZI-Q_eLooM_prj.ioc** STM32CubeMX project and regenerate the proper files. These are the steps:
- Open the .ioc project with STM32CubeMX
- Modify clock tree and peripherals according to your your needs.
- In _Project Manager --> Project_ avoid changing the Project location and let the following checkbox selected:
  - Do not generate the _main()_
  - Generate Under Root
  - Use latest available version
  - Use Default Firmware Location
- In _Project Manager --> CodeGenerator_:
  - Select 'Add necessary library files as reference in the toolchain project configuration file'
  - Select all the other checkboxes
  - If you are not interested in keeping the original files, you can uncheck 'Backup previously generated files when re-generating'
- Click on _GENERATE CODE_
  - **NUCLEO-U575ZI-Q_eLooM_prj.ioc** doesn't configure LPBAM. If a warning a warning pops up, ignore it and click Yes.

STM32CubeMX will now overwrites the files, updating the peripherals configuration as you need and keeping the content manually added between the `/* USER_CODE */` macros.
It will also generate an STM32CubeIDE project, with the related project files, linker scripts, drivers and startup. This project is not needed, remember that we are using STM32CubeMX only to setup the board configuration, peripherals and clock tree. The rest must be deleted.

Some manual configuration is needed to conclude the regeneration process:
- Delete all files and folders except for **Inc**, **Src** and the **.ioc** file
- eLooM-based firmware instantiates the clock tree into the **sysinit_mx.c** file. So you must manually copy the following functions from the automatically generated **main.c** into **sysinit_mx.c** (in _Project/Core/Src_):
  - SystemClock_Config()
  - PeriphCommonClock_Config()
- Declare PeriphCommonClock_Config as a static function into sysinit_mx.c and call it as last instruction in SystemClock_Config().
- Delete 'stm32*.h' files from **Inc** folder
- Delete 'main.c', 'stm32*.c', 'syscalls.c', 'sysmem.c' and 'system_stm32*.c' from **Src** folder

