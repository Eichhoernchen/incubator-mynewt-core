/**
 * Copyright (c) 2015 Runtime Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stddef.h>
#include <errno.h>
#include <string.h>
#include "host/ble_gap.h"
#include "ble_hs_hci_batch.h"
#include "ble_gap_conn.h"

/**
 * Configures the connection event callback.  The callback is executed whenever
 * any of the following events occurs:
 *     o Connection creation succeeds.
 *     o Connection creation fails.
 *     o Connection establishment fails.
 *     o Established connection broken.
 */
void
ble_gap_set_connect_cb(ble_gap_connect_fn *cb, void *arg)
{
    ble_gap_conn_cb = cb;
    ble_gap_conn_arg = arg;
}

/**
 * Performs the Direct Connection Establishment Procedure, as described in
 * vol. 3, part C, section 9.3.8.
 *
 * @param addr_type             The peer's address type; one of:
 *                                  o BLE_HCI_ADV_PEER_ADDR_PUBLIC
 *                                  o BLE_HCI_ADV_PEER_ADDR_RANDOM
 *                                  o BLE_HCI_ADV_PEER_ADDR_PUBLIC_IDENT
 *                                  o BLE_HCI_ADV_PEER_ADDR_RANDOM_IDENT
 * @param addr                  The address of the peer to connect to.
 *
 * @return                      0 on success; nonzero on failure.
 */
int
ble_gap_direct_connection_establishment(uint8_t addr_type, uint8_t *addr)
{
    struct ble_hs_hci_batch_entry *entry;

    entry = ble_hs_hci_batch_entry_alloc();
    if (entry == NULL) {
        return ENOMEM;
    }

    entry->bhb_type = BLE_HS_HCI_BATCH_TYPE_DIRECT_CONNECT;
    entry->bhb_direct_connect.bwdc_peer_addr_type = addr_type;
    memcpy(entry->bhb_direct_connect.bwdc_peer_addr, addr,
           sizeof entry->bhb_direct_connect.bwdc_peer_addr);

    ble_hs_hci_batch_enqueue(entry);

    return 0;
}

/**
 * Enables Directed Connectable Mode, as described in vol. 3, part C, section
 * 9.3.3.
 *
 * @param addr_type             The peer's address type; one of:
 *                                  o BLE_HCI_ADV_PEER_ADDR_PUBLIC
 *                                  o BLE_HCI_ADV_PEER_ADDR_RANDOM
 *                                  o BLE_HCI_ADV_PEER_ADDR_PUBLIC_IDENT
 *                                  o BLE_HCI_ADV_PEER_ADDR_RANDOM_IDENT
 * @param addr                  The address of the peer to connect to.
 *
 * @return                      0 on success; nonzero on failure.
 */
int
ble_gap_directed_connectable(uint8_t addr_type, uint8_t *addr)
{
    struct ble_hs_hci_batch_entry *entry;

    entry = ble_hs_hci_batch_entry_alloc();
    if (entry == NULL) {
        return ENOMEM;
    }

    entry->bhb_type = BLE_HS_HCI_BATCH_TYPE_DIRECT_ADVERTISE;
    entry->bhb_direct_advertise.bwda_peer_addr_type = addr_type;
    memcpy(entry->bhb_direct_advertise.bwda_peer_addr, addr,
           sizeof entry->bhb_direct_advertise.bwda_peer_addr);

    ble_hs_hci_batch_enqueue(entry);

    return 0;
}