#include "include/ft_printf.h"

int main()
{
    // printf("%-100f.|\n", 123456578.12345678);
    // ft_printf("%-100f.|\n", 123456578.12345678);
    // printf("%*.*d|\n", 10, 4, 199);
    // ft_printf("%*.*d|\n", 10, 4, 199);
    // printf("%lld   %010.2%      %d %o dhsdjhdjkkjds %s|\n", 1000, 1234, 12345, "aloha");
    // ft_printf("%lld   %010.2%      %d %o dhsdjhdjkkjds %s|\n", 1000, 1234, 12345, "aloha");
    printf("%Lf|\n", LDBL_MAX);
    ft_printf("%Lf|\n", LDBL_MAX);
    return (0);
}