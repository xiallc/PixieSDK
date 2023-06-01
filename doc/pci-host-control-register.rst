CSR bit definitions
###################

A Pixie-16 module has several control registers that are used to
activate runs, download DSP code or reset DSP, control pull-up resistor
for the trigger lines on the backplane, and/or indicate module status to
the host computer. Software on the host computer can set and/or read
bits in these registers to control the operation of the Pixie-16 module
or monitor its internal status.

+---------------------------+---------+------+---------------------------------+
| Name                      | Bit     | Mode | Description                     |
+===========================+=========+======+=================================+
| RUNENABLE                 | 0       | RW   | Set to 0 to stop a run, 1 to    |
|                           |         |      | start a run                     |
+---------------------------+---------+------+---------------------------------+
| Download DSP code         | 1       | RW   | Set to 1 to enable DSP code     |
|                           |         |      | download. If 0 DSP is running.  |
+---------------------------+---------+------+---------------------------------+
| PCI Active read or write  | 2       | RW   | Reads 1 if PCI r/w is active    |
|                           |         |      | 0 otherwise.                    |
+---------------------------+---------+------+---------------------------------+
| Pull-up resistor control  | 3       | RW   | 0 if backplane SYNC lines are   |
|                           |         |      | not connected. 1 if the Wired-OR|
|                           |         |      | connect to the backplane.       |
+---------------------------+---------+------+---------------------------------+
| Reset DSP                 | 4       | RW   | Generate a DSP reset pulse by   |
|                           |         |      | calling Pixie_ReadCSR to get    |
|                           |         |      | this value then set this bit to |
|                           |         |      | 1. Finally write the original   |
|                           |         |      | value back to the System FPGA.  |
+---------------------------+---------+------+---------------------------------+
| Reserved                  | 5       |      |                                 |
+---------------------------+---------+------+---------------------------------+
| EXTFIFO_WML               | 6       | RO   | If 1 the data in the External   |
|                           |         |      | FIFO exceeds the watermark and  |
|                           |         |      | is ready to read out.           |
|                           |         |      | 0 otherwise.                    |
+---------------------------+---------+------+---------------------------------+
| Reserved                  | [12:7]  |      |                                 |
+---------------------------+---------+------+---------------------------------+
| RUNACTIVE                 | 13      | RO   | 1 if a run is active. 0 if not. |
+---------------------------+---------+------+---------------------------------+
| Reserved                  | 14      |      |                                 |
+---------------------------+---------+------+---------------------------------+
| CLREXTMEM_ACTIVE          | 15      | RO   | 1 if clearing external memory   |
|                           |         |      | is running. 0 otherwise.        |
+---------------------------+---------+------+---------------------------------+

