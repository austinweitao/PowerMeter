/*
 * Copyright © 2008-2013 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus/modbus.h>

#include "unit-test.h"

enum {
    TCP,
    TCP_PI,
    RTU
};


int main(int argc, char *argv[])
{
    uint8_t *tab_rp_bits;
    uint16_t *tab_rp_registers;
    uint16_t *tab_rp_registers_bad;
    modbus_t *ctx;
    int i;
    uint8_t value;
    int nb_points;
    int rc;
    float real;
    uint32_t ireal;
    struct timeval old_response_timeout;
    struct timeval response_timeout;
    int use_backend;
    uint16_t tmp_value;
    float float_value;

    use_backend = RTU;
    ctx = modbus_new_rtu("/dev/ttyUSB0", 19200, 'N', 8, 1);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to allocate libmodbus context\n");
        return -1;
    }
    modbus_set_debug(ctx, TRUE);
    modbus_set_error_recovery(ctx,
                              MODBUS_ERROR_RECOVERY_LINK |
                              MODBUS_ERROR_RECOVERY_PROTOCOL);
    modbus_set_slave(ctx, SERVER_ID);

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }


    /* Allocate and initialize the memory to store the registers */
    nb_points = 16; //max register number
    tab_rp_registers = (uint16_t *) malloc(nb_points * sizeof(uint16_t));
    memset(tab_rp_registers, 0, nb_points * sizeof(uint16_t));



    /** HOLDING REGISTERS **/

    /* Single register */

    rc = modbus_read_registers(ctx, 999,
                               2, tab_rp_registers);
    printf("modbus_read_registers 999: ");
    if (rc != 2) {
        printf("FAILED (nb points %d)\n", rc);
        goto close;
    }
    tmp_value = tab_rp_registers[0];
    tab_rp_registers[0] = tab_rp_registers[1];
    tab_rp_registers[1] = tmp_value;
   
    float_value = modbus_get_float(tab_rp_registers);
    printf("float value is %f\n",float_value);  

    printf("OK\n");


    rc = modbus_read_registers(ctx, 1001,
                               2, tab_rp_registers);
    printf("modbus_read_registers 1001: ");
    if (rc != 2) {
        printf("FAILED (nb points %d)\n", rc);
        goto close;
    }
    tmp_value = tab_rp_registers[0];
    tab_rp_registers[0] = tab_rp_registers[1];
    tab_rp_registers[1] = tmp_value;
   
    float_value = modbus_get_float(tab_rp_registers);
    printf("float value is %f\n",float_value);  

    printf("OK\n");

    rc = modbus_read_registers(ctx, 1003,
                               2, tab_rp_registers);
    printf("modbus_read_registers: 1003");
    if (rc != 2) {
        printf("FAILED (nb points %d)\n", rc);
        goto close;
    }
    tmp_value = tab_rp_registers[0];
    tab_rp_registers[0] = tab_rp_registers[1];
    tab_rp_registers[1] = tmp_value;
   
    float_value = modbus_get_float(tab_rp_registers);
    printf("float value is %f\n",float_value);  

    printf("OK\n");
    /* End of single register */


    printf("Report slave ID: \n");
    /* tab_rp_bits is used to store bytes */
    rc = modbus_report_slave_id(ctx, tab_rp_bits);
    if (rc == -1) {
        printf("FAILED\n");
        goto close;
    }

    /* Slave ID is an arbitraty number for libmodbus */
    if (rc > 0) {
        printf("OK Slave ID is %d\n", tab_rp_bits[0]);
    } else {
        printf("FAILED\n");
        goto close;
    }


close:
    /* Free the memory */
        printf("FAILED 1\n");
    free(tab_rp_bits);
        printf("FAILED 2\n");
    free(tab_rp_registers);

    /* Close the connection */
        printf("FAILED 3\n");
    modbus_close(ctx);
        printf("FAILED 4\n");
    modbus_free(ctx);

    return 0;
}

