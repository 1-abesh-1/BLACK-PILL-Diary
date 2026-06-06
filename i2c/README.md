## 🛠️ How the Code Works (As shown in i2c.png)

The flowchart in `i2c.png` maps our C code directly to the STM32 hardware steps:

![I2C Driver Code Flowchart](i2c.png)

* **1. Start (`i2c_start`)**
  * Tells the hardware to generate a START condition.
  * Loops until the **SB** (Start Bit) flag in `SR1` becomes `1`.

* **2. Send Address (`i2c_address_w`)**
  * Shifts the address left (`adr << 1`) and sends it.
  * Loops until the **ADDR** (Address Sent) flag in `SR1` becomes `1`.
  * **The "Secret Handshake":** Reads `SR1` and then `SR2` to clear the flag and unfreeze the clock line.

* **3. Send Data (`i2c_data_w`)**
  * Writes data to the Data Register (`DR`).
  * Loops until the **TXE** (Transmit Empty) flag in `SR1` becomes `1`. *(Clears itself automatically!)*

* **4. Stop (`i2c_stop`)**
  * Waits for the last byte to fully finish transmitting.
  * Tells the hardware to generate a STOP condition to release the lines.
