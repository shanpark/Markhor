extern int __bss_start__;
extern int __bss_end__;

extern void Markhor(void);

__attribute__((naked))  void StartUp(void) {
    int* pos = &__bss_start__;
    int* end = &__bss_end__;

    while (pos < end)
        *pos++ = 0;

    Markhor();

    while (1)
        ;
}
