#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qobject/qlist.h"
#include "hw/arm/boot.h"
#include "hw/core/boards.h"
#include "hw/core/sysbus.h"
#include "system/address-spaces.h"
#include "qemu/units.h"
#include "qom/object.h"
#include "hw/arm/machines-qom.h"
#include "hw/char/serial.h"
#include "hw/char/pl011.h"
#include "hw/misc/unimp.h"
#include "hw/misc/mtk_sysreg.h"
#include "system/system.h"

#define MT6761_RAM_BASE   0x40000000
#define MT6761_UART0_BASE 0x11002000
#define MT6761_UART1_BASE 0x11003000
#define MT6761_GIC_BASE   0x0C000000

typedef struct Mt6761Peripheral {
    hwaddr base;
    hwaddr size;
    const char *name;
} Mt6761Peripheral;

static void mt6761_map_real_devices(DeviceState *gicdev)
{
    sysbus_create_simple("pl061", 0x10005000,
                         qdev_get_gpio_in(gicdev, 8));
    sysbus_create_simple("pl031", 0x10017000,
                         qdev_get_gpio_in(gicdev, 9));
    sysbus_create_simple("sp804", 0x10008000,
                         qdev_get_gpio_in(gicdev, 10));
    sysbus_create_simple("sp804", 0x10009000,
                         qdev_get_gpio_in(gicdev, 11));
    sysbus_create_simple("pl022", 0x1100a000,
                         qdev_get_gpio_in(gicdev, 12));
    sysbus_create_simple("pl022", 0x11010000,
                         qdev_get_gpio_in(gicdev, 13));
    sysbus_create_simple("pl022", 0x11012000,
                         qdev_get_gpio_in(gicdev, 14));
    sysbus_create_simple("pl022", 0x11013000,
                         qdev_get_gpio_in(gicdev, 15));
    sysbus_create_simple("generic-sdhci", 0x11230000, NULL);
    sysbus_create_simple("generic-sdhci", 0x11240000, NULL);
    sysbus_create_simple("versatile_i2c", 0x11007000, NULL);
    sysbus_create_simple("versatile_i2c", 0x11008000, NULL);
    sysbus_create_simple("versatile_i2c", 0x1100f000, NULL);
    sysbus_create_simple("versatile_i2c", 0x11011000, NULL);
    sysbus_create_simple("versatile_i2c", 0x11016000, NULL);
}

static void mt6761_map_peripherals(DeviceState *gicdev)
{
    static const Mt6761Peripheral peripherals[] = {
        { 0x10002000, 0x1000, "mt6761.io_cfg_lt" },
        { 0x10002200, 0x1000, "mt6761.io_cfg_lm" },
        { 0x10002400, 0x1000, "mt6761.io_cfg_lb" },
        { 0x10002600, 0x1000, "mt6761.io_cfg_bl" },
        { 0x10002800, 0x1000, "mt6761.io_cfg_bm" },
        { 0x10002a00, 0x1000, "mt6761.io_cfg_rb" },
        { 0x10002c00, 0x1000, "mt6761.io_cfg_rt" },
        { 0x10003000, 0x1000, "mt6761.pericfg" },
        { 0x10004000, 0x1000, "mt6761.efuse_dbg" },
        { 0x10006000, 0x1000, "mt6761.sleep" },
        { 0x10007000, 0x1000, "mt6761.toprgu" },
        { 0x10008000, 0x1000, "mt6761.apxgpt" },
        { 0x1000a000, 0x1000, "mt6761.hacc" },
        { 0x1000b000, 0x1000, "mt6761.apirq" },
        { 0x1000d000, 0x1000, "mt6761.pwrap" },
        { 0x1000f000, 0x1000, "mt6761.sleep_reg_md" },
        { 0x10010000, 0x1000, "mt6761.kp" },
        { 0x10011000, 0x1000, "mt6761.topmisc" },
        { 0x10012000, 0x1000, "mt6761.dvfsrc_top" },
        { 0x10013000, 0x1000, "mt6761.mbist_ao" },
        { 0x10014000, 0x1000, "mt6761.apcldmain_ao" },
        { 0x10014400, 0x1000, "mt6761.apcldmaout_ao" },
        { 0x10014800, 0x1000, "mt6761.apcldmamisc_ao" },
        { 0x10015000, 0x1000, "mt6761.mdcldmain_ao" },
        { 0x10015400, 0x1000, "mt6761.mdcldmaout_ao" },
        { 0x10015800, 0x1000, "mt6761.mdcldmamisc_ao" },
        { 0x1001a000, 0x1000, "mt6761.security_ao" },
        { 0x1001b000, 0x1000, "mt6761.topckgen_ao" },
        { 0x1001d000, 0x1000, "mt6761.dramc" },
        { 0x10200000, 0x1000, "mt6761.mcucfg" },
        { 0x10204000, 0x1000, "mt6761.sys_cirq" },
        { 0x10205000, 0x1000, "mt6761.m4u" },
        { 0x10207000, 0x1000, "mt6761.devapc" },
        { 0x10208000, 0x1000, "mt6761.bus_dbg" },
        { 0x10209000, 0x1000, "mt6761.ap_ccif0" },
        { 0x1020a000, 0x1000, "mt6761.md_ccif0" },
        { 0x1020b000, 0x1000, "mt6761.ap_ccif1" },
        { 0x1020c000, 0x1000, "mt6761.md_ccif1" },
        { 0x1020d000, 0x1000, "mt6761.infra_mbist" },
        { 0x1020e000, 0x1000, "mt6761.infracfg" },
        { 0x10210000, 0x1000, "mt6761.dxcc_sec" },
        { 0x10219000, 0x1000, "mt6761.emi" },
        { 0x11000000, 0x1000, "mt6761.ap_dma" },
        { 0x11001000, 0x1000, "mt6761.auxadc" },
        { 0x11006000, 0x1000, "mt6761.pwm" },
        { 0x11007000, 0x1000, "mt6761.i2c0" },
        { 0x11008000, 0x1000, "mt6761.i2c1" },
        { 0x11009000, 0x1000, "mt6761.i2c2" },
        { 0x1100a000, 0x1000, "mt6761.spi0" },
        { 0x1100b000, 0x1000, "mt6761.therm_ctrl" },
        { 0x1100c000, 0x1000, "mt6761.btif" },
        { 0x1100d000, 0x1000, "mt6761.irtx" },
        { 0x1100e000, 0x1000, "mt6761.disp_pwm0" },
        { 0x1100f000, 0x1000, "mt6761.i2c3" },
        { 0x11010000, 0x1000, "mt6761.spi1" },
        { 0x11011000, 0x1000, "mt6761.i2c4" },
        { 0x11012000, 0x1000, "mt6761.spi2" },
        { 0x11013000, 0x1000, "mt6761.spi3" },
        { 0x11016000, 0x1000, "mt6761.i2c5" },
        { 0x11018000, 0x1000, "mt6761.nfi" },
        { 0x11019000, 0x1000, "mt6761.nfiecc" },
        { 0x11200000, 0x1000, "mt6761.usb0" },
        { 0x11220000, 0x1000, "mt6761.audio" },
        { 0x11230000, 0x1000, "mt6761.msdc0" },
        { 0x11240000, 0x1000, "mt6761.msdc1" },
        { 0x11c00000, 0x1000, "mt6761.efusec" },
        { 0x11c10000, 0x1000, "mt6761.mipi_rx_ana_csi0" },
        { 0x11c20000, 0x1000, "mt6761.mipi_rx_ana_csi1" },
        { 0x11c40000, 0x1000, "mt6761.msdc1_pad_macro" },
        { 0x11cd0000, 0x1000, "mt6761.msdc0_pad_macro" },
        { 0x13000000, 0x1000, "mt6761.mfg_auckland" },
        { 0x13ffe000, 0x1000, "mt6761.mfg_cfg" },
        { 0x14000000, 0x1000, "mt6761.mmsys_config" },
        { 0x14001000, 0x1000, "mt6761.disp_mutex" },
        { 0x14002000, 0x1000, "mt6761.smi_common" },
        { 0x14003000, 0x1000, "mt6761.smi_larb0" },
        { 0x14004000, 0x1000, "mt6761.mdp_rdma0" },
        { 0x14005000, 0x1000, "mt6761.mdp_rsz0" },
        { 0x14006000, 0x1000, "mt6761.mdp_rsz1" },
        { 0x14007000, 0x1000, "mt6761.mdp_wdma0" },
    };

    for (size_t i = 0; i < ARRAY_SIZE(peripherals); i++) {
        create_unimplemented_device(peripherals[i].name,
                                    peripherals[i].base,
                                    peripherals[i].size);
    }

    /* MTK SysReg devices */
    DeviceState *dev;

    dev = qdev_new(TYPE_MTK_SYSREG);
    qdev_prop_set_uint32(dev, "length", 0x1000);
    object_property_add_child(OBJECT(gicdev), "topckgen", OBJECT(dev));
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, 0x10000000);

    dev = qdev_new(TYPE_MTK_SYSREG);
    qdev_prop_set_uint32(dev, "length", 0x1000);
    object_property_add_child(OBJECT(gicdev), "infracfg_ao", OBJECT(dev));
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, 0x10001000);

    dev = qdev_new(TYPE_MTK_SYSREG);
    qdev_prop_set_uint32(dev, "length", 0x1000);
    object_property_add_child(OBJECT(gicdev), "apmixed", OBJECT(dev));
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, 0x1000c000);

    mt6761_map_real_devices(gicdev);
}

static struct arm_boot_info mt6761_binfo;

static void mt6761_init(MachineState *machine)
{
    MemoryRegion *sysmem = get_system_memory();

    for (int i = 0; i < machine->smp.cpus; i++) {
        Object *cpuobj = object_new(machine->cpu_type);

        if (object_property_find(cpuobj, "has_el3")) {
            object_property_set_bool(cpuobj, "has_el3", false, &error_fatal);
        }

        qdev_realize(DEVICE(cpuobj), NULL, &error_fatal);
    }

    DeviceState *gicdev = qdev_new("arm-gicv3");
    qdev_prop_set_uint32(gicdev, "num-cpu", machine->smp.cpus);
    qdev_prop_set_uint32(gicdev, "num-irq", 160);
    qdev_prop_set_uint32(gicdev, "revision", 3);
    qdev_prop_set_bit(gicdev, "has-security-extensions", false);

    QList *redist_region_count = qlist_new();
    qlist_append_int(redist_region_count, machine->smp.cpus);
    qdev_prop_set_array(gicdev, "redist-region-count", redist_region_count);

    sysbus_realize_and_unref(SYS_BUS_DEVICE(gicdev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(gicdev), 0, MT6761_GIC_BASE);

    memory_region_add_subregion(sysmem, MT6761_RAM_BASE, machine->ram);
    pl011_create(MT6761_UART0_BASE,
                 qdev_get_gpio_in(gicdev, 123), serial_hd(0));
    pl011_create(MT6761_UART1_BASE,
                 qdev_get_gpio_in(gicdev, 124), serial_hd(1));
    mt6761_map_peripherals(gicdev);

    memset(&mt6761_binfo, 0, sizeof(mt6761_binfo));
    mt6761_binfo.loader_start = MT6761_RAM_BASE;
    mt6761_binfo.board_id = -1;
    mt6761_binfo.ram_size = machine->ram_size;
    mt6761_binfo.secure_boot = false;
    mt6761_binfo.is_linux = true;

    arm_load_kernel(ARM_CPU(first_cpu), machine, &mt6761_binfo);
}

static void mt6761_machine_init(MachineClass *mc)
{
    mc->desc = "Cloud Mobile Stratus C7 (MediaTek MT6761)";
    mc->init = mt6761_init;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("cortex-a53");
    mc->default_ram_size = 512 * MiB;
    mc->default_ram_id = "mt6761.ram";
    mc->default_cpus = 1;
    mc->min_cpus = 1;
    mc->max_cpus = 4;
}

DEFINE_MACHINE_AARCH64("mt6761-stratus", mt6761_machine_init)
