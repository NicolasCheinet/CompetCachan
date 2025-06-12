# 1 "Isr.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.46\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "Isr.c" 2
void __attribute__((picinterrupt(("low_priority")))) myLowIsr(void)
{
    LowIsr();
}

void __attribute__((picinterrupt(("high_priority")))) myHighIsr(void)
{
    HighIsr();
}
