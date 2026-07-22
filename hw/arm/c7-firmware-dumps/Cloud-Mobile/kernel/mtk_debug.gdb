# mtk_debug.gdb - Custom kernel fuzzing macros

define rewind
    monitor loadvm start_kernel_ready
    set $pc = start_kernel
    delete
    echo [!] Rewound to start_kernel. All breakpoints cleared.\n
end

define analyze_abort
    echo \n[!] === DATA ABORT ANALYSIS ===\n
    echo [!] Offending Memory Address (r0):\n
    info reg r0
    
    echo \n[!] True LR (Caller Function @ pt_regs+56):\n
    set $true_lr = *(unsigned int *)($r2 + 56)
    info symbol $true_lr
    
    echo \n[!] True PC (Crashing Instruction @ pt_regs+60):\n
    set $true_pc = *(unsigned int *)($r2 + 60)
    info symbol $true_pc
    echo ===============================\n
end

define stub_success
    break $arg0
    commands
        silent
        set $r0 = 0
        set $pc = $lr
        continue
    end
    echo [!] Function stubbed to return 0 (Success): $arg0\n
end

define stub_error
    break $arg0
    commands
        silent
        # -14 is standard -EFAULT in Linux
        set $r0 = -14 
        set $pc = $lr
        continue
    end
    echo [!] Function stubbed to return -EFAULT: $arg0\n
end

define catch_aborts
    break do_DataAbort
    echo [!] Trap set for do_DataAbort.\n
    continue
end
