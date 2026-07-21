#ifndef HW_MISC_MTK_SYSREG_H
#define HW_MISC_MTK_SYSREG_H

#include "hw/core/sysbus.h"
#include "qom/object.h"

#define TYPE_MTK_SYSREG "mtk-sysreg"
OBJECT_DECLARE_SIMPLE_TYPE(MTKSysRegState, MTK_SYSREG)

struct MTKSysRegState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    MemoryRegion iomem;
    MemoryRegion iomem2;
    uint32_t length;
};

#endif
