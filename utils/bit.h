#define set_bit(var, n) var |= 1UL << n
#define clear_bit(var, n) var &= ~(1UL << n)
#define toggle_bit(var, n) var ^= 1UL << n
#define check_bit(var, n) (var >> n) & 1U
