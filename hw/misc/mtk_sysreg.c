#include "qemu/osdep.h"
#include "qemu/log.h"
#include "hw/core/sysbus.h"
#include "hw/core/qdev-properties.h"
#include "hw/misc/mtk_sysreg.h"

static uint64_t mtk_sysreg_read(void *opaque, hwaddr offset, unsigned size)
{
    if (offset & (size - 1)) {
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: unaligned read at offset 0x%" HWADDR_PRIx "\n",
                      __func__, offset);
        return 0;
    }

    qemu_log_mask(LOG_UNIMP,
                  "%s: read at offset 0x%" HWADDR_PRIx "\n",
                  __func__, offset);
    return 0;
}

static void mtk_sysreg_write(void *opaque, hwaddr offset,
                             uint64_t value, unsigned size)
{
    if (offset & (size - 1)) {
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: unaligned write at offset 0x%" HWADDR_PRIx "\n",
                      __func__, offset);
        return;
    }

    qemu_log_mask(LOG_UNIMP,
                  "%s: write at offset 0x%" HWADDR_PRIx " value 0x%" PRIx64 "\n",
                  __func__, offset, value);
}

static const MemoryRegionOps mtk_sysreg_ops = {
    .read = mtk_sysreg_read,
    .write = mtk_sysreg_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = {
        .min_access_size = 1,
        .max_access_size = 8,
        .unaligned = true,
    },
};

static void mtk_sysreg_realize(DeviceState *dev, Error **errp)
{
    MTKSysRegState *s = MTK_SYSREG(dev);
    SysBusDevice *sbd = SYS_BUS_DEVICE(dev);

    memory_region_init_io(&s->iomem, OBJECT(s), &mtk_sysreg_ops, s,
                          "mtk-sysreg", s->length);
    sysbus_init_mmio(sbd, &s->iomem);
}

static const Property mtk_sysreg_properties[] = {
    DEFINE_PROP_UINT32("length", MTKSysRegState, length, 0x1000),
};

static void mtk_sysreg_class_init(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    device_class_set_props(dc, mtk_sysreg_properties);
    dc->realize = mtk_sysreg_realize;
}

static const TypeInfo mtk_sysreg_info = {
    .name          = TYPE_MTK_SYSREG,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(MTKSysRegState),
    .class_init    = mtk_sysreg_class_init,
};

static void mtk_sysreg_register_types(void)
{
    type_register_static(&mtk_sysreg_info);
}

type_init(mtk_sysreg_register_types)
