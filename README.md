sac-v1
======

Proyecto SAC v1 (Sistema Automatizado de Conducción. Versión 1)

All the source code is written in PIC ASM, and is well commented.
The PCB and Schematics are for use with gEDA suite.
Aditional information can be found at http://sac.sindormir.net

Syvic

.
|-- AUTHOR
|-- CHANGELOG.es.txt
|-- LICENSE
|-- README
|-- boards
|   |-- DEBUG
|   |   |-- debug.pcb
|   |   `-- debug.sch
|   |-- UCC
|   |   |-- ucc.pcb
|   |   `-- ucc.sch
|   |-- VACIO
|   |   |-- display.pcb
|   |   |-- display.sch
|   |   |-- sensors.pcb
|   |   |-- sensors.sch
|   |   |-- vacio.pcb
|   |   `-- vacio.sch
|   `-- components
|       |-- 2mm-header-12.fp
|       |-- CNY70.sym
|       `-- cny70.pcb
`-- source
    |-- README.sources
    |-- common
    |   |-- eeprom.inc
    |   |-- sac.h
    |   `-- temps.inc
    |-- debuger
    |   `-- test-recvr.asm
    |-- logic
    |   `-- control-vacio.asm
    |-- motor-control
    |   |-- control-servo.asm
    |   `-- movs.inc
    |-- ucc
    |   |-- control-panel.inc
    |   |-- led-pos.inc
    |   `-- ucc.asm
    `-- wireless
        |-- cyspi.inc
        |-- cywmsr.inc
        `-- cywusb6935.inc
